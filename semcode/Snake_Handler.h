
#ifndef H_H
#define H_H

#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


#include "Snake.h"


#define MAXIMUM_OF_SNAKES 2
#define ZERO 0
#define DEFAULT_HANDLER_SIZE 100


/*
Class that holds snakes.
Easiest way how to use 
update or fill array of all snakes.
*/
class Snake_Handler{
    public:
        std::vector<Snake*> snakes;
        size_t size;
        Snake_Handler(){
            snakes.reserve(DEFAULT_HANDLER_SIZE);
            printf("Handler: inititalized.\n");
            size = ZERO;
        }
        
        /*
        
         it deletes all snakes 
        
        */
        void delete_snakes();
        

        /*
        
         it adds new snake

        */
        void add_snake(Snake* snake);
        
        /*
        
            iterates over snakes and updates them

        */
        void update();

        /*
        
            iterates over snakes and fill array
        @params
            uint16_t* fb = to pass for snakes
            unsigned char * parlcd_mem_base = to pass to render method    

        */
        void fill_array(uint16_t* fb, int LCD_width);
};

#endif