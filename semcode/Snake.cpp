#ifndef S_C
#define S_C

#include "Snake.h"


Snake::Snake(int x, int y, uint16_t new_color, Game_Properties new_gm){
    printf("Snake: initialized.\n");
    gm = new_gm;
    color = new_color;
    health = 1;
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
    // moves[0].move_dir = Snake_Tile::UP;
    // moves[0].move_x = 0;
    // moves[0].move_y = -1;

    // moves[1].move_dir = Snake_Tile::RIGHT;
    // moves[1].move_x = 1;
    // moves[1].move_y = 0;

    // moves[2].move_dir = Snake_Tile::DOWN;
    // moves[2].move_x = 0;
    // moves[2].move_y = 1;
    
    // moves[3].move_dir = Snake_Tile::LEFT;
    // moves[3].move_x = -1;
    // moves[3].move_y = 0;
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
    // if (size >= max_size-1){
    //     printf("RESIZE**********************\n");
    //     max_size*=2;
    // }
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

void Snake::set_velocity_to_food(){
    float diff_x = (float)((snake_tiles[0].get_x()) - food_x);
    float diff_y = (float)((snake_tiles[0].get_y()) - food_y);
    float distance = (float) sqrt(pow(diff_x,2)+pow(diff_y,2));
    velocity_x = round((-1/distance)*diff_x);
    velocity_y = round((-1/distance)*diff_y);
}


bool Snake::is_possible_die_by_eating_myself(int move_x, int move_y){
    bool ret = false;
    Snake_Tile my_head = get_tile(0);
    my_head.set_x(my_head.get_x()+move_x);
    my_head.set_y(my_head.get_y()+move_y);
    for (size_t i = 3; i < size-1; i++)
    {
        //printf("CHECKING VALUES %d %d\n", my_head.get_x(), my_head.get_y());
        Snake_Tile tmp_tile = get_tile(i);
        Snake_Tile::Direction tmp_dir = tmp_tile.get_direction();
        printf("PREV DIR %s\n", tmp_tile.print_direction());
        printf("PREV%zu: x%d y%d\n", i, tmp_tile.get_x(), tmp_tile.get_y());
        Snake_Tile checked_tile = get_tile(i+1);
       // printf("CHECK %zu BEFORE increase: x%d y%d\n", i+1, checked_tile.get_x(), checked_tile.get_y());
        checked_tile.set_x(moves[tmp_dir].move_x + checked_tile.get_x());
        checked_tile.set_y(moves[tmp_dir].move_y + checked_tile.get_y());
      //  printf("CHECK %zu After increase: x%d y%d\n", i+1, checked_tile.get_x(), checked_tile.get_y());
        if (checked_tile.get_y()==my_head.get_y() || checked_tile.get_x()==my_head.get_x()){
            
            
            //Snake_Tile::Direction checked_dir = checked_tile.get_direction();
          //  printf("COULD DIE BY MYSELF\n");
            //just for the tiles that will be infront of head
            //moves[tmp_dir].reward -=0.2;
            //printf("*SET dir %s\n -0.2r", tmp_tile.print_direction());
            return true;
        }
    }
    return ret;
}

bool Snake::is_possible_die(int move_x, int move_y){
    bool ret = false;
    Snake_Tile my_head = get_tile(0);
    my_head.set_x(my_head.get_x()+move_x);
    my_head.set_y(my_head.get_y()+move_y);
    if (my_head.get_x() < 0 || my_head.get_x() >= gm.game_width/gm.size_of_tile){
        printf("Could die by X %d %d, color: %x\n",my_head.get_x(), my_head.get_y(),color);
        return true;
    }
    if (my_head.get_y()<0 || my_head.get_y() >= gm.game_height/gm.size_of_tile){
        printf("Could die by Y %d %d, color: %x\n",my_head.get_x(), my_head.get_y(),color);
        return true;
    }
    if(opponent_snake!=NULL){
        for (size_t i = 1; i < opponent_snake->get_size(); i++)
        {
            Snake_Tile prev = opponent_snake->get_tile(i-1);
            Snake_Tile test = opponent_snake->get_tile(i);
            test.set_x(prev.get_x());
            test.set_y(prev.get_y());
            printf("OPPONENT SIZE: %d\n", opponent_snake->get_size());
            if (test.get_y()==my_head.get_y() || test.get_x()==my_head.get_x()){
                printf("Could die by Other_one %d %d, color: %x\n",my_head.get_x(), my_head.get_y(),color);
                return true;
            }
        }
        Snake_Tile opp_head = opponent_snake->get_tile(0);
        if (opp_head.get_x() == my_head.get_x() && opp_head.get_y() == my_head.get_y()){
            return true;
        }
        
    }
    return ret;
}

bool Snake::is_possible_eat_food(int move_x, int move_y){
    Snake_Tile my_head = get_tile(0);
    my_head.set_x(my_head.get_x()+move_x);
    my_head.set_y(my_head.get_y()+move_y);
    return my_head.get_x()==food_x && my_head.get_y() == food_y;
}

float Snake::calculate_distance_to_food(){
    float diff_x = food_x - (float)((snake_tiles[0].get_x()));
    float diff_y = food_y - (float)((snake_tiles[0].get_y()));
    float distance = (float) sqrt(pow(diff_x,2)+pow(diff_y,2));
    return distance;
}

float Snake::is_getting_closer_to_food(int move_x, int move_y){
    Snake_Tile my_head = get_tile(0);
    my_head.set_x(my_head.get_x()+move_x);
    my_head.set_y(my_head.get_y()+move_y);
    float new_diff_x = food_x - (float)((my_head.get_x()));
    float new_diff_y = food_y - (float)((my_head.get_y()));
    float new_distance = (float) sqrt(pow(new_diff_x,2)+pow(new_diff_y,2));
    return new_distance;
}

bool Snake::is_possible_get_into_circle(int move_x, int move_y){
    // map<Snake_Tile::Direction, Move>::iterator it2;
    // for(it2 = moves.begin(); it2!= moves.end(); it2++){
    // }
    return true;
}

Snake_Tile::Direction  Snake::choose_move(){
    moves[Snake_Tile::UP].reward =1;
    moves[Snake_Tile::RIGHT].reward =1;
    moves[Snake_Tile::DOWN].reward =1;
    moves[Snake_Tile::LEFT].reward =1;
    float max_reward = 0;
    Snake_Tile::Direction direction_of_max_reward = Snake_Tile::NONE;
    map<Snake_Tile::Direction, Move>::iterator it;
    for(it = moves.begin(); it!= moves.end(); it++){
        if (!is_new_direction_correct(it->second.move_dir, dir)){
            it->second.reward -=0.9;
        }else if (die(it->second.move_x, it->second.move_y)){
            it->second.reward -=0.75;
        }else if (is_possible_die(it->second.move_x, it->second.move_y)){
            it->second.reward -=0.8;
        }else if(is_possible_eat_food(it->second.move_x, it->second.move_y)){
            it->second.reward += 0.1;
        }else{
            if(is_getting_closer_to_food(it->second.move_x, it->second.move_y)<calculate_distance_to_food()){
                printf("GETTING CLOSER\n");
                it->second.reward -=0.1;
            }else{
                printf("NO GETTING CLOSER\n");
                it->second.reward -=0.2;
            }
        }
        if(direction_of_max_reward == Snake_Tile::NONE){
            direction_of_max_reward = it->first;
        }
        if(max_reward <= it->second.reward){
            if(max_reward == it->second.reward){
                int a = rand() % 100;
                if(a <50){
                    max_reward = it->second.reward;
                    direction_of_max_reward = it->first;
                }else{
                    max_reward = max_reward;
                    direction_of_max_reward = direction_of_max_reward;
                }   
            }else{
                max_reward = it->second.reward;
                direction_of_max_reward = it->first;
            }
            Snake_Tile toprint;
            toprint.set_direction(direction_of_max_reward);
            printf("CHOSEN: %f = dir:%s\n",max_reward, toprint.print_direction());
        }
        Snake_Tile toprint;
        toprint.set_direction(it->first);
        printf("REWARD: %f dir:%s\n", it->second.reward, toprint.print_direction());
    }
    if(max_reward<=0){
        printf("ERROR: **********************************************************\n");
    }
    Snake_Tile toprint;
    toprint.set_direction(direction_of_max_reward);
    printf("CHOSEN: %f = dir:%s\n",max_reward, toprint.print_direction());
    return direction_of_max_reward;
    // for (size_t i = 0; i < 4; i++)
    // {
    //     moves[i].reward = 3;
    //     if (!is_new_direction_correct(moves[i].move_dir, dir)){
    //         moves[i].reward = -2;
    //         continue;
    //     }
    //     if (is_possible_die(moves[i].move_x, moves[i].move_y)){
    //         moves[i].reward = -1;
    //         continue;
    //     }


    // }
    
    // set_velocity_to_food();
    // Snake_Tile::Direction new_dir = set_direction();
    // if (is_new_direction_correct(new_dir, dir)){
    //     // if(!can_die()){

    //     // }
    // }
}

bool Snake::die(int move_x, int move_y){
    Snake_Tile my_head = get_tile(0);
    my_head.set_x(my_head.get_x()+move_x);
    my_head.set_y(my_head.get_y()+move_y);
    for (size_t i = 2; i < size; i++){
        Snake_Tile prev = get_tile(i-1);
        Snake_Tile test = get_tile(i);
        test.set_x(prev.get_x());
        test.set_y(prev.get_y());
        if(my_head.get_y() == test.get_y() && my_head.get_x() == test.get_x()){
            printf("Could die by himself %d %d, color: %x\n",my_head.get_x(), my_head.get_y(),color);
            return true;
        }
    }
    return false;
}

Snake_Tile::Direction Snake::choose_move2(){
    Snake_Tile::Direction selected_dir = Snake_Tile::NONE;
    float control = 10000;
    for (int dirInt = Snake_Tile::UP; dirInt!=Snake_Tile::NONE; dirInt++){
        Snake_Tile::Direction new_dir = static_cast<Snake_Tile::Direction> (dirInt);
        if (is_new_direction_correct(new_dir,dir))
        {
            Snake_Tile test;
            test.set_direction(new_dir);
            printf("DIRECTION: %d %d\n", moves[new_dir].move_x, moves[new_dir].move_y);
            printf("%s\n", test.print_direction());
            if(!die(moves[new_dir].move_x, moves[new_dir].move_y)){
                if(!is_possible_die(moves[new_dir].move_x, moves[new_dir].move_y)){
                    float distance = is_getting_closer_to_food(moves[new_dir].move_x, moves[new_dir].move_y);
                    if (selected_dir == Snake_Tile::NONE){
                        selected_dir = new_dir;
                    }
                    if(distance < control){
                        control = distance;
                        selected_dir = new_dir;
                    }
                }else{
                    printf("COULD DIE BY OTHER\n");
                }
            }else{
                printf("COULD DIE\n");
            }
        }
    }
    if (selected_dir == Snake_Tile::NONE){
        selected_dir = Snake_Tile::LEFT;
        printf("SET BASE\n");
    }
    return selected_dir;
}

void Snake::move_tiles(){
    if (size>0){
        Snake_Tile tmp1 = snake_tiles[0];
        //printf("HEAD: %s ", tmp1.print_direction());
        Snake_Tile tmp2;
        for (size_t i = 1; i <= size; i++)
        {  
            tmp2 = snake_tiles[i];
            snake_tiles[i] = tmp1;
            tmp1 = tmp2;
            //printf("t:%zu: dir: %s x:%d y:%d",i, snake_tiles[i].print_direction(), snake_tiles[i].get_x(), snake_tiles[i].get_y());
            if (i==size){
                tail = snake_tiles[i];
            }
        }
       // printf("\n");
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


void Snake::update(){
    if(size>0){
        printf("MOVE************************\n");
        printf("SIZE: %zu\n", size);
        Snake_Tile::Direction new_reward_dir = choose_move2();
        if(!is_new_direction_correct(new_reward_dir, dir)){
            printf("THAT IS NOT CORRECT! UUUUUUUUUUUUUUUUUUUUUUUUUUUUU\n");
        }
        velocity_x = moves[new_reward_dir].move_x;
        velocity_y = moves[new_reward_dir].move_y;
        dir = set_direction();
        move_tiles();
        snake_tiles[0].set_direction(new_reward_dir);
        snake_tiles[0].set_y(snake_tiles[0].get_y()+velocity_y);
        snake_tiles[0].set_x(snake_tiles[0].get_x()+velocity_x);
    
    
        // choose_move();
        // set_velocity_to_food();
        // if (velocity_x==0)
        // {   
        //     dir = set_direction();
        //     move_tiles();
            
        //     snake_tiles[0].set_direction(dir);
        //     snake_tiles[0].set_y(snake_tiles[0].get_y()+velocity_y);
        // }else{
        //     dir = set_direction();
        //     move_tiles();
        //     snake_tiles[0].set_direction(dir);
        //     snake_tiles[0].set_x(snake_tiles[0].get_x()+velocity_x);
        // }
        printf(" HEAD: %s %d %d\n", snake_tiles[0].print_direction(),snake_tiles[0].get_x(), snake_tiles[0].get_y());
        printf("FOOD X:%d Y:%d\n", food_x, food_y);
        printf("END MOVE************************\n");
    }else{
        fprintf(stderr, "Cannot do Update, because size of snake is 0.\n");
    }
}
#endif

