#include "boundingbox.h"

void BoundingBox::Inicializar ( int posX, int posY, int ancho, int alto )
{
    limiteIzquierdo = posX;
    limiteDerecho = posX + ancho;
    limiteSuperior = posY + alto;
    limiteInferior = posY;
}

void BoundingBox::MoverEnEjeX ( int distancia )
{
    limiteIzquierdo += distancia;
    limiteDerecho += distancia;
}

void BoundingBox::MoverEnEjeY ( int distancia )
{
    limiteSuperior += distancia;
    limiteInferior += distancia;
}

int BoundingBox::HayColision ( const BoundingBox & bb )
{
    //algoritmo bounding box... he hecho varios de ellos

    //ALGORITMO DE LOS LADOS----------------------------------------------------

/*    //comprueba interseccion de los segmentos que hacen de lados
    if (bb.limiteInferior < limiteSuperior)
    {
        if ( (bb.limiteIzquierdo > limiteIzquierdo) &&
             (bb.limiteIzquierdo < limiteDerecho)    )
            return 2;
        if ( (bb.limiteDerecho < limiteDerecho) &&
             (bb.limiteDerecho > limiteIzquierdo))
            return 2;
    }	    
    if (bb.limiteIzquierdo < limiteDerecho)
    {
        if ( (bb.limiteSuperior > limiteSuperior) &&
             (bb.limiteSuperior < limiteInferior)    )
            return 4;
        if ( (bb.limiteInferior < limiteInferior) &&
             (bb.limiteInferior > limiteSuperior))
            return 4;
    }	    
    if (bb.limiteIzquierdo > limiteDerecho)
    {
        if ( (bb.limiteSuperior > limiteSuperior) &&
             (bb.limiteSuperior < limiteInferior)    )
            return 6;
        if ( (bb.limiteInferior < limiteInferior) &&
             (bb.limiteInferior > limiteSuperior))
            return 6;
    }	    
    if (bb.limiteInferior > limiteSuperior)
    {
        if ( (bb.limiteIzquierdo > limiteIzquierdo) &&
             (bb.limiteIzquierdo < limiteDerecho)    )
            return 8;
        if ( (bb.limiteDerecho < limiteDerecho) &&
             (bb.limiteDerecho > limiteIzquierdo))
            return 8;
    }	    
    return 0;
*/
    //----------------------------------------------------ALGORITMO DE LOS LADOS

    //ALGORITMO DE LAS ESQUINAS-------------------------------------------------
    //comprueba la penetracion de vertices. Es rapido.
    //solo apto para casos en los que las dimensiones de this sean menores que
    // las dimensiones de bb. es decir, que el prota debe ser más grande que
    // los tiles. si no es así, caerá 'a traves' si el prota está centrado e
    // intenta penetrar el tile por ese lao centrado.
/*    if ( bb.limiteSuperior > limiteInferior
         && bb.limiteDerecho > limiteIzquierdo )
        return 1;
    if ( bb.limiteSuperior > limiteInferior
         && bb.limiteIzquierdo < limiteDerecho )
        return 1;
    if ( bb.limiteInferior < limiteSuperior
         && bb.limiteIzquierdo < limiteDerecho )
        return 1;
    if ( bb.limiteInferior < limiteSuperior
         && bb.limiteDerecho > limiteIzquierdo )
        return 1;
    return 0;*/
    return ( ((bb.limiteIzquierdo < limiteDerecho) &&
        (bb.limiteDerecho > limiteIzquierdo) &&
        (bb.limiteInferior < limiteSuperior) &&
        (bb.limiteSuperior > limiteInferior))  ||
        ((limiteIzquierdo < bb.limiteDerecho) &&
        (limiteDerecho > bb.limiteIzquierdo) &&
        (limiteInferior < bb.limiteSuperior) &&
        (limiteSuperior > bb.limiteInferior)) );

    //-------------------------------------------------ALGORITMO DE LAS ESQUINAS
}

int BoundingBox::HayColision ( int x, int y )
{
    return ( y < limiteSuperior &&
             y > limiteInferior &&
             x < limiteDerecho &&
             x > limiteIzquierdo );
}

bool BoundingBox::operator!= (const BoundingBox & bb)
{
    return !(this->limiteSuperior == bb.limiteSuperior &&
        this->limiteInferior == bb.limiteInferior &&
        this->limiteIzquierdo == bb.limiteIzquierdo &&
        this->limiteDerecho == bb.limiteDerecho );
}
