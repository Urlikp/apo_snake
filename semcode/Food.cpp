#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include "Snake_Handler.cpp"
//#include "Snake.h"

#define START_POSITION 0

using namespace std;

class Food{
        
        int max_x, max_y;
        int tile_size;
        int border, color;
        Snake_Handler* handler;
    private:
        void add_new_food();
        bool was_food_eaten();
    public:
    int food_x, food_y;
        Food(Snake_Handler* handler2, int x, int y, int border2, int color2, int max_x2, int max_y2, int tile_size2){
            srand((unsigned) time(0));
            handler = handler2;
            max_x = max_x2;
            max_y = max_y2;
            tile_size = tile_size2;
            border = border2;
            color = color2;
            start(x,y);
        }
        void start(int, int);
        void update();
        void fill_array(uint16_t *fb, int LCD_width);
};

void Food::start(int x, int y){
    food_x = x;
    food_y = y;
    for (size_t j = 0; j < handler->size; j++){
        handler->snakes[j]->set_position_of_food(food_x, food_y);
    }
}

void Food::update(){
    if (was_food_eaten())
    {
        add_new_food();
    }
}

void Food::fill_array(uint16_t *fb, int LCD_width){
        for (int i = food_y * tile_size; i < food_y * tile_size + border; i++) {
            for (int j = food_x * tile_size; j < food_x * tile_size + tile_size; j++) {
                fb[LCD_width * i + j] = color;
            }
        }
        for (int i = food_y * tile_size; i < food_y * tile_size + tile_size; i++) {
            for (int j = food_x * tile_size; j < food_x * tile_size + border; j++) {
                fb[LCD_width * i + j] = color;
            }
        }
        for (int i = food_y * tile_size; i < food_y * tile_size + tile_size; i++) {
            for (int j = food_x * tile_size + tile_size - border; j < food_x * tile_size + tile_size; j++) {
                fb[LCD_width * i + j] = color;
            }
        }
        for (int i = food_y * tile_size + tile_size - border; i < food_y * tile_size + tile_size; i++) {
            for (int j = food_x * tile_size; j < food_x * tile_size + tile_size; j++) {
                fb[LCD_width * i + j] = color;
            }
        }
}

void Food::add_new_food(){
    bool ret = true;
    while(ret){
        ret = false;
        food_x = rand() %(max_x/tile_size);
        food_y = rand() %(max_y/tile_size);
        for (size_t i = 0; i < handler->size && ret==false; i++)
        {
            int tmp_size = handler->snakes[i]->get_size();
            for (size_t j = 0; j < tmp_size && ret==false; j++)
            {
                Snake_Tile tmp = handler->snakes[i]->get_tile(j);
                if (tmp.get_x() == food_x && tmp.get_y()==food_y)
                {
                    ret = true;
                }
            }
        }
    }
    for (size_t j = 0; j < handler->size; j++){
        handler->snakes[j]->set_position_of_food(food_x, food_y);
    }
    
}

bool Food::was_food_eaten(){
    bool ret = false;
    for (size_t i = 0; i < handler->size && ret == false; i++)
    {
        Snake_Tile tmp = handler->snakes[i]->get_tile(0);
        printf("%d, %d \n", tmp.get_x(), tmp.get_y());
        if (tmp.get_x() == food_x && tmp.get_y()==food_y)
        {
            ret = true;
            handler->snakes[i]->add_snake_tile();
            if(i==0){

            }else{
                
            }
        } 
    }
    return ret;
}
