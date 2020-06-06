#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Snake.cpp"

#define DEFAULT_HANDLER_SIZE 2

using namespace std;


class Snake_Handler{
    public:
        std::vector<Snake> snakes;
        int size;
        Snake_Handler(){
            snakes.reserve(DEFAULT_HANDLER_SIZE);
            size = 0;
        }
        void delete_snakes(){
            size = 0;
            snakes.clear();
        }
        void add_snake(Snake snake){
            if(size<=2){
                size++;
                snakes.emplace_back(snake);
            }else{
                fprintf(stderr, "Cannot add more snakes!\n");
            }
        }
        void update(){
            for (size_t i = 0; i < size; i++)
            {
                snakes[i].update(1,1); //to delete 1,1
            }
        }
        void fill_array(uint16_t* fb, int LCD_width){
            for (size_t i = 0; i < size; i++)
            {
                snakes[i].fill_array(fb, LCD_width); //to delete 1,1
            } 
        }
};