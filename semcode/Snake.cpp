
#include <stdlib.h>
#include <iostream>

#include "Snake_Tile.cpp"

using namespace std;

class Snake{
    int size;
    Snake_Tile* snake_tiles;
    Snake_Tile* head;
    Snake_Tile* tail;
    public:
        Snake(int, int);
        Snake_Tile* get_snake_tiles(){return snake_tiles;}
        int get_size(){return size;}
        void add_snake_tile();
        void delete_snake_tiles();
        void render();
        void update();
};

Snake::Snake(int x, int y){
    snake_tiles = (Snake_Tile*) malloc(sizeof(Snake_Tile)*10);
    if (snake_tiles == NULL)
    {
        fprintf(stderr,"Cannot malloc snake tiles!\n");
        exit(1);
    }
    snake_tiles[0] = Snake_Tile(x,y);
    head = &snake_tiles[0];
}