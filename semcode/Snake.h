#ifndef A
#define A 

#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <iterator>
#include <iostream>

#define DEFAULT_SIZE 2

#define DELETE_COLOR 0x0000


using namespace std;

#include "Snake_Tile.h"
#include "Game_Properites.h"

/*
    This class represents snake
*/
class Snake{
    protected:
    //PROTECTED VARIABLES
        typedef struct Move
        {
            Snake_Tile::Direction move_dir;
            int move_x;
            int move_y;
            float reward;
        }Move;

        std::map<Snake_Tile::Direction, Move> moves;

        //Move moves[4]; //0=UP 1=RIGHT 2=DOWN 3=LEFT
        
        Snake_Tile::Direction dir;
        Snake* opponent_snake = NULL;
        Game_Properties gm;
        
        /*Snake properties*/
        std::vector<Snake_Tile> snake_tiles;
        size_t size = 0;
        size_t max_size;
        Snake_Tile tail;
        int color;
        int tile_size;
        int health;

        /*variables used in update*/
        int food_x, food_y;
        int velocity_x, velocity_y;

    //PROTECTED FUNCTIONS
        bool is_new_direction_correct(Snake_Tile::Direction new_dir, Snake_Tile::Direction prev_dir);
        Snake_Tile::Direction set_direction();
        void move_tiles();
        void set_velocity_to_food();
        Snake_Tile::Direction  choose_move();
        Snake_Tile::Direction choose_move2();
        bool is_possible_die(int move_x, int move_y);
        bool is_possible_die_by_eating_myself(int move_x, int move_y);
        bool is_possible_eat_food(int move_x, int move_y);
        bool is_possible_get_into_circle(int move_x, int move_y);
        float is_getting_closer_to_food(int move_x, int move_y);
        bool die(int move_x, int move_y);
        void is_possible_move();
        float calculate_distance_to_food();
        void set_moves();

    public:
    //PUBLIC FUNCITONS

        /*
            Constructor of snake
            @param 
            x, y means start positions (x and y coordinates in tiles)
            new_color is a Hex number of color which snake uses in fill_array 
            gm struct that holds properties of game
        */
        Snake(int x, int y, int new_color, Game_Properties gp);
        
        /*
            Set address of an opponent snake
            @param 
            Snake the opponent snake
        */
        void set_opposite_snake(Snake *op_snake);
        
        /*
            Fill the buffer of display with color on positions where snake occurs
            @param 
            uint16_t* is an one dimensional array of display
            int must be lcd width
        */
        void fill_array(uint16_t*, int);
        
        /*
            Update states of snake
        */
        void update();

        /*
            Get snake body
            @return 
            snake_tiles a vector whose values are Snake Tiles
        */
        std::vector<Snake_Tile> get_snake_tiles(){return snake_tiles;}

        /*
            Get tile of int position that is passed
            @param 
            int position of snake body that will be returned
            @return
            If position is in range of snake size it returns Snake_Tile of position
            otherwise Snake_Tile(-1,-1) it means NULL
        */
        Snake_Tile get_tile(int);

        /*
            Increase snake body, called when snake ate a food.
        */
        void add_snake_tile();

        /*
            Get size of snake it is also a Score of the snake
            @return
            Actual size of snake
        */
        int get_size(){return size;}

        /*
            Set snake's food_x and food_y to know where food is (used AI of snake)
            @param
            first int is a X coord position in tiles of food
            second int is a Y coord position in tiles of food 
        */
        void set_position_of_food(int, int);

        /*
            Delete snake body;
        */
        void delete_snake_tiles();
};
#endif