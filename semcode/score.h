/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  score.h - class that implements score

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#ifndef S_H
#define S_H

#include "Snake_Handler.h"
#include "Render.h"
#include <time.h>

#define BASE_COLOR_S BASE_COLOR

/*
Class Score is used to render and save scores of snakes, score of snake - size 
	of snake, easy peasy lemon squeezy...
*/
class Score
{
    private:
        int score_1;
        int score_2;
        uint16_t color_1;
        uint16_t color_2;
		struct timespec begin_clock, end_clock;
    public:
        /*
        Constructor of a Score sets base values of a score class
        */
        Score()
        {
            score_1 = 1;
            score_2 = 1;
            color_1 = 0xffff;
            color_2 = 0xffff;
        }
        
        /*
        Update, this method saves size of both snakes to class integer variables 
        	named score_x 
        @params
            Snake_Handler handler - to find snakes
        */
        void update(Snake_Handler handler);

        /*
        This method renders into fb game layout, creates 3 rectangles for score 
        	and time
        @params
            uint16_t* fb - to write into it in a render method
            unsigned char* parlcd_mem_base - to pass in render method
        */
        void score_fill_array(uint16_t* fb, unsigned char* parlcd_mem_base);

        /*
        resets values of score to play another game
        */
        void reset();
        
        /*
        Getters
        @return
        	Score of first snake
        	Score of second snake
        	Color of first snake
        	Color of second snake
        */
        int get_score_1()
        {
        	return score_1;
    	}
        int get_score_2()
        {
        	return score_2;
    	}
        uint16_t get_color_1()
        {
        	return color_1;
    	}
        uint16_t get_color_2()
        {
        	return color_2;
    	}
};
#endif
