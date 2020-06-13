/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Collision.h - collision of snakes

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#ifndef C_H
#define C_H

#include "Game_Properties.h"
#include "Snake_Handler.h"
#include "Render.h"

/*
Collision_update checks for collisions between snakes and walls, If snake dies
	because of collision his LED diode will change to red.
    Snake 1 - left diode, snake 2 - right diode.
    - Rules of snake game
        - out of playground = death
        - head of snake collides with snake tile = death
@params 
    Snake_Hanlder - to check each snake in handler
    Game_Properties - to know about sizes of game
    unsigned char* mem_base - to turn on LED diode
@returns
    true if any snake died otherwise false.
*/

bool collision_update(Snake_Handler handler, Game_Properties gm, unsigned char* mem_base);


#endif
