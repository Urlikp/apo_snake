/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  options.h - options screen

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/
 
#ifndef O_H
#define O_H

#include "Game_Properties.h"
#include "Render.h"

//colors
#ifndef BASE_COLOR
#define BASE_COLOR DARK_WHITE
#endif
#ifndef SELECT_COLOR
#define SELECT_COLOR BRIGHT_WHITE
#endif

//Rectangle 
#define BORDER_O 5
#define WIDTH_O LCD_WIDTH
#define HEIGHT_O 100
#define STARTY_O 120
#define STARTX_O 0
//Text
#define STARTX_1_O LCD_WIDTH/8-30
#define STARTY_1_O 35
#define STARTY_2_O 60

/*
Class used for game options / settings to set game parameters.
*/
class Menu_Options
{
    private:
		int pointer_to_color;
		int pointer_to_color2;
		int pointer_to_item;
		int copy_size_of_tile;
		int copy_speed;
		int const size_of_selected_colors = 7;
		uint16_t selected_colors[7] = {BRIGHT_BLUE, BRIGHT_GREEN, DARK_BLUE, 
      									DARK_GREEN, PURPLE, YELLOW, ORANGE};
    public:
		/*
		Constructor creates base for class menu options
		*/
		Menu_Options()
      	{
  			pointer_to_color = 1;
        	pointer_to_color2 = 3;
        	pointer_to_item = 1;
        	copy_speed = 1;
        	copy_size_of_tile = 32;
      	}
      	
		/*
		Function controls marked button in options menu and changes the int 
			value dependant on what is selected.
			[w] = up, [s] = down, [f] = select, [x] = exit 
		@params 
			char key_pressed - pressed key
			int& speed - is a speed of snakes
			int& size_of_tile - is a size of game tile
			int& snake_1_color - the color of snake 1
			int& snake_2_color - the color of snake 2
		@returns
			if [F] is pressed it returns number of selected button otherwise 0;
		*/
		int options_selection(char key_pressed, int& speed, int& size_of_tile, 
							uint16_t& snake_1_color, uint16_t& snake_2_color, 
							unsigned char* mem_base);

		/*
		Function renders mmarked button
		@params 
			uint16_t* fb - to write into it in a render method
			unsigned char* parlcd_mem_base - to pass in render method
			Game_properties - to know which size could be used for render;
		*/
		void options_fill_array(uint16_t* fb, unsigned char* parlcd_mem_base, 
								Game_Properties gp);
};
#endif
