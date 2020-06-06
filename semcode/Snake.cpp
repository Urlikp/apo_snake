
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>


#include "Snake_Tile.cpp"

#define DEFAULT_SIZE 100

#define DELETE_COLOR 0x0000

using namespace std;

class Snake{
    protected:
        enum Direction{UP, LEFT, DOWN, RIGHT};
        Direction dir;
        size_t size = 0;
        size_t max_size;
        Snake_Tile tail;
        int tile_size;
        int velocity_x, velocity_y;
        int color;
        std::vector<Snake_Tile> snake_tiles;
    protected:
        void set_direction();
        void move_tiles();

    public:
        Snake(int, int, int, int);
        std::vector<Snake_Tile> get_snake_tiles(){return snake_tiles;}
        int get_size(){return size;}
        Snake_Tile get_tile(int pos);
        void add_snake_tile();
        void delete_snake_tiles();
        void fill_array(uint16_t*, int);
        void update(int, int);
};

Snake::Snake(int x, int y, int new_color, int new_tile_size){
    color = new_color;
    tile_size = new_tile_size;
    snake_tiles.reserve(DEFAULT_SIZE);
    snake_tiles.emplace_back(x,y);
    max_size = DEFAULT_SIZE;
    size++;
}

Snake_Tile Snake::get_tile(int pos){
    if (pos>0 && pos < size)
    {
        return snake_tiles[pos];
    }else{
        return Snake_Tile(-1,-1); //like a null
    }
    
}

void Snake::add_snake_tile()
{
    if (size == max_size-1){
        snake_tiles.resize(max_size*2);
        max_size*=2;
    }
    snake_tiles.emplace_back();
    size++;
}

void Snake::delete_snake_tiles(){
    snake_tiles.clear();
    size = 0;
}

void Snake::set_direction(){
    if (velocity_x<-1){
        dir = LEFT;
    }else if(velocity_x>0){
        dir = RIGHT;
    }else if(velocity_y<0){
        dir = UP;
    }else if(velocity_y>0){
        dir = DOWN;
    }
}

void Snake::move_tiles(){
    if (size>0){
        Snake_Tile tmp1 = snake_tiles[0];
        Snake_Tile tmp2;
        for (size_t i = 1; i <= size; i++)
        {  
            tmp2 = snake_tiles[i];
            snake_tiles[i] = tmp1;
            tmp1 = tmp2;
            if (i==size){
                tail = snake_tiles[i];
            }
        }
    }
}



void Snake::fill_array(uint16_t *fb,  int LCD_width){
    for (size_t i = 0; i <= size; i++)
    {
        if (i < size){
            Rectangle *rect = snake_tiles[i].get_occupied_field(tile_size, tile_size);

            for (int y = rect->y*rect->h; y < rect->y*rect->h + rect->h; y++)
            {
                for (int x  = rect->x*rect->w; x < rect->x*rect->w+rect->w; x++)
                {
                    fb[LCD_width * y + x] = color;
                }
            }
        }else{

        Rectangle *rect = tail.get_occupied_field(tile_size, tile_size);

            for (int y = rect->y*rect->h; y < rect->y*rect->h + rect->h; y++)
            {
                for (int x  = rect->x*rect->w; x < rect->x*rect->w+rect->w; x++)
                {
                    fb[LCD_width * y + x] = DELETE_COLOR;
                }
            }
        }
    }
}


void Snake::update(int food_x, int food_y){
    if(size>0){
        float diff_x = (float)((snake_tiles[0].get_x()) - food_x);
        float diff_y = (float)((snake_tiles[0].get_y()) - food_y);
        float distance = (float) sqrt(pow(diff_x,2)+pow(diff_y,2));
        velocity_x = round((-1/distance)*diff_x);
        velocity_y = round((-1/distance)*diff_y);
        if (velocity_x==0)
        {
            move_tiles();
            snake_tiles[0].set_y(snake_tiles[0].get_y()+velocity_y);
        }else{
            move_tiles();
            snake_tiles[0].set_x(snake_tiles[0].get_x()+velocity_x);
        }
        set_direction();
    }else{
        fprintf(stderr, "Cannot do Update, because size of snake is 0.\n");
    }
}


