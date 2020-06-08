/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  Snake_Tile.cpp      - snake_tile file

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "Rectangle.h"

class Snake_Tile{
        //int width, height;
        int x, y;
    public:
        /*Directions*/
        enum Direction{UP, LEFT, DOWN, RIGHT, NONE};
        Direction dir;
        Snake_Tile(){x=0; y=0;};
        Snake_Tile(int start_x, int start_y){
                x = start_x;
                y = start_y;
                dir = NONE;
        }
        int get_x(){return x;}
        int get_y(){return y;}
        void set_x(int new_x){x=new_x;}
        void set_y(int new_y){y=new_y;}
        Direction get_direction(){return dir;}
        void set_direction(Direction new_dir){dir=new_dir;}
        const char* print_direction(){
            switch (dir)
            {
            case UP:
                return "UP";
            case DOWN:
                return "DOWN";
            case LEFT:
                return "LEFT";
            case RIGHT:
                return "RIGHT";
            default:
                return "NONE";
            }
        }
        Rectangle* get_occupied_field(int w, int h){
            return new Rectangle(x, y, w, h);
        }
        bool intersects(Snake_Tile tile, int tile_size){
            return get_occupied_field(tile_size,tile_size)->intersects(tile.get_occupied_field(tile_size, tile_size));
        }
};