
#ifndef HANDLER_INCLUDE
#define HANDLER_INCLUDE

#include "Snake_Handler.h"

using namespace std;

/*

    All described in Snake_Handler.h

*/


void Snake_Handler::delete_snakes(){
    for (size_t i = 0; i < size; i++)
    {
        snakes[i]->delete_snake_tiles();
    }
    size = ZERO;
    snakes.clear();
}


void Snake_Handler::add_snake(Snake* snake){
        size++;
        snakes.emplace_back(snake);
}


void Snake_Handler::update(){
    for (size_t i = 0; i < size; i++)
    {
        snakes[i]->update();
    }
}

void Snake_Handler::fill_array(uint16_t* fb, int LCD_width){
    for (size_t i = 0; i < size; i++)
    {
        snakes[i]->fill_array(fb, LCD_width);
    } 
}

#endif