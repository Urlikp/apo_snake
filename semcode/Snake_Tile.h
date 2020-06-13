/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Snake_Tile.h - class that implements Snake tiles

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "Rectangle.h"

/*
    This class represents snake tiles
*/
class Snake_Tile
{
        /*
            coords
        */
        int x, y;
    public:
    
        /*
            Directions
        */
        enum Direction{UP, LEFT, DOWN, RIGHT, NONE};
        Direction dir;

        /*
            Constructor without parameters needs to be for
            creating new snake tile in Snake
        */
        Snake_Tile()
        {
        	x=0; 
        	y=0;
    	};

        /*
            Constructor of snake tile with two parameters
            @params
            int start_x = xtile
            int start_y = xtile
        */
        Snake_Tile(int start_x, int start_y)
        {
                x = start_x;
                y = start_y;
                dir = NONE;
        }

        /*
            Getters and setters
        */
        int get_x(){return x;}
        int get_y(){return y;}
        void set_x(int new_x){x=new_x;}
        void set_y(int new_y){y=new_y;}
        Direction get_direction(){return dir;}
        void set_direction(Direction new_dir){dir=new_dir;}
        
        /*
            Creates from enum direction string
            used for printing states of snake head
        */
        const char* print_direction()
        {
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

        /*
            Returns rectangle of occupied area of tile
            used in snake's method fill array
            @params
            -int w - width, int h - height, both in tilesize
            @return
            -rectangle
        */
        Rectangle* get_occupied_field(int w, int h)
        {
            return new Rectangle(x, y, w, h);
        }

        /*
            Checks if two snake tiles intersect each other
            @params
            Snake_Tile tile - testing subject
            int tile_size - size of tile.
            @return 
            True if two tiles intersect each other otherwise false 
        
        */
        bool intersects(Snake_Tile tile, int tile_size)
        {
            return get_occupied_field(tile_size,tile_size)->intersects(tile.get_occupied_field(tile_size, tile_size));
        }
};
