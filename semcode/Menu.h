/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Menu.h - menu screen

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Game_Properties.h"
#include "Render.h"

#ifndef BASE_COLOR
#define BASE_COLOR DARK_WHITE
#endif
#ifndef SELECT_COLOR
#define SELECT_COLOR BRIGHT_WHITE
#endif

//RECT
#define BORDER_M 5
#define WIDTH_M LCD_WIDTH
#define HEIGHT_M 100
#define STARTY_M 120
#define STARTX_M 0
//TEXT
#define STARTX_1_M LCD_WIDTH / 8 - 30
#define STARTY_1_M 35
#define STARTY_2_M 60


/*
Class used for game menu selection.
*/
class  Game_Menu
{
	int pointer = 1;
  public:
    /*
    Constructor creates base for class menu option
    */
    Game_Menu()
    {
		pointer = 1;
    }
    
    /*
    Function controls marked button in menu and changes the int value dependant 
    	on what is selected.
    	[w] = up, [s] = down, [f] = select, [x] = back to menu 
    @params 
		char key_pressed - pressed key
    @returns
		if [F] is pressed it returns number of selected button otherwise 0;
    */
    int menu_selection(char key_pressed);
    
    /*
    Function renders marked button
    @params 
		uint16_t* fb - to write into it in a render method
		unsigned char* parlcd_mem_base - to pass in render method
		Game_properties - to know which size could be used for render;
    */
    void menu_fill_array(uint16_t* fb, unsigned char* parlcd_mem_base, 
    					Game_Properties gp);
};
