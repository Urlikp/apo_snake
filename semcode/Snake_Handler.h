/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Snake_Handler.h - class that implements snake handler

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#ifndef H_H
#define H_H

#include "Snake.h"

#define ZERO 0
#define DEFAULT_HANDLER_SIZE 100

/*
Class that holds snakes. Easiest way how to use update or fill array of all 
	snakes.
*/
class Snake_Handler
{
    public:
        std::vector<Snake*> snakes;
        size_t size;
        
        /*
        Constructor of a Snake handler sets base values of class
        */
        Snake_Handler()
        {
            snakes.reserve(DEFAULT_HANDLER_SIZE);
            printf("Handler: inititalized.\n");
            size = ZERO;
        }
        
        /*
        delete all snakes 
        */
        void delete_snakes();
        

        /*
        add a new snake
        @param 
        	Snake* snake - new Snake to be added
        */
        void add_snake(Snake* snake);
        
        /*
        iterates over snakes and updates them
        */
        void update();

        /*
        iterates over snakes and fill the render array
        @params
            uint16_t* fb - render array
            int LCD_width - LCD width
        */
        void fill_array(uint16_t* fb, int LCD_width);
};

#endif
