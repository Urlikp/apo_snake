#ifndef C_H
#define C_H


#include "Game_Properites.h"
#include "Snake_Handler.h"
#include "Render.h"

/*

Collision_update checks each snake to do no wrong moves
    if so snake that made a wrong move his diode turn on to red
        snake 1 the right diode, snake 2 the left diode.
    - Simple rules of game snake
        - out of playground = death
        - head of snake crashed into movable parts of game = death
@params 
    Snake_Hanlder - to check each snake in handler
    Game_Properties - to know about sizes of game
    unsigned char* mem_base - to turn on LED diode
@returns
    true if some snake died otherwise false.

*/

bool collision_update(Snake_Handler handler, Game_Properties gm, unsigned char* mem_base);


#endif