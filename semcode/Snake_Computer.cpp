
#include "Snake.cpp"
#include "Snake_Handler.cpp"


class Snake_Computer: public Snake{
    bool is_food_known = false;
    private:
        int counter;
        void set_direction_to_food();
    public:
        //Snake_Computer(int x, int y, int new_color, int new_tile_size) : Snake(x, y, new_color, new_tile_size){      
       // }
};