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
#define STARTX_1_M LCD_WIDTH/8-LCD_WIDTH/10
#define STARTY_1_M 35
#define STARTY_2_M 60

class End_Scene{
    private:
        int counter;
        int moveX;
        int moveY;

    public:
        End_Scene(){
        counter = 0;
        moveX = STARTX_1_M;
        moveY = LCD_HEIGHT/4;
        }

        void end_scene_update();

        void end_scenefill_array(uint16_t* fb, unsigned char *parlcd_mem_base);
};