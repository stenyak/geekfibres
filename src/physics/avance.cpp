#include "avance.h"

int Avance::GetAvanceX ( void )
{
    return avanceX;
}
int Avance::GetAvanceY ( void )
{
    return avanceY;
}

bool Avance::HayAvance ( void )
{
    return ( bool ( avanceX ) || bool ( avanceY ) );
}
void Avance::SetAvanceX ( int avX )
{
    avanceX = avX;
}
void Avance::SetAvanceY ( int avY )
{
    avanceY = avY;
}
void Avance::SetAvance ( int avX, int avY )
{
    avanceX = avX;
    avanceY = avY;
}
void Avance::SetAvanceCuadrante ( int avX, int avY, int cuadrante )
{
    SetAvance ( avX, avY );
    InvertirACuadrante ( cuadrante );

}
void Avance::InvertirACuadrante ( int cuadrante )
{
    switch ( cuadrante )
    {
    case 7:
        avanceX = -avanceX;
        break;
    case 1:
        avanceX = -avanceX;
        avanceY = -avanceY;
        break;
    case 3:
        avanceY = -avanceY;
        break;
    default:
        break;
    }
}
