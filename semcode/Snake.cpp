
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>


#include "Snake_Tile.cpp"

#define DEFAULT_SIZE 100

#define DELETE_COLOR 0x0000

using namespace std;

class Snake{
    size_t size = 0;
    size_t max_size;
    Snake_Tile tail;
    Snake_Tile tail2;
    int tile_size;
    int delete_coords[2];
    int color;
    bool wasTileAdded = false;
    bool wasMove = false;

    //Snake_Tile snake_tiles[DEFAULT_SIZE];
    std::vector<Snake_Tile> snake_tiles;
    public:
        Snake(int, int, int, int);
        std::vector<Snake_Tile> get_snake_tiles(){return snake_tiles;}
        int get_size(){return size;}
        void add_snake_tile();
        void delete_snake_tiles();
        void fill_array(uint16_t*, int);
        void clear_array(uint16_t*, int, int);
        void update(int, int);
};

Snake::Snake(int x, int y, int new_color, int new_tile_size){
    color = new_color;
    tile_size = new_tile_size;
    snake_tiles.reserve(DEFAULT_SIZE);
    snake_tiles.emplace_back(x,y);
    max_size = DEFAULT_SIZE;
    tail = snake_tiles[0];
    size++;
}

void Snake::add_snake_tile()
{
    if (size == max_size-1){
        snake_tiles.resize(max_size*2);
        max_size*=2;
    }
    snake_tiles.emplace_back();
    printf("SIZE: %d \n",size);
    printf("SIZE2 %d\n", snake_tiles.size());
    size++;
    printf("SIZE: %d \n",size);
    wasTileAdded = true;
}

void Snake::delete_snake_tiles(){
    snake_tiles.clear();
}

void Snake::fill_array(uint16_t *fb,  int LCD_width){
    for (size_t i = 0; i <= size; i++)
    {
        printf("clanek: %d, x:%d y:%d ", i, (int)snake_tiles[i].get_x(), (int)snake_tiles[i].get_y());
        Rectangle *rect = snake_tiles[i].get_occupied_field(tile_size, tile_size);

        for (int y = rect->y*rect->h; y < rect->y*rect->h + rect->h; y++)
        {
            for (int x  = rect->x*rect->w; x < rect->x*rect->w+rect->w; x++)
            {
                fb[LCD_width * y + x] = color;
            }
        }
    }
        Rectangle *rect = tail2.get_occupied_field(tile_size, tile_size);

        for (int y = rect->y*rect->h; y < rect->y*rect->h + rect->h; y++)
        {
            for (int x  = rect->x*rect->w; x < rect->x*rect->w+rect->w; x++)
            {
                fb[LCD_width * y + x] = DELETE_COLOR;
            }
        }
}


void Snake::update(int food_x, int food_y){
     printf("************************\n");
	float diff_x = (float)((snake_tiles[0].get_x()) - food_x);
	float diff_y = (float)((snake_tiles[0].get_y()) - food_y);
	float distance = (float) sqrt(pow(diff_x,2)+pow(diff_y,2));
	int velocity_x = round((-1/distance)*diff_x);
	int velocity_y = round((-1/distance)*diff_y);
	if (velocity_x==0)
	{
        Snake_Tile tmp1 = snake_tiles[0];
        Snake_Tile tmp2;
        for (size_t i = 1; i <= size; i++)
        {  
            tmp2 = snake_tiles[i];
            snake_tiles[i] = tmp1;
            tmp1 = tmp2;
            if (i==size){
                tail2 = snake_tiles[i];
            }
        }
        snake_tiles[0].set_y(snake_tiles[0].get_y()+velocity_y);
	}else{
        Snake_Tile tmp1 = snake_tiles[0];
        Snake_Tile tmp2;
        for (size_t i = 1; i <= size; i++)
        {  
            tmp2 = snake_tiles[i];
            snake_tiles[i] = tmp1;
            tmp1 = tmp2;
            if (i==size){
                tail2 = snake_tiles[i];
            }
        }
        snake_tiles[0].set_x(snake_tiles[0].get_x()+velocity_x);
    }
    tail = snake_tiles[size-1];
	printf("Velocity: y:%d, x:%d\n", velocity_y, velocity_x);
    printf("HEAD: X:%d, Y:%d\n", snake_tiles[0].get_x(), snake_tiles[0].get_y());
    printf("TAIL: %d, %d\n", tail.get_x(), tail.get_y());
    for (size_t i = 0; i < size; i++)
    {
        printf("clanek: %d, x:%d y:%d ", i, (int)snake_tiles[i].get_x(), (int)snake_tiles[i].get_y());
    }
    printf("\n");
    if (snake_tiles[0].get_x()==food_x && snake_tiles[0].get_y())
    {
        //printf("INTERSECT FOOD HEAD\n");
        //add_snake_tile();
    }
    printf("************************\n");
    
}


