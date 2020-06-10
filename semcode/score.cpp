#include "score.h"

/*

All described in file score.h

*/

void Score::update(Snake_Handler handler){
    if(handler.size>1){
        color_2 = handler.snakes[1]->get_color();
        score_2 = handler.snakes[1]->get_size();
    }
    score_1 = handler.snakes[0]->get_size();
    color_1 = handler.snakes[0]->get_color();

}
void Score::score_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base){
    
    int xOffset = GAME_WIDTH;
    uint16_t color = DARK_WHITE;
    draw_rect(0,0, LCD_WIDTH, GAME_HEIGHT, 2,color,fb);
    draw_rect(xOffset,0, LCD_WIDTH-GAME_WIDTH, GAME_HEIGHT, 5,color,fb);
    draw_rect(xOffset,0, LCD_WIDTH-GAME_WIDTH, GAME_HEIGHT/3,5,color,fb);
    draw_rect(xOffset,0, LCD_WIDTH-GAME_WIDTH, 2*(GAME_HEIGHT/3),5,color,fb);
    
    
    int xTile = xOffset/4 + LCD_WIDTH/64, yTile = GAME_HEIGHT/64;
    char str[10];  //size is enough to render an integer of max 3 digits
    sprintf(str, "%d", score_1);
    int size_of_str = 0;
    while (str[size_of_str] != 0){size_of_str++;}
    char *begin = str;
    draw_string(xTile, yTile, color_1, begin, size_of_str, fb);

    clock_gettime(CLOCK_MONOTONIC, &end_clock);
	int time = (end_clock.tv_sec - begin_clock.tv_sec);
    xTile = xOffset/4 + LCD_WIDTH/64; 
    yTile = GAME_HEIGHT/12 + GAME_HEIGHT/64;
    char str2[10];  //size is enough to render an integer of max 3 digits
    sprintf(str2, "%d", time);
    size_of_str = 0;
    while (str2[size_of_str] != 0){size_of_str++;}
    begin = str2;
    draw_string(xTile, yTile, BASE_COLOR_S, begin, size_of_str, fb);
    
    xTile = xOffset/4 + LCD_WIDTH/64; 
    yTile = 2*(GAME_HEIGHT/12) + GAME_HEIGHT/64;
    char str3[10];  //size is enough to render an integer of max 3 digits
    sprintf(str3, "%d", score_2);
    size_of_str = 0;
    while (str3[size_of_str] != 0){size_of_str++;}
    begin = str3;
    draw_string(xTile, yTile, color_2, begin, size_of_str, fb);
}

void Score::reset(){
    score_1 = 1;
    score_2 = 1;
    clock_gettime(CLOCK_MONOTONIC, &begin_clock);
}
