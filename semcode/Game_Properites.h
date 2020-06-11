#ifndef GP_H
#define GP_H
#include <stdint.h>
/*
    This struct is used to keep game properties.
*/
typedef struct Game_Properties{
    int game_width;
    int game_height;
    int size_of_tile;
    int LCD_width;
    int LCD_height;
    int speed;
    uint16_t color_1; //color of snake 1
    uint16_t color_2; //color of snake 2
}Game_Properties;

#endif
