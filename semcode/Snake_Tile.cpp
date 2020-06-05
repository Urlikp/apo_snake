/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  Snake_Tile.cpp      - snake_tile file

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/
#include "Rectangle.cpp"

class Snake_Tile{
        //int width, height;
        int x, y;
    public:
        Snake_Tile(int, int);
        int get_x(){return x;};
        int get_y(){return y;};
        Rectangle* get_occupied_field(int w, int h){
            return new Rectangle(x, y, w, h);
        }
};

Snake_Tile::Snake_Tile(int start_x, int start_y){
    x = start_x;
    y = start_y;
}