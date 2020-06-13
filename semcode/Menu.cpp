/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Menu.cpp - menu screen

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "Menu.h"
/*
All functions are described in Menu.cpp
*/

int Game_Menu::menu_selection(char key_pressed)
{
    int return_value = 0;
    int number_of_items = 4;
    
    switch (key_pressed)
    {
    case 'f':
    	return_value = pointer;
    	break;
    case 'w':
    	pointer--;
    	break;
    case 's':
		pointer++;
    	break;
    case 'x':
    	fprintf(stderr, "Game has ended!\n");
        return_value = 4;
    	break;
    }
    
    if (pointer < 1)
    {
        pointer = number_of_items;
    }

    if (pointer > number_of_items)
    {
        pointer = 1;
    }
    printf("Menu: %d item\n", pointer);
    return return_value;
}

void Game_Menu::menu_fill_array(uint16_t* fb, unsigned char* parlcd_mem_base, 
								Game_Properties gp)
{
    uint16_t colors[2] = {BASE_COLOR, BASE_COLOR};
    colors[pointer % 2] = SELECT_COLOR;

    //GAME NAME RENDER
    int xTile = STARTX_1_M, yTile = 5;
    char str[] = "S N A K E";
    int size_of_str = 0;
    while (str[size_of_str] != 0)
    {
    	size_of_str++;
	}
    char* begin = str;
    draw_rect(STARTX_M, 0, WIDTH_M, HEIGHT_M, 3 * BORDER_M, BASE_COLOR, fb);
    draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);

    //MENU BUTTONS 2 RENDER
    if (pointer <= 2)
    {
        int xTile = STARTX_1_M, yTile = STARTY_1_M;
        char str[] = "DEMO";
        int size_of_str = 0;
        while (str[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        char* begin = str;
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M, WIDTH_M, HEIGHT_M, BORDER_M, colors[1], fb);
        xTile = STARTX_1_M; yTile = STARTY_2_M;
        char str2[] = "STANDARD";
        size_of_str = 0;
        while (str2[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        begin = str2;
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M+HEIGHT_M, WIDTH_M, HEIGHT_M, BORDER_M, 
				colors[0], fb);
	}
    //MENU BUTTONS 4 RENDER
    else if(pointer <=4)
    {
        int xTile = STARTX_1_M, yTile = STARTY_1_M;
        char str[] = "OPTIONS";
        int size_of_str = 0;
        while (str[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        char* begin = str;
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M, WIDTH_M, HEIGHT_M, BORDER_M, colors[1], fb);
        xTile = STARTX_1_M; yTile = STARTY_2_M;
        char str2[] = "QUIT";
        size_of_str = 0;
        while (str2[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        begin = str2;
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M+HEIGHT_M, WIDTH_M, HEIGHT_M, BORDER_M, 
				colors[0], fb);
    } 

}
