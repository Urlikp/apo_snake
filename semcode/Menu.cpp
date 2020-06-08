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

void menu_fill_array(uint16_t* fb, int& pointer, int color1, int color2, Game_Properties gp){

}