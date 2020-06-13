/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  end_scene.h - credits

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "Render.h"

#ifndef BASE_COLOR
#define BASE_COLOR DARK_WHITE
#endif

//TEXT
#define STARTX_1_ES (LCD_WIDTH / 8) - (LCD_WIDTH / 10)

/*
Class used when the game ends
*/
class End_Scene
{
    private:
        int counter;
        int moveX;
        int moveY;
        int max;

    public:

        /*
        Constructor of a class that creates an fundamentation of class
        	and sets values 
        */
        End_Scene()
        {
			counter = 0;
		    moveX = STARTX_1_ES;
        	moveY = LCD_HEIGHT / 4;
        	max = 400;
        }

        /*
        Getter
        */
        int get_max()
        {
        	return max;
    	}

        /*
        Function updates the moveY to create animation with text
        */
        void end_scene_update();

        /*
        Function renders the end text
        @params 
          uint16_t* fb - to write into it in a render method
          unsigned char* parlcd_mem_base - to pass in render method
        */
        void end_scenefill_array(uint16_t* fb, unsigned char* parlcd_mem_base);
};
