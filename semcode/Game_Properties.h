/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Game_Properties.h - global variables

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#ifndef GP_H
#define GP_H
#include <stdint.h>
/*
This struct is used to keep game properties.
*/
typedef struct Game_Properties
{
    int game_width;
    int game_height;
    int size_of_tile;
    int LCD_width;
    int LCD_height;
    int speed;
    uint16_t color_1; //color of snake 1
    uint16_t color_2; //color of snake 2
}Game_Properties;

#endif
