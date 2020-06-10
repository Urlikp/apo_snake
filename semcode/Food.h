// #ifndef F_H
// #define F_H
// #include <stdlib.h>
// #include <stdio.h>
// #include <cstdlib>
// #include <ctime>
// #include <iostream>
// #include <random>

// #include "Snake_Handler.cpp"
// #include "Snake.h"


// #define START_POSITION 0

// 

// /*    
// class Food creates a food for snakes, if new food is created
// it lets know each snake where food is.
// */

// class Food{
        
//         int max_x, max_y;
//         int tile_size;
//         int border, color;
//         Snake_Handler* handler;
//     private:
        
//         /*to add new food*/
//         void add_new_food();

//         /*checks if food was eaten*/
//         bool was_food_eaten();
//     public:
//     int food_x, food_y;


//         /*    
//         Constructor of a Food

//         @params
//             Snake_Handler* handler2 -handler with snakes, before all snakes should be initialized in handler
//             int x, y - start coords of food
//             int border2 - is a width of rectangular edge that will be rendered
//             int color2 - is a color of food 
//             int max_x2, max_y0 - are a maximal sizes of a playground
//             int tile_size2 = is a tile size of game
//         */
//         Food(Snake_Handler* handler2, int x, int y, int border2, int color2, int max_x2, int max_y2, int tile_size2){
//             handler = handler2;
//             max_x = max_x2;
//             max_y = max_y2;
//             tile_size = tile_size2;
//             border = border2;
//             color = color2;
//             start(x,y);
//         }

//         /*
//         Sets coords of food 

//         @params
//             int x - x coord
//             int y - y coord
//         */
//         void start(int, int);


//         /*
        
//         Function called every update in a game checks
//         if food was eaten and if so, it creates new one 
        
//         */
//         void update();

        
//         /*

//         Render a food of x and y coords
        
//         @params
//             uint16_t* fb = to write into it in a render method
//             unsigned char * parlcd_mem_base = to pass to render method    
//         */
//         void fill_array(uint16_t *fb, int LCD_width);
// };
// #endif