/*******************************************************************
  Simple program to move in menu on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  Menu.h      -     

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/



#include "Menu.h"

//pointer is for rendering to show which item could be selected to know 
// player where his cursor in menu is.
int menu_selection(char key_pressed, int& pointer){
    int return_value = 0;
    int number_of_items = 4;
    int pointer_to_item = pointer;
    
    if (key_pressed == 'f'){
        printf("select\n");
        return_value = pointer_to_item;
    }

    if (key_pressed == 'w'){
        printf("move up\n");
        pointer_to_item--;
    }

    if (key_pressed == 's'){
        printf("move down\n");
        pointer_to_item++;
    }
    
    if (key_pressed == 'x'){
        fprintf(stderr,"Game has been ended!\n");
        exit(0);
    }
    
    if (pointer_to_item<1)
    {
        printf("switched\n");
        pointer_to_item = number_of_items;
    }

    if(pointer_to_item>4){
        printf("switched2\n");
        pointer_to_item = 1;
    }
    pointer = pointer_to_item;
    return return_value;
}

void menu_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base, int pointer, Game_Properties gp){
    uint16_t colors[2] = {BASE_COLOR, BASE_COLOR};
    colors[pointer%2] = SELECT_COLOR;
    
    //GAME NAME
    int xTile = STARTX_1_M, yTile = 5;
    char str[]="S N A K E";
    int size_of_str = 0;
    while (str[size_of_str] != 0){size_of_str++;}
    char *begin = str;
    draw_rect(STARTX_M, 0, WIDTH_M, HEIGHT_M, 3*BORDER_M, BASE_COLOR, fb);
    draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);

    //MENU ITEMS
    if(pointer <=2){
        int xTile = STARTX_1_M, yTile = STARTY_1_M;
        char str[]="DEMO";
        int size_of_str = 0;
        while (str[size_of_str] != 0){size_of_str++;}
        char *begin = str;
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M, WIDTH_M, HEIGHT_M, BORDER_M, colors[1], fb);
        xTile = STARTX_1_M; yTile = STARTY_2_M;
        char str2[]="STANDARD";
        size_of_str = 0;
        while (str2[size_of_str] != 0){size_of_str++;}
        begin = str2;
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M+HEIGHT_M, WIDTH_M, HEIGHT_M, BORDER_M, colors[0], fb);
    }else if(pointer <=4){
        int xTile = STARTX_1_M, yTile = STARTY_1_M;
        char str[]="OPTIONS";
        int size_of_str = 0;
        while (str[size_of_str] != 0){size_of_str++;}
        char *begin = str;
        draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M, WIDTH_M, HEIGHT_M, BORDER_M, colors[1], fb);
        xTile = STARTX_1_M; yTile = STARTY_2_M;
        char str2[]="QUIT";
        size_of_str = 0;
        while (str2[size_of_str] != 0){size_of_str++;}
        begin = str2;
        draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
        draw_rect(STARTX_M, STARTY_M+HEIGHT_M, WIDTH_M, HEIGHT_M, BORDER_M, colors[0], fb);
    } 
}