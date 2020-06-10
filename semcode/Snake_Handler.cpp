
#ifndef HANDLER_INCLUDE
#define HANDLER_INCLUDE

#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Snake.h"



#define MAXIMUM_OF_SNAKES 2
#define ZERO 0
#define DEFAULT_HANDLER_SIZE 100

using namespace std;

class Snake_Handler{
    public:
        std::vector<Snake*> snakes;
        int size;
        Snake_Handler(){
            snakes.reserve(DEFAULT_HANDLER_SIZE);
            printf("Handler: inititalized.\n");
            size = ZERO;
        }
        void delete_snakes(){
            for (size_t i = 0; i < size; i++)
            {
                snakes[i]->delete_snake_tiles();
            }
            size = ZERO;
            snakes.clear();
        }
        void add_snake(Snake* snake){
            size++;
            snakes.emplace_back(snake);
        }
        void update(){
            for (size_t i = 0; i < size; i++)
            {
                snakes[i]->update();
            }
        }
        void fill_array(uint16_t* fb, int LCD_width){
            for (size_t i = 0; i < size; i++)
            {
                snakes[i]->fill_array(fb, LCD_width);
            } 
        }
};

#endif