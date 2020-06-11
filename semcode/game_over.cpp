#include "game_over.h"

/*
    All described in game_over.h
*/

int game_over_update(char keypressed){
    int return_value = 0;
    if(keypressed == 'x'){
        return_value = 1; 
    }
    if(keypressed == 'f'){
        return_value = 1; 
    }
    return return_value;
}

void game_over_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base, int score_1, int score_2, uint16_t color_1, uint16_t color_2){
    //Game over
    int xTile = STARTX_1_GO, yTile = 5;
    char str[]="GAME OVER";
    int size_of_str = 0;
    while (str[size_of_str] != 0){size_of_str++;}
    char *begin = str;
    draw_rect(STARTX_GO, 0, WIDTH_GO, HEIGHT_GO, 3*BORDER_GO, BASE_COLOR, fb);
    draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);

    //snake 1
    xTile = STARTX_1_GO; yTile = STARTY_1_GO;
    char str3[]="SNAKE: ";
    size_of_str = 0;
    while (str3[size_of_str] != 0){size_of_str++;}
    begin = str3;
    draw_filled_rect(STARTX_GO+25, STARTY_GO+25, 50, 50, color_1, fb);
    draw_string(xTile, yTile, color_1, begin, size_of_str, fb);
     //his score
    xTile = STARTX_1_GO+50; yTile = STARTY_1_GO;
    char str4[10];
    sprintf(str4, "%d", score_1);
    size_of_str = 0;
    while (str4[size_of_str] != 0){size_of_str++;}
    begin = str4;
    draw_string(xTile, yTile, color_1, begin, size_of_str, fb);

    //snake 2
    xTile = STARTX_1_GO; yTile = STARTY_2_GO;
    char str5[]="SNAKE: ";
    size_of_str = 0;
    while (str5[size_of_str] != 0){size_of_str++;}
    begin = str5;
    draw_filled_rect(STARTX_GO+25, STARTY_GO+25+100, 50, 50, color_2, fb);
    draw_string(xTile, yTile, color_2, begin, size_of_str, fb);
    //his score
    xTile = STARTX_1_GO+50; yTile = STARTY_2_GO;
    char str6[10];
    sprintf(str6, "%d", score_2);
    size_of_str = 0;
    while (str6[size_of_str] != 0){size_of_str++;}
    begin = str6;
    draw_string(xTile, yTile, color_2, begin, size_of_str, fb);
}