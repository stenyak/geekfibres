class Avance
{
  private:
    int avanceX;
    int avanceY;
  public:
    int GetAvanceX ( void );
    int GetAvanceY ( void );
    bool HayAvance ( void );
    void SetAvanceX ( int avX );
    void SetAvanceY ( int avY );
    void SetAvance ( int avX, int avY );
    void SetAvanceCuadrante ( int avX, int avY, int cuadrante );
    void InvertirACuadrante ( int cuadrante );
};
