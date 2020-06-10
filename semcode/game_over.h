
#include "Render.h"

#define BASE_COLOR_S BASE_COLOR

#define BORDER_GO 5
#define WIDTH_GO LCD_WIDTH
#define HEIGHT_GO 100
#define STARTY_GO 120
#define STARTX_GO 0

#define STARTX_1_GO LCD_WIDTH/8-40
#define STARTY_1_GO 35
#define STARTY_2_GO 60


int game_over_update(char keypressed);

void game_over_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base, int score_1, int score_2, uint16_t color_1, uint16_t color_2);

