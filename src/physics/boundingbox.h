class BoundingBox
{
  private:
    int limiteIzquierdo;
    int limiteDerecho;
    int limiteSuperior;
    int limiteInferior;
  public:
//        BoundingBox(int posX, int posY, int ancho, int alto);
    void Inicializar ( int posX, int posY, int ancho, int alto );
    void MoverEnEjeX ( int distancia ); //desplaza la bounding box sobre el eje x
    void MoverEnEjeY ( int distancia ); //desplaza la bounding box sobre el eje y
    int HayColision ( const BoundingBox & bb );    //detecta colision entre dos bb; devuelve 2 4 6 u 8 segun el lao en el que hay colision
    int HayColision ( int x, int y );   //detecta colision entre un bb y un punto;
    bool operator!= (const BoundingBox & bb);
};
