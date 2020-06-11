
#include "Render.h"

#define BASE_COLOR_S BASE_COLOR

//Rectangles
#define BORDER_GO 5
#define WIDTH_GO LCD_WIDTH
#define HEIGHT_GO 100
#define STARTY_GO 120
#define STARTX_GO 0

//Text
#define STARTX_1_GO LCD_WIDTH/8-40
#define STARTY_1_GO 35
#define STARTY_2_GO 60

/*
game_over_update checks keypressed

@params 
    char keypressed to pass pressed key
@returns
    it returns 1 if pressed key is F or X otherwise 0
*/
int game_over_update(char keypressed);



/*
game_over_fill_array renders game over window,
    Score of each snake is rendered with his color in the widnow.

@params 
    uint16_t* fb - to write into it in a render method
    unsigned char * parlcd_mem_base - to pass in render method
    int score_1 - score of snake 1
    int score_2 - score of snake 2
    uint16_t color_1 - color of snake 1
    uint16_t color_2 - color of snake 2
*/
void game_over_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base, int score_1, int score_2, uint16_t color_1, uint16_t color_2);

