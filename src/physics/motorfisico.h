#include "../actors/protagonista.h"
#include "../world/mundo.h"
#include "debug.h"
#include "stdio.h"
#include "../physics/avance.h"
#include "../physics/boundingbox.h"
#include "../actors/disparo.h"
#include "../actors/malo.h"

#define GRAVEDAD 12;

#define CAMARA_FACTOR_INERCIA 10
#define CAMARA_FACTOR_PREDICCION 15

void ActualizarEstadoDelMundo (Protagonista * prota, Mundo & mundo, float &posCamaraX, float &posCamaraY);
void AveriguaAvance (int posX, int posY, int velX, int velY, Avance & avance);
bool Colision (int posX, int posY, BoundingBox & protabb, BoundingBox & tilebb, Mundo & mundo, Protagonista * prota, bool avanzaEjeY);
bool ColisionMalo (int posX, int posY, BoundingBox & malobb, BoundingBox & tilebb, Mundo & mundo, Malo & malo, bool avanzaEjeY, BoundingBox & protabb, Protagonista * prota, int numeroMalo);
void AplicarFisicosAlProta (Protagonista * prota, Mundo & mundo);
void AplicarFisicosAlMalo (Malo & malo, Mundo & mundo, BoundingBox & protabbi, Protagonista * prota, int numeroMalo);
void AplicarFisicosALosDisparos (Protagonista * prota, Mundo & mundo);
void AplicarFisicosALosMalos (Protagonista * prota, Mundo & mundo);
