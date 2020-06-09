#include "Snake_Handler.cpp"
#include "Game_Properites.h"

bool collision_update(Snake_Handler handler, Game_Properties gm){
    for(size_t i = 0; i < handler.size; i++){
        Snake_Tile test_head = handler.snakes[i]->get_tile(0);
        if(test_head.get_x()<0 || test_head.get_x()>=gm.game_width/gm.size_of_tile){
            return true;
        }
        if(test_head.get_y()<0 || test_head.get_y()>=gm.game_height/gm.size_of_tile){
            return true;
        }
        Snake *test = handler.snakes[i];
        for (size_t j = 1; j < test->get_size(); j++)
        {
            if(test_head.get_x() == test->get_tile(j).get_x() && test_head.get_y() == test->get_tile(j).get_y()){
                return true;
            }
        }
        for(size_t j = 0; j < handler.size; j++){
            if(j!=i){
                Snake *test2 = handler.snakes[j];
                for (size_t j = 0; j < test2->get_size(); j++)
                {
                    if(test_head.get_x() == test2->get_tile(j).get_x() && test_head.get_y() == test2->get_tile(j).get_y()){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}