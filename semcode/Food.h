/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Food.cpp - class that implements food

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#ifndef F_H
#define F_H

#include <time.h>

#include "Render.h"
#include "Snake_Handler.h"

/*    
class Food creates a food for snakes, if new food is created it tells each snake
	where the food is. Food is generated randomly.
*/
class Food
{  
        int max_x, max_y;
        int tile_size;
        int border, color;
        int timer = 0;
        bool start_time = false;
        Snake_Handler* handler;
    private:
        
        /*
        to add new food
        */
        void add_new_food();

        /*
        checks if food was eaten
        */
        bool was_food_eaten(unsigned char* mem_base);
    public:
    	int food_x, food_y;

        /*    
        Constructor of a Food
        @params
            Snake_Handler* handler2 - handler of snakes
            int x, y - start coords of food
            int border2 - is a width of rectangular edge that will be rendered
            int color2 - is a color of food 
            int max_x2, max_y0 - are a maximal sizes of a playground
            int tile_size2 - size of tile (in pixels) in game
        */
        Food(Snake_Handler* handler2, int x, int y, int border2, int color2, 
        	int max_x2, int max_y2, int tile_size2)
    	{
            handler = handler2;
            max_x = max_x2;
            max_y = max_y2;
            tile_size = tile_size2;
            border = border2;
            color = color2;
            start(x,y);
            srand(time(NULL));
        }

        /*
        Sets coords of food 
        @params
            int x - x coord
            int y - y coord
        */
        void start(int, int);


        /*
        Function called every update in a game, it checks if food was eaten and 
        	if so, it creates new one and changes LED diode to blue based on 
        	which snake ate the food (snake 1 - left diode, snake 2 - right 
        	diode).
        @params
            unsigned char* mem_base - to turn on LED diodes
        */
        void update(unsigned char* mem_base);

        
        /*
        Render a food on x and y coords
        @params
            uint16_t* fb - to write into it in a render method
			int LCD_width - width of LCD display
        */
        void fill_array(uint16_t* fb, int LCD_width);
};
#endif
