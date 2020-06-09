#ifndef R_H
#define R_H

#include <stdlib.h>
#include <stdio.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#define LCD_WIDTH			480
#define LCD_HEIGHT			320
#define GAME_WIDTH			320
#define GAME_HEIGHT			320


#define SIZE_OF_SQUARE		32
#define SIZE_OF_PIXEL		4


#define BRIGHT_RED 			0xf800
#define BRIGHT_GREEN 		0x07e0
#define BRIGHT_BLUE 		0x001f

#define DARK_RED 			0x7800
#define DARK_GREEN	 		0x01e0
#define DARK_BLUE 			0x000f

#define BRIGHT_WHITE		0xffff
#define DARK_WHITE			0x5555
#define BLACK 				0x0000

#define PURPLE				0x780f
#define PINK                0xf81f
#define CYAN                0x07ff
#define YELLOW  			0xffe0
#define ORANGE              0xfd20


#define LED_LINE_0			0x00000000
#define LED_LINE_1			0xff000000
#define LED_LINE_2			0xffff0000
#define LED_LINE_3			0xffffff00
#define LED_LINE_4			0xffffffff


#define LED_RED				0xff0000
#define LED_GREEN			0x00ff00
#define LED_BLUE			0x0000ff
#define LED_BLACK			0x000000


int char_width(font_descriptor_t* fdes, int ch);

void draw_pixel(int xTile, int yTile, uint16_t colour, uint16_t *pixels);

void draw_char(int xTile, int yTile, font_descriptor_t* fdes, int ch, int back, uint16_t colour, uint16_t *pixels);

void draw_string(int xTile, int yTile, uint16_t colour, char *ch, int size_of_string, uint16_t *pixels);

void draw_rect(int x, int y, int width, int height, int border, uint16_t color, uint16_t *fb);

void draw_filled_rect(int x, int y, int width, int height, uint16_t color, uint16_t *fb);

void fill_unit(int xTile, int yTile, uint16_t colour, uint16_t* pixels);

void clear_array(uint16_t colour, uint16_t* pixels);

void draw(unsigned char *parlcd_mem_base, uint16_t* pixels);

void led_line(uint32_t val_line, unsigned char *mem_base);

void led_RGB1(uint32_t colour, unsigned char *mem_base);

void led_RGB2(uint32_t colour, unsigned char *mem_base);

#endif