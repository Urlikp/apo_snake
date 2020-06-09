/*******************************************************************
  Simple program to move in menu on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  Menu.h      - 

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Game_Properites.h"
#include "Render.h"
#include "Snake.h"



#define BASE_COLOR DARK_RED
#define SELECT_COLOR BRIGHT_RED
//RECT
#define BORDER_M 5
#define WIDTH_M LCD_WIDTH
#define HEIGHT_M 100
#define STARTY_M 120
#define STARTX_M 0
//TEXT
#define STARTX_1_M LCD_WIDTH/8-30
#define STARTY_1_M 35
#define STARTY_2_M 60

int menu_selection(char key_pressed, int& pointer);

void menu_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base, int pointer, Game_Properties gp);