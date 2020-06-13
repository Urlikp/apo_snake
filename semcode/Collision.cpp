/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Collision.cpp - collision of snakes

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include "Collision.h"
/*
All functions are described in Collision.h
*/

bool collision_update(Snake_Handler handler, Game_Properties gm, 
					unsigned char* mem_base)
{
    size_t death_snake = -1;
    bool ret = false;
    for (size_t i = 0; i < handler.size && !ret; i++)
    {
        Snake_Tile test_head = handler.snakes[i]->get_tile(0);
        if (test_head.get_x() < 0 || 
        	test_head.get_x() >= gm.game_width / gm.size_of_tile)
        {
            printf("Collision: snake: %zu died by coord X.\n", i);
            death_snake = i;
            ret = true;
        }
        if (test_head.get_y() < 0 || 
        	test_head.get_y() >= gm.game_height / gm.size_of_tile)
        {
            printf("Collision: snake: %zu died by coord Y.\n", i);
        	death_snake = i;
            ret = true;
        }
        Snake *test = handler.snakes[i];
        for (size_t j = 1; j < test->get_size() && !ret; j++)
        {
            if (test_head.get_x() == test->get_tile(j).get_x() && 
            	test_head.get_y() == test->get_tile(j).get_y())
        	{
                death_snake = i;
                printf("Collision: snake: %zu died by eating himself.\n", i);
                ret = true;
            }
        }
        for (size_t j = 0; j < handler.size && !ret; j++)
        {
            if (j != i)
            {
                Snake *test2 = handler.snakes[j];
                for (size_t j = 0; j < test2->get_size() && !ret; j++)
                {
                    if (test_head.get_x() == test2->get_tile(j).get_x() && 
                    	test_head.get_y() == test2->get_tile(j).get_y())
                	{
                        printf("Collision: snake: %zu died by eating opponent snake.\n", i);
                        death_snake = i;
                        ret = true;
                    }
                }
            }
        }
    }
    if (ret)
    {
        //to turn on LED diodes
        if(death_snake == 0)
        {
            led_RGB1(LED_RED, mem_base);
        }
        else
        {
            led_RGB2(LED_RED, mem_base);
        }
        return ret;
    }
    return false;
}
