/*******************************************************************
  Simple program to implement Snake video game on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  Snake.h - class that implements snake

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/
 
#ifndef A
#define A 

#include <math.h>
#include <vector>
#include <map>

#define DEFAULT_SIZE 2

#define DELETE_COLOR 0x0000

#include "Snake_Tile.h"
#include "Game_Properties.h"

/*
This class represents snake
*/
class Snake
{
    protected:
    //PROTECTED VARIABLES
        bool is_player_playing;
        typedef struct Move
        {
            Snake_Tile::Direction move_dir;
            int move_x;
            int move_y;
            float reward;
        }Move;

        std::map<Snake_Tile::Direction, Move> moves;
        Snake_Tile::Direction dir;

        Snake* opponent_snake = NULL;
        Game_Properties gm;
        
        /*
        Snake properties
        */
        std::vector<Snake_Tile> snake_tiles;
        size_t size = 0;
        size_t max_size;
        Snake_Tile tail;
        uint16_t color;
        int tile_size;
        int health;

        /*
        variables used in update
        */
        int food_x, food_y;
        int velocity_x, velocity_y;

    //PROTECTED FUNCTIONS
        /*
        Check if new direction is legal, only 90 or 0 degree turns are legal.
        @param
        	Direction new_dir - new direction of snake
        	Direction prev_dir - current direction of snake
        @return 
        	If new direction is correct than true otherwise false
        */
        bool is_new_direction_correct(Snake_Tile::Direction new_dir, 
        							Snake_Tile::Direction prev_dir);
        /*
        Returns direction that corresponds to coord change
        @return
        	direction based on coord change
        */
        Snake_Tile::Direction set_direction();
        
        /*
        it moves the coords of each tile of snake
        */
        void move_tiles();

        /*
        function with simple AI that chooses moves which brings the snake closer 
        	to food
        @return
        	which direction is best for the snake	
        */
        Snake_Tile::Direction choose_move2();
        
        /*
        to set dictionary with moves
        */
        void set_moves();

        /*
        to check if tile is occupied with coord X and Y
        @param
        	int x - x coord of tile
        	int y - y coord of tile
        @return 
        	if there is snake on tile than true, otherwise false
        */
        bool is_tile_occupied(int x, int y);

    public:
    //PUBLIC FUNCITONS
        /*
        Constructor of snake
        @param 
            x, y means start positions (x and y coordinates in tiles)
            new_color is a Hex number of color which snake uses in fill_array 
            gm struct that holds properties of game
        */
        Snake(int x, int y, uint16_t new_color, Game_Properties gp, 
        	bool is_player);
        
        /*
        Set address of an opponent snake
        @param 
            Snake* op_snake - the opponent snake
        */
        void set_opposite_snake(Snake* op_snake);
        
        /*
        Fill the buffer of display with color on positions where snake occurs
        @param 
            uint16_t* fb - one dimensional array of display
            int LCD_width - lcd width
        */
        void fill_array(uint16_t* fb, int LCD_width);
        
        /*
        Update state of snake
        */
        void update();

        /*
        Get snake body
        @return 
            snake_tiles a vector whose values are Snake Tiles
        */
        std::vector<Snake_Tile> get_snake_tiles()
        {
        	return snake_tiles;
    	}

        /*
        Get tile of int position that is passed
        @param 
            int pos - position of snake body that will be returned
        @return
            If position is in range of snake size it returns Snake_Tile on 
            position pos otherwise Snake_Tile(-1, -1) which represents NULL
        */
        Snake_Tile get_tile(int pos);

        /*
        Increase snake body, called when snake ate a food.
        */
        void add_snake_tile();

        /*
        Get size of snake it is also a Score of the snake
        @return
            Actual size of snake
        */
        size_t get_size()
        {
        	return size;
    	}

        /*
        Set snake's food_x and food_y variables to where the food is
        @param
            int new_food_x - X coord position in tiles of food
            int new_food_y - Y coord position in tiles of food 
        */
        void set_position_of_food(int new_food_x, int new_food_y);

        /*
        Delete snake body;
        */
        void delete_snake_tiles();



        /*
        Sets new direction for the snake
        @param
        	Direction dir - new direction
        */
        void set_direction(Snake_Tile::Direction dir);


        /*
        Get color of snake;
        @return
        	color of snake
        */
        uint16_t get_color()
        {
            return color;
        }
};
#endif
