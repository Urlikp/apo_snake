#ifndef S_C
#define S_C

#include "Snake.h"
/*
    All described in Snake.h
*/

Snake::Snake(int x, int y, uint16_t new_color, Game_Properties new_gm, bool is_player){
    printf("Snake: initialized.\n");
    gm = new_gm;
    color = new_color;
    health = 1;
    is_player_playing = is_player;
    tile_size = gm.size_of_tile;
    snake_tiles.resize(DEFAULT_SIZE);
    snake_tiles.emplace_back(x,y);
    snake_tiles[0].set_x(x);
    snake_tiles[0].set_y(y);
    max_size = DEFAULT_SIZE;
    set_moves();
    size++;
    printf("  Start: x=%d y=%d, Color=%x, TileSize=%d\n", snake_tiles[0].get_x(), snake_tiles[0].get_y(),color, tile_size);
}

void Snake::set_moves(){
    moves.emplace(Snake_Tile::UP, Move{Snake_Tile::UP, 0, -1, 0});
    moves.emplace(Snake_Tile::LEFT, Move{Snake_Tile::LEFT, -1, 0, 0});
    moves.emplace(Snake_Tile::DOWN, Move{Snake_Tile::DOWN, 0, 1, 0});
    moves.emplace(Snake_Tile::RIGHT, Move{Snake_Tile::RIGHT, 1, 0, 0});
}


void Snake::set_opposite_snake(Snake* op_snake){
    opponent_snake = op_snake;
}


void Snake::set_position_of_food(int new_food_x, int new_food_y){
    food_x = new_food_x;
    food_y = new_food_y;
}


Snake_Tile Snake::get_tile(int pos){
    if (pos>=0 && pos < size)
    {
        return snake_tiles[pos];
    }else{
        fprintf(stderr, "Cannot get a tile of snake: position =%d but must be in range 0 to %zu.\n", pos, size);
        return Snake_Tile(-1,-1); //like a null
    }
    
}


void Snake::add_snake_tile()
{
    snake_tiles.emplace_back(0,0);
    size++;
}


void Snake::delete_snake_tiles(){
    snake_tiles.clear();
    size = 0;
    max_size = DEFAULT_SIZE;
}


Snake_Tile::Direction Snake::set_direction(){
    Snake_Tile::Direction new_dir = dir;
    if (velocity_x<0){
        new_dir = Snake_Tile::LEFT;
    }else if(velocity_x>0){
        new_dir = Snake_Tile::RIGHT;
    }else if(velocity_y<0){
        new_dir = Snake_Tile::UP;
    }else if(velocity_y>0){
        new_dir = Snake_Tile::DOWN;
    }
    return new_dir;
}


bool Snake::is_new_direction_correct(Snake_Tile::Direction new_dir, Snake_Tile::Direction prev_dir){
    bool ret = true;
    if (new_dir == Snake_Tile::LEFT && prev_dir == Snake_Tile::RIGHT){
        ret =  false;
    }else if (new_dir == Snake_Tile::RIGHT && prev_dir == Snake_Tile::LEFT){
        ret = false;
    }else if (new_dir == Snake_Tile::UP && prev_dir == Snake_Tile::DOWN){
        ret = false;
    }else if (new_dir == Snake_Tile::DOWN && prev_dir == Snake_Tile::UP){
        ret = false;
    }
    return ret;
}


bool Snake::is_tile_occupied(int x, int y)
{
	for (size_t i = 0; i < size - 1; i++)
	{
		if (get_tile(i).get_x() == x && get_tile(i).get_y() == y)
		{
			return true;
		}
	}
	for (size_t i = 0; i < opponent_snake->size; i++)
	{
		if (opponent_snake->get_tile(i).get_x() == x && opponent_snake->get_tile(i).get_y() == y)
		{
			return true;
		}
	}
	return false;
}

Snake_Tile::Direction Snake::choose_move2(){
    Snake_Tile::Direction selected_dir = Snake_Tile::NONE;
    int closest = 69;
    Snake_Tile my_head = get_tile(0);
    
    for (int dirInt = Snake_Tile::UP; dirInt != Snake_Tile::NONE; dirInt++)
    {
        Snake_Tile::Direction new_dir = static_cast<Snake_Tile::Direction> (dirInt);
        if (is_new_direction_correct(new_dir,dir))
        {
        	int head_x = my_head.get_x(), head_y = my_head.get_y();
        	switch(new_dir)
			{
			case Snake_Tile::UP:
				(head_y)--;
				break;
			case Snake_Tile::RIGHT:
				(head_x)++;
				break;
			case Snake_Tile::DOWN:
				(head_y)++;
				break;
			case Snake_Tile::LEFT:
				(head_x)--;
				break;
			}
			if (head_x >= 0 && head_x < gm.game_width/gm.size_of_tile && head_y >= 0 && head_y < gm.game_height/gm.size_of_tile)
			{
				if (!is_tile_occupied(head_x, head_y))
				{
					int new_diff_x = food_x - head_x;
					int new_diff_y = food_y - head_y;
					float new_distance = (float) sqrt(pow(new_diff_x,2)+pow(new_diff_y,2));
					if (closest >=  new_distance)
					{
						closest = new_distance;
						selected_dir = new_dir;
					}
				}
			}
        }
    }
    if (selected_dir == Snake_Tile::NONE)
    {
        selected_dir = dir;
        printf("SET BASE\n");
    }
    return selected_dir;
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

void Snake::set_direction(Snake_Tile::Direction dir){
    snake_tiles[0].set_direction(dir);
}

void Snake::update(){
    if(size>0){
        if(is_player_playing){
            velocity_x = moves[snake_tiles[0].get_direction()].move_x;
            velocity_y = moves[snake_tiles[0].get_direction()].move_y;
            dir = set_direction();
            move_tiles();
            snake_tiles[0].set_direction(snake_tiles[0].get_direction());
            snake_tiles[0].set_y(snake_tiles[0].get_y()+velocity_y);
            snake_tiles[0].set_x(snake_tiles[0].get_x()+velocity_x);
        }else{
            printf("MOVE************************\n");
            Snake_Tile::Direction new_reward_dir = choose_move2();
            velocity_x = moves[new_reward_dir].move_x;
            velocity_y = moves[new_reward_dir].move_y;
            dir = set_direction();
            move_tiles();
            snake_tiles[0].set_direction(new_reward_dir);
            snake_tiles[0].set_y(snake_tiles[0].get_y()+velocity_y);
            snake_tiles[0].set_x(snake_tiles[0].get_x()+velocity_x);
            printf("HEAD: %s X%d Y%d S%zu\n", snake_tiles[0].print_direction(),snake_tiles[0].get_x(), snake_tiles[0].get_y(),size);
            printf("****************************\n");
        }
    }else{
        fprintf(stderr, "Cannot do Update, because size of snake is 0.\n");
    }
}
#endif

