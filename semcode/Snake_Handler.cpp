/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Snake_Handler.cpp - class that implements snake handler

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "Snake_Handler.h"
/*
All functions are described in Snake_Handler.h
*/

void Snake_Handler::delete_snakes()
{
    for (size_t i = 0; i < size; i++)
    {
        snakes[i]->delete_snake_tiles();
    }
    size = ZERO;
    snakes.clear();
}

void Snake_Handler::add_snake(Snake* snake)
{
    size++;
    snakes.emplace_back(snake);
}

void Snake_Handler::update()
{
    for (size_t i = 0; i < size; i++)
    {
        snakes[i]->update();
    }
}

void Snake_Handler::fill_array(uint16_t* fb, int LCD_width)
{
    for (size_t i = 0; i < size; i++)
    {
        snakes[i]->fill_array(fb, LCD_width);
    } 
}
