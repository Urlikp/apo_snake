/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  options.cpp - options screen

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "options.h"
/*
All functions are described in options.h
*/

int Menu_Options::options_selection(char key_pressed, int& speed, 
									int& size_of_tile, uint16_t& snake_1_color, 
									uint16_t& snake_2_color, 
									unsigned char* mem_base)
{   
    int return_value = 0;
    int number_of_items = 5;

    switch (key_pressed)
    {
    case 'w':
    	pointer_to_item--;
    	printf("Options: %d item\n", pointer_to_item);
    	break;
    case 's':
		pointer_to_item++;
		printf("Options: %d item\n", pointer_to_item);
    	break;
    case 'x':
        printf("Options: Back to menu.\n");
        return_value = 1;
    	break;
    case 'f':
    	switch (pointer_to_item)
        {
        case 1:
            speed++;
            if(speed > 5)
            {
                speed = 1;
            }
            copy_speed = speed;
            uint32_t val_line;
            switch (speed)
            {
            case 1:
            	val_line = LED_LINE_0;
            	break;
            case 2:
            	val_line = LED_LINE_1;
            	break;
            case 3:
            	val_line = LED_LINE_2;
            	break;
            case 4:
            	val_line = LED_LINE_3;
            	break;
            case 5:
            	val_line = LED_LINE_4;
            	break;
            default:
                val_line = LED_LINE_0;;
                break;
            }
            led_line(val_line, mem_base);
            break;
        case 2:
        	switch (size_of_tile)
            {
            case 16:
                size_of_tile = 20;
            	break;
            case 20:
                size_of_tile = 32;
            	break;
            case 32:
                size_of_tile = 16;
            	break;
        	}
            copy_size_of_tile = size_of_tile;
        case 3:
            pointer_to_color++;
            if(pointer_to_color >= size_of_selected_colors)
            {
                pointer_to_color = 0;
            }
            snake_1_color = selected_colors[pointer_to_color];
            break;
        case 4:
            pointer_to_color2++;
            if(pointer_to_color2 >= size_of_selected_colors)
            {
                pointer_to_color2 = 0;
            }
            snake_2_color = selected_colors[pointer_to_color2];
            break;
        case 5:
            return_value = 1;
            printf("Options: Back to menu.\n");
            break;
        }
        printf("Options: item %d SELECTED.\n", pointer_to_item);
    }
    
    if (pointer_to_item < 1)
    {
        pointer_to_item = number_of_items;
    }

    if(pointer_to_item > 5)
    {
        pointer_to_item = 1;
    }
    return return_value;
}

void Menu_Options::options_fill_array(uint16_t* fb, 
									unsigned char* parlcd_mem_base,  
									Game_Properties gp)
{
    uint16_t colors[2] = {BASE_COLOR, BASE_COLOR};
    colors[pointer_to_item % 2] = SELECT_COLOR;

    //GAME OPTIONS
    int xTile = STARTX_1_O, yTile = 5;
    char str[] = "OPTIONS";
    int size_of_str = 0;
    while (str[size_of_str] != 0)
    {
    	size_of_str++;
	}
    char* begin = str;
    draw_rect(STARTX_O, 0, WIDTH_O, HEIGHT_O, 3 * BORDER_O, BASE_COLOR, fb);
    draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);
    
    //FIRST TWO BUTTON
    if (pointer_to_item <= 2)
    {           
        int xTile = STARTX_1_O, yTile = STARTY_1_O;
        char str[] = "SPEED:  ";
        int size_of_str = 0;
        while (str[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        str[size_of_str - 1] = copy_speed + '0';
        char* begin = str;
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_O, STARTY_O, WIDTH_O, HEIGHT_O, BORDER_O, 
    			colors[1], fb);

        xTile = STARTX_1_O; yTile = STARTY_2_O;
        char str2[] = "SIZE:";
        size_of_str = 0;
        while (str2[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        begin = str2;
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        
        xTile = STARTX_1_O + 50; yTile = STARTY_2_O;
        // 10 must be enough to hold max 2 digit number (integer)
		char str3[10];
        sprintf(str3, "%d", copy_size_of_tile);
        size_of_str = 0;
        while (str3[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        begin = str3;
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        
        draw_rect(STARTX_O, STARTY_O+HEIGHT_O, WIDTH_O, HEIGHT_O, BORDER_O, 
        		colors[0], fb);        
    }
    //NEXT TWO BUTTONS
    else if (pointer_to_item <= 4)
    {
        int xTile = STARTX_1_O, yTile = STARTY_1_O;
        char str[] = "COLOR_1";
        int size_of_str = 0;
        while (str[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        char* begin = str;
        draw_filled_rect(STARTX_O + 25, STARTY_O + 25, 50, 50, 
        				selected_colors[pointer_to_color], fb);
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_O, STARTY_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[1], fb);
        
        xTile = STARTX_1_O; yTile = STARTY_2_O;
        char str2[] = "COLOR 2";
        size_of_str = 0;
        while (str2[size_of_str] != 0)
        {
        	size_of_str++;
    	}
        begin = str2;
        draw_filled_rect(STARTX_O + 25, STARTY_O + 25 + HEIGHT_O, 50, 50, 
        				selected_colors[pointer_to_color2], fb);
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        draw_rect(STARTX_O, STARTY_O + HEIGHT_O, WIDTH_O, HEIGHT_O, BORDER_O, 
    			colors[0], fb);
    }
    //NEXT ONE BUTTON
    else if (pointer_to_item<=5)
    {
        int xTile = STARTX_1_O, yTile = STARTY_1_O;
        char str[] = "BACK";
        int size_of_str = 0;
        while (str[size_of_str] != 0)
        {
        	size_of_str++;
        }
        char* begin = str;
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_O, STARTY_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[1], fb);
    }
}

