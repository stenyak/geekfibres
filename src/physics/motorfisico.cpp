#include "motorfisico.h"

// esto es el algoritmo de alto nivel del motor fisico. se encarga de actualizar
// todas las propiedades de los objetos fisicos, asi como de enviar peticiones
// de eventos al resto de motores del juego. por ejemplo, cuando el prota choca
// con el techo, puede enviar una peticion al soundengine para que, si éste lo
// ve oportuno, reproduzca un sonido "ouch.wav".
void ActualizarEstadoDelMundo (Protagonista * prota, Mundo & mundo, float &posCamaraX, float &posCamaraY)
{
	// pide al objeto prota que actualice sus aceleraciones en funcion de las
	// pulsaciones de teclao. realmente, lo principal que nos va a interesar
	// (al motor fisico) es la aceleracion 'deseada' por el usuario.
	// Por ejemplo, el usuario pulsa la tecla de salto, por tanto esta funcion
	// actualiza la propiedad aceleracion_eje_y a +100 or something.
	prota->Actualizate ();
	Malo::ActualizaTodos(int(floor(posCamaraX)), int(floor(posCamaraY)), prota);

	// actualiza posicion de los disparos
	Disparo::ActualizaTodos();

	// una vez que sabemos la aceleracion deseada por el usuario, se hacen los
	// calculos importantes del motor fisico: comprobar que esa aceleracion
	// deseada puede ser aplicada efectivamente, teniendo en cuenta el entorno
	// que le rodea (gravedad, colisiones con tiles....) 
	AplicarFisicosAlProta (prota, mundo);

	// aplicar fisicos a los enemigos. La IA no está aki, sino en la funcion de
	// actualizar de cada enemigo. Esto solo aplica las leyes fisicas sobre
	// las acciones que la IA pretende realizar.
	AplicarFisicosALosMalos (prota, mundo);

	// comprueba colisiones y demás fisicos de los disparos
	AplicarFisicosALosDisparos (prota, mundo);

	// actualizamos las posiciones de la camara (que sigue al prota).
	// se tienen en cuenta los factores de inercia y prediccion (definidos en el
	// archivo motorfisico.h), que permiten ver mejor a donde vas sin brusquedad
	// en el eje y se ha eliminado el factor de prediccion, porque el personaje
	// sufre aceleraciones instantaneas en el eje y (cuando salta), por tanto
	// la vista marea un poco y tal y tal...
	posCamaraX += float (prota->GetPosX () + 40 + float (prota->GetVelX ()) * CAMARA_FACTOR_PREDICCION - posCamaraX) / CAMARA_FACTOR_INERCIA;
	posCamaraY += float (prota->GetPosY () + 100 + float (prota->GetVelY ()) /* * CAMARA_FACTOR_PREDICCION*/  - posCamaraY) / CAMARA_FACTOR_INERCIA;

	// imprime en consola un poco de debugging...
	if (DEBUG)
		printf ("\nPosCamara=%f,%f", posCamaraX, posCamaraY);
}


// esta funcion es parte de las rutinas de deteccion y prevencion de colision
// esta funcion toma la posicion actual del prota (igualada a cero) y el vector
// de velocidad (de avance) deseado. como resultado, modifica el objeto 'avance'
// que representa el avance real del personaje para este variable timestep del
// motor fisico.... que viene a ser, el siguiente pixel al que tiene que avanzar
// el personaje...
// en efecto, todo esto implica que el motor fisico nunca permite penetraciones
// entre objetos (cosa que sí que suele pasar con motores fisicos 3d, donde las
// colisiones se calculan entre ecuaciones y no entre pixeles)... es decir, que
// con este motor fisico las cosas nunca van a "explotar", ni hace falta
// correccion de colision... todo ello gracias a que al trabajar con pìxeles
// podemos usar variables int para bastantes cosas.
// weno todo esto suena un poco complicao... pero... que diablos, pa qué
// engañarnos: sí que lo es ;)
void AveriguaAvance (int posX, int posY, int velX, int velY, Avance & avance)
{
	int cuadrante;              //9,7,1,3: como en teclao numerico

	// antes de ponernos a trabajar, pasamos los vectores al primer cuadrante...
	// de esta forma nos ahorramos comprobaciones del sentido de velocidad y
	// todo es algo mas facil de hacer... seguramente haya mejores formas de
	// hacerlo, pero weno, esto funciona y puedo entenderlo sin que mi cerebro
	// haga mucho swapping de memoria :P
	// cuadrantes: mira el numpad, y ahi los tienes: cuadrante 9,7,1 y 3 en
	// anticlockwise.
	if (velX >= 0)
	{
		if (velY >= 0)
		{
			cuadrante = 9;
		}else{
			cuadrante = 3;
			velY = -velY;
			posY = -posY;
		}
	}else{
		velX = -velX;
		posX = -posX;
		if (velY >= 0)
		{
			cuadrante = 7;
		}else{
			cuadrante = 1;
			velY = -velY;
			posY = -posY;
		}
	}


	// estos son unas rutinas warras que simplifican un poco el proceso en caso
	// de que el vector de velocidad (del frame actual) sea paralelo a *uno* de
	// de los ejes OX, OY.
	// la verdad es que al final el motor fisico no usa ninguna funcion de pseudo
	// anti-aliasing (leer mas abajo), asi que estas simplifiaciones forman
	// parte intrínseca del algoritmo.
	if ((velX == posX) && (velY == posY)) //si la velocidad deseada es cero...
	{
		avance.SetAvanceCuadrante (0, 0, cuadrante); //pos no avances nada.
		return;
	}
	if (velX == posX) //si no hay que avanzar en el eje OX...
	{
		if (velY > posY) //...si hay que avanzar en el OY
		{
			avance.SetAvanceCuadrante (0, +1, cuadrante); //...pos sube...
			return;
		}
		else //...o si no...
		{
			avance.SetAvanceCuadrante (0, 0, cuadrante); //...uedate quieto
			return;
		}
	}
	if (velY == posY) //same as above, pero para el otro eje
	{
		if (velX > posX)
		{
			avance.SetAvanceCuadrante (+1, 0, cuadrante);
			return;
		}
		else
		{
			avance.SetAvanceCuadrante (0, 0, cuadrante);
			return;
		}
	}

	// si no se pueden usar las simplificaciones... -> si el vector de velocidad
	// va en diagonal.
	if ((velX != posX) && (velY != posY))
	{
		// weno aki iba antes el susodicho algoritmo de 'pseudo-antialiasing'
		// (como lo llamo yo). este algoritmo cogia el vector de velocidad, y
		// averiguaba como llegar de la forma "lo más recta" posible. es muy
		// similar a los algoritmos usados para proyectar la ecuacion de una
		// recta en pantalla (en pixeles) (es decir, lo que cualquier gpu hace
		// hoy en dia en un abrir y cerrar de ojos.... weno mucho menos que eso         // ;).
		// pero se diferencia de estos en que siempre se debe poder pasar de un
		// pixel al siguiente moviendose en el eje X *o* en el eje Y. es decir,
		// sin diagonales (ambos a la vez).
		// inicialmente asi lo hacia, pero la verdad es que, para un juego de
		// estas caracteristicas, el realismo perfecto no es prioritario, y por
		// tanto he decidido no hacer uso del algoritmo este.
		// en vez de eso, ahora mismo se avanza en la direccion 0,1, es decir,
		// en el eje Y, en el sentido pertinente. esto hace que, a nivel de
		// avance por pixeles (que puede ser entendido como variableTimeStep),
		// el objeto no avanza como deberia (en diagonal), sino que va primero
		// en el eje y, y cuando a ha acabado con el eje y, recorre el eje x.
		// inicialmente pensé que daria lugar a glitches obvios en runtime, pero
		// los betatetings iniciales indican lo contrario.
		// el algoritmo inicial de antialiasing esta disponible en la release
		// 10unstable o así, cuando el motor fisico todavia estaba en pleno
		// desarrollo y todavia se usaban floats en vez de ints.
		// por cierto, lo de que avance en el eje y (0,1), pos es completamente
		// arbitrario, y se puede cambiar a (1,0). como el bug es inapreciable
		// incluso si se intenta observar a proposito (lo he intentao ver pero
		// no me ha sido posible... aunque está claro que existe), poco importa
		// lo que se elija.
		avance.SetAvanceCuadrante (0, 1, cuadrante);
		return;
	}
}

// esta funcion indica si ha habido colision entre la bounding box protabb, y
// *alguno* de los tiles cercanos, de bounding box tilebb.
// retorna true si se ha detectao colision con alguno de los tiles.
bool Colision (int posX, int posY, BoundingBox & protabb, BoundingBox & tilebb, Mundo & mundo, Protagonista * prota, bool avanzaEjeY)
{
	// lo primero que hacemos es averiguar qué tiles pueden estar colisionando
	// con la protabb. por supuesto, se podria comprobar colisiones a saco con
	// todos los tiles del mundo, pero no es plan. estas variables almacenan
	// informacion sobre qué tiles pueden realmente colisionar.
	// se trata de 'envolver' la protabb con el rectangulo formado por tiles mas
	// pequeño posible.

	// declaracion de las variables que se van a usar para almacenar el rango
	// de posibles tiles colisionantes
	int tileIndexInicioX;       //indice de matriz de tiles
	int tileIndexInicioY;       //indice de matriz de tiles
	int tileInicioPixelX;       //en pixeles
	int tileInicioPixelY;       //en pixeles
	int tileIndexFinalX;        //indice matriz
	int tileIndexFinalY;        //indice matriz

	// indica las dimensiones de la bundingbox del prota (posX y posY ya tienen
	// aplicados los offset para bounding box (en AplicarFisicosAlProta).
	protabb.Inicializar (posX, posY, prota->getBBWidth(), prota->getBBHeight());
	// inicializar las variables de rango de tiles
	tileIndexInicioX = posX / DIMENSION_TILE;
	tileIndexInicioY = posY / DIMENSION_TILE;
	tileInicioPixelX = tileIndexInicioX * DIMENSION_TILE;
	tileInicioPixelY = tileIndexInicioY * DIMENSION_TILE;
	tileIndexFinalX = (posX + prota->getBBWidth()) / DIMENSION_TILE;
	tileIndexFinalY = (posY + prota->getBBHeight()) / DIMENSION_TILE;
	tilebb.Inicializar (tileInicioPixelX, tileInicioPixelY, DIMENSION_TILE, DIMENSION_TILE);

	// desde aki empieza el algoritmo de deteccion con los tiles, uno a uno.
	// se inicia en el tile inferior izquierdo, y se acaba en el superior
	// derecho si no ha habido colisiones.
	// en cuanto hay una colision, el algoritmo de interrumpe esta funcion
	// termina devolviendo true.
	// si no hay colisiones, se acaba devolviendo un false.

	// inicializamos las variables. haHabidoColision es un int porque se puede
	// hacer que devuelva el eje y sentido en el que se ha producido la
	// "primera" colision (siguiendo el recorrido abajoizquierda-arribaderecha).
	int haHabidoColision = 0;
	int i = tileIndexInicioX;
	int j = tileIndexInicioY;

	// bucle de comprobaciones abajoizquierda-arribaderecha
	while ( /*(!haHabidoColision) &&*/ (!(j==tileIndexFinalY+1)) )
	{
		//comprueba la colision con el tile actual (si es solido, y si éste existe, claro).
		if (mundo.tilesMundo[i][j])
		{
            TileData * tileData = TileBuilder::GetTileDataByName(mundo.tilesMundo[i][j]->GetType());
            if (!tileData->isSolid)
			{
                if(tileData->isEndLevelPoint)
                {
						prota->setFinDeNivel();
                }
                if (tileData->doesIncreaseHealth)
                {
                        prota->aumentarVida();          // aumenta su vida
						delete mundo.tilesMundo[i][j];  // quita el powerup de memoria
						mundo.tilesMundo[i][j]=NULL;    // actualiza su puntero a NULL
					
				}
			}else{
				haHabidoColision += 1 /*protabb.HayColision(tilebb)*/;
				if ( avanzaEjeY && j == tileIndexFinalY)
				{
					if (mundo.tilesMundo[i][j]->disminuirNivelDeIntegridad())
					{
						mundo.tilesMundo[i][j]->CrearExplosiones(i*DIMENSION_TILE,j*DIMENSION_TILE);
						delete mundo.tilesMundo[i][j];
						mundo.tilesMundo[i][j]=NULL;
					}
				}
			}
		}
		// comprueba si hay que saltar a la linea superior de tiles
		if (++i > tileIndexFinalX)
		{
			// modifica el bbox de tile para que se ajuste al siguiente.
			i = tileIndexInicioX;
			j++;
		}
	}   
	static BoundingBox malobb;
	int numeroDeMalos = Malo::GetNumeroDeMalos();
	for (int maloActual = 0; maloActual < numeroDeMalos; maloActual++)
	{
		malobb.Inicializar (Malo::GetMaloNumero(maloActual)->GetPosX(), Malo::GetMaloNumero(maloActual)->GetPosY(), ANCHURA_MALO_BB, ALTURA_MALO_BB);
		if (malobb.HayColision(protabb))
		{
			prota->disminuirVidaTemporizado(5);
			//       haHabidoColision += 1;
		}
	}
	return (haHabidoColision);
}

bool ColisionMalo (int posX, int posY, BoundingBox & malobb, BoundingBox & tilebb, Mundo & mundo, Malo & malo, bool avanzaEjeY, BoundingBox & protabb, Protagonista * prota, int numeroMalo)
{
	// lo primero que hacemos es averiguar qué tiles pueden estar colisionando
	// con la protabb. por supuesto, se podria comprobar colisiones a saco con
	// todos los tiles del mundo, pero no es plan. estas variables almacenan
	// informacion sobre qué tiles pueden realmente colisionar.
	// se trata de 'envolver' la protabb con el rectangulo formado por tiles mas
	// pequeño posible.

	// declaracion de las variables que se van a usar para almacenar el rango
	// de posibles tiles colisionantes
	int tileIndexInicioX;       //indice de matriz de tiles
	int tileIndexInicioY;       //indice de matriz de tiles
	int tileInicioPixelX;       //en pixeles
	int tileInicioPixelY;       //en pixeles
	int tileIndexFinalX;        //indice matriz
	int tileIndexFinalY;        //indice matriz

	// indica las dimensiones de la bundingbox del prota (posX y posY ya tienen
	// aplicados los offset para bounding box (en AplicarFisicosAlProta).
	malobb.Inicializar (posX, posY, ANCHURA_MALO_BB, ALTURA_MALO_BB);
	// inicializar las variables de rango de tiles
	tileIndexInicioX = posX / DIMENSION_TILE;
	tileIndexInicioY = posY / DIMENSION_TILE;
	tileInicioPixelX = tileIndexInicioX * DIMENSION_TILE;
	tileInicioPixelY = tileIndexInicioY * DIMENSION_TILE;
	tileIndexFinalX = (posX + prota->getBBWidth()) / DIMENSION_TILE;
	tileIndexFinalY = (posY + prota->getBBHeight()) / DIMENSION_TILE;
	tilebb.Inicializar (tileInicioPixelX, tileInicioPixelY, DIMENSION_TILE, DIMENSION_TILE);

	// desde aki empieza el algoritmo de deteccion con los tiles, uno a uno.
	// se inicia en el tile inferior izquierdo, y se acaba en el superior
	// derecho si no ha habido colisiones.
	// en cuanto hay una colision, el algoritmo de interrumpe esta funcion
	// termina devolviendo true.
	// si no hay colisiones, se acaba devolviendo un false.

	// inicializamos las variables. haHabidoColision es un int porque se puede
	// hacer que devuelva el eje y sentido en el que se ha producido la
	// "primera" colision (siguiendo el recorrido abajoizquierda-arribaderecha).
	int haHabidoColision = 0;
	int i = tileIndexInicioX;
	int j = tileIndexInicioY;

	// bucle de comprobaciones abajoizquierda-arribaderecha
	while ( /*(!haHabidoColision) &&*/ (!(j==tileIndexFinalY+1)) )
	{
		//comprueba la colision con el tile actual (si es solido, y si éste existe, claro).
		if (mundo.tilesMundo[i][j])
		{
			if (!mundo.tilesMundo[i][j]->esTraspasable())
			{
				haHabidoColision += 1 /*malobb.HayColision(tilebb)*/;
				if ( avanzaEjeY && j == tileIndexFinalY)
				{
					if (mundo.tilesMundo[i][j]->disminuirNivelDeIntegridad())
					{
                        delete mundo.tilesMundo[i][j];
						mundo.tilesMundo[i][j]=NULL;
					}
				}
			}
		}
		// comprueba si hay que saltar a la linea superior de tiles
		if (++i > tileIndexFinalX)
		{
			// modifica el bbox de tile para que se ajuste al siguiente.
			//            tilebb.MoverEnEjeX (-(i * DIMENSION_TILE - tileInicioPixelX));
			i = tileIndexInicioX;
			//            tilebb.MoverEnEjeY (DIMENSION_TILE);
			j++;
		}
	}
	if (malobb.HayColision(protabb))
	{
		//  prota.disminuirVida();
		//        haHabidoColision += 1;
	}

	static BoundingBox otroMalobb;
	int numeroDeMalos = Malo::GetNumeroDeMalos();
	for (int maloActual = 0; maloActual < numeroDeMalos; maloActual++)
	{
		otroMalobb.Inicializar (Malo::GetMaloNumero(maloActual)->GetPosX(), Malo::GetMaloNumero(maloActual)->GetPosY(), ANCHURA_MALO_BB, ALTURA_MALO_BB);
		if (otroMalobb.HayColision(malobb))
		{
			if ((numeroMalo != maloActual))
			{
				haHabidoColision += 1;
			}
		}
	}
	return (haHabidoColision);
}

void AplicarFisicosALosMalos (Protagonista * prota, Mundo & mundo)
{
	// indica las dimensiones de la bundingbox del prota (posX y posY ya tienen
	// aplicados los offset para bounding box (en AplicarFisicosAlProta).
	BoundingBox protabb;
	protabb.Inicializar (prota->GetPosX(), prota->GetPosY(), prota->getBBWidth(), prota->getBBHeight());
	int numeroDeMalos = Malo::GetNumeroDeMalos();
	for (int maloActual = 0; maloActual < numeroDeMalos; maloActual++)
	{
		AplicarFisicosAlMalo (*Malo::GetMaloNumero(maloActual), mundo, protabb, prota, maloActual);
	}
}

void AplicarFisicosAlMalo (Malo & malo, Mundo & mundo, BoundingBox & protabb, Protagonista * prota, int numeroMalo)
{
	// se recogen los datos actuales del prota para procesarlos.
	int posX = malo.GetPosX () + OFFSET_X_MALO;   // pasamos a coordenadas fisicas
	int posY = malo.GetPosY () + OFFSET_Y_MALO;   // es decir, a boundingbox
	int velX = malo.GetVelX ();
	int velY = malo.GetVelY ();
	int accX = malo.GetAccX ();
	int accY = malo.GetAccY ();

	// ahora se realizan los calculos fisicos, stuff that matters
	accY -= GRAVEDAD;                // aplicar gravedad

	// ahora se aplica la aceleracion calculada a la velocidad
	velX += accX;
	velY += accY;

	// se limita la velocidad maxima de caida y de desplazamiento lateral
	velX = int (0.6 * float (velX));
	velY = int (0.8 * float (velY));

	// calculo y correcion de colisiones: primero se crea un par de bboxes
	BoundingBox malobb;
	BoundingBox tilebb;
	int posIntermediaX = 0;
	int posIntermediaY = 0;

	// este bucle va comprobando colisiones en todas las posiciones intermedias desde
	// posX,posY hasta posX+velX,posY+velY. de esta forma no hay efecto tunel: se
	// trata de un algoritmo que previene la penetracion fisica, al contrario que
	// la mayoria de motores fisicos que corrigen la penetracion una vez que ésta
	// se ha producido.
	// Todo esto significa que el prota se comporta de forma perfecta (o al menos
	// debería, espero que no habra bugs por ahí...).

	Avance avance;

	AveriguaAvance (posIntermediaX, posIntermediaY, velX, velY, avance);
	while (avance.HayAvance ())
	{
		// si toca avanzar en el eje X, asegurate de que no chocas con nada al hacerlo
		if (avance.GetAvanceX ())
		{
			if (ColisionMalo (posX + posIntermediaX + avance.GetAvanceX (), posY + posIntermediaY, malobb, tilebb, mundo, malo, false, protabb, prota, numeroMalo))
			{
				// si chocas al intentar avanzar, quedate donde estas y elimina la
				// componente horizontal de la velocidad.
				velX = posIntermediaX;
				if (avance.GetAvanceX () < 0)
				{
					// ha tocao muro izquierdo
					malo.NoPuedesIrIzquierda();
				}else{
					// ha tocao muro derecho
					malo.NoPuedesIrDerecha();
				}
			}
			else
			{
				// si no chocas, pos adelante maxote, avanza el pixel pertinente.
				posIntermediaX += avance.GetAvanceX ();
				malo.PuedesIrDerecha();
				malo.PuedesIrIzquierda();
			}
		}
		// si toca avanzar en el eje Y, asegurate de que no chocas con nada al hacerlo
		if (avance.GetAvanceY ())
		{
			if (ColisionMalo (posX + posIntermediaX, posY + posIntermediaY + avance.GetAvanceY (), malobb, tilebb, mundo, malo, true, protabb, prota, numeroMalo))
			{
				if (avance.GetAvanceY () < 0)
				{
					// ha tocao el suelo
					malo.PuedesSaltar ();
				}
				else
				{
					// ha tocao el techo
				}
				velY = posIntermediaY;
			}
			else
			{
				if (avance.GetAvanceY () < 0)
				{
					// puede avanzar hacia abajo
					malo.NoPuedesSaltar ();
				}
				posIntermediaY += avance.GetAvanceY ();
			}
		}
		AveriguaAvance (posIntermediaX, posIntermediaY, velX, velY, avance);
	}
	posX += posIntermediaX;
	posY += posIntermediaY;

	posX -= OFFSET_X_MALO;     // volvemos de coordenadas fisicas (bbox) a graficas
	posY -= OFFSET_Y_MALO;     // same here...

	// una vez hechos todos los calculos fisicos, se aplican al protagonista
	malo.SetPosVelX (posX, velX);
	malo.SetPosVelY (posY, velY);
}

void AplicarFisicosALosDisparos (Protagonista * prota, Mundo & mundo)
{
	int disparosTotales = Disparo::NumeroDisparos();
	Disparo * disparoActual;
	Malo * maloActual;
	int numeroDeMalos = Malo::GetNumeroDeMalos();
	BoundingBox malobb, protabb;
	
    protabb.Inicializar (prota->GetPosX (), prota->GetPosY (), prota->getBBWidth(), prota->getBBHeight());
	for (int numeroDisparoActual = 0;numeroDisparoActual < disparosTotales; numeroDisparoActual++)
	{
		disparoActual = Disparo::GetDisparo(numeroDisparoActual);
		if (disparoActual->DisparadoPorProta())
		{
			for (int numMaloActual = 0;numMaloActual < numeroDeMalos; numMaloActual++)
			{   
				maloActual = Malo::GetMaloNumero(numMaloActual);
				malobb.Inicializar (maloActual->GetPosX() + OFFSET_X_MALO, maloActual->GetPosY() + OFFSET_Y_MALO, ANCHURA_MALO_BB, ALTURA_MALO_BB);
				if (malobb.HayColision(disparoActual->GetPosX(),disparoActual->GetPosY()))
				{
					maloActual->DisminuyeVida(DANYO_BALAS);//Muerete();
					disparoActual->Destruyete();
				}
			}
		}else{
			if (protabb.HayColision(disparoActual->GetPosX(), disparoActual->GetPosY()))
			{
				prota->disminuirVida();
				disparoActual->Destruyete();
			}
		}
		int x = disparoActual->GetPosX() / DIMENSION_TILE;
		int y = disparoActual->GetPosY() / DIMENSION_TILE;
		if (x >= 0 && x < ANCHO_MUNDO && y >=0 && y < ALTO_MUNDO && mundo.tilesMundo[x][y])
		{
			if (!(mundo.tilesMundo[x][y]->esTraspasable()))
			{
				if (disparoActual->DisparadoPorProta() && mundo.tilesMundo[x][y]->disminuirNivelDeIntegridad(mundo.levelData->GetPlayerShotsDamage()))
				{
					mundo.tilesMundo[x][y]->CrearExplosiones(x*DIMENSION_TILE,y*DIMENSION_TILE);
					delete mundo.tilesMundo[x][y];
					mundo.tilesMundo[x][y]=NULL;
				}
				disparoActual->Destruyete();
			}
		}
	}
}

void AplicarFisicosAlProta (Protagonista * prota, Mundo & mundo)
{
	// se recogen los datos actuales del prota para procesarlos.
	int posX = prota->GetPosX ();
	int posY = prota->GetPosY ();
	int velX = prota->GetVelX ();
	int velY = prota->GetVelY ();
	int accX = prota->GetAccX ();
	int accY = prota->GetAccY ();

	// ahora se realizan los calculos fisicos, stuff that matters
	accY -= GRAVEDAD;                // aplicar gravedad


    velX += accX;	
	velY += accY;

	velX = int (0.6 * float (velX));
	velY = int (0.8 * float (velY));

	// calculo y correcion de colisiones: primero se crea un par de bboxes
	BoundingBox protabb;
	BoundingBox tilebb;
	int posIntermediaX = 0;
	int posIntermediaY = 0;

	// este bucle va comprobando colisiones en todas las posiciones intermedias desde
	// posX,posY hasta posX+velX,posY+velY. de esta forma no hay efecto tunel: se
	// trata de un algoritmo que previene la penetracion fisica, al contrario que
	// la mayoria de motores fisicos que corrigen la penetracion una vez que ésta
	// se ha producido.
	// Todo esto significa que el prota se comporta de forma perfecta (o al menos
	// debería, espero que no habra bugs por ahí...).

	Avance avance;

	AveriguaAvance (posIntermediaX, posIntermediaY, velX, velY, avance);
	bool haGritao = false;
	while (avance.HayAvance ())
	{
		// si toca avanzar en el eje X, asegurate de que no chocas con nada al hacerlo
		if (avance.GetAvanceX ())
		{
			if (Colision (posX + posIntermediaX + avance.GetAvanceX (), posY + posIntermediaY, protabb, tilebb, mundo, prota, false))
			{
				// si chocas al intentar avanzar, quedate donde estas y elimina la
				// componente horizontal de la velocidad.
				velX = posIntermediaX;

			}
			else
			{
				// si no chocas, pos adelante maxote, avanza el pixel pertinente.
				posIntermediaX += avance.GetAvanceX ();
			}
		}
		// si toca avanzar en el eje Y, asegurate de que no chocas con nada al hacerlo
		if (avance.GetAvanceY ())
		{
			if (Colision (posX + posIntermediaX, posY + posIntermediaY + avance.GetAvanceY (), protabb, tilebb, mundo, prota, true))
			{
				if (avance.GetAvanceY () < 0)
				{
					// ha tocao el suelo
					prota->PuedesSaltar ();
					prota->SetTouchingGround(true);
				}
				else
				{
					// ha tocao el techo
					if (haGritao)
					{
						haGritao = true;
						prota->PlayDoh();
					}
				}
				velY = posIntermediaY;
			}
			else
			{
				prota->SetTouchingGround(false);
				if (avance.GetAvanceY () < 0)
				{
					// puede avanzar hacia abajo
					prota->NoPuedesSaltar ();
				}
				posIntermediaY += avance.GetAvanceY ();
			}
		}
		AveriguaAvance (posIntermediaX, posIntermediaY, velX, velY, avance);
	}

	posX += posIntermediaX;
	posY += posIntermediaY;

	// una vez hechos todos los calculos fisicos, se aplican al protagonista
	prota->SetPosVelX (posX, velX);
	prota->SetPosVelY (posY, velY);

}
