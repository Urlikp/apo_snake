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
#include "Game_Properites.h"

int menu_selection(char key_pressed, int& pointer);

void menu_fill_array(uint16_t* fb, int& pointer, int color1, int color2, Game_Properties gp);