/*******************************************************************
  Simple program to implement video game Snake on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  APO_Snake.c      - main file

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include <math.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "input.h"

#define LCD_WIDTH			480
#define LCD_HEIGHT			320
#define GAME_WIDTH			320
#define GAME_HEIGHT			320

#define WAIT_TIME			100

#define SIZE_OF_SQUARE		32

#define BRIGHT_RED 			0xf800
#define BRIGHT_GREEN 		0x07e0
#define BRIGHT_BLUE 		0x001f
#define DARK_RED 			0x7800
#define DARK_GREEN	 		0x01e0
#define DARK_BLUE 			0x000f
#define WHITE 				0xffff
#define BLACK 				0x0000

#define LED_LINE_0			0x00000000
#define LED_LINE_1			0xff000000
#define LED_LINE_2			0xffff0000
#define LED_LINE_3			0xffffff00
#define LED_LINE_4			0xffffffff

#define LED_RED				0xff0000
#define LED_GREEN			0x00ff00
#define LED_BLUE			0x0000ff
#define LED_BLACK			0x000000

uint16_t *fb;
enum direction{STOP, UP, RIGHT, DOWN, LEFT};
enum direction dir;
bool game_running;

typedef struct Tile{
	int x;
	int y;
}snake_tile;

typedef struct Snake{
	snake_tile* tiles;
	int size;
}snake;

/*void draw_pixel(int xTile, int yTile, unsigned short color) 
{
	if (xTile>=0 && xTile<480 && yTile>=0 && yTile<320) 
	{
		fb[xTile+480*yTile] = color;
	}
}

void draw_char(int xTile, int yTile, font_descriptor_t* fdes, char ch) {
}

int char_width(font_descriptor_t* fdes, int ch) 
{
	int width = 0;
	if ((ch >= fdes->firstchar) && (ch-fdes->firstchar < fdes->size)) 
	{
		ch -= fdes->firstchar;
		if (!fdes->width) 
		{
			width = fdes->maxwidth;
		} 
		else 
		{
			width = fdes->width[ch];
		}
	}
	return width;
}*/


void fill_unit(int xTile, int yTile, uint16_t colour)
{
	if (xTile >= 0 && xTile < GAME_WIDTH/SIZE_OF_SQUARE && yTile >= 0 && yTile < GAME_HEIGHT/SIZE_OF_SQUARE)
	{
		for (int i = yTile * SIZE_OF_SQUARE; i < SIZE_OF_SQUARE * yTile + SIZE_OF_SQUARE; i++)
		{
			for (int j = xTile * SIZE_OF_SQUARE; j < SIZE_OF_SQUARE * xTile + SIZE_OF_SQUARE; j++)
			{
				fb[LCD_WIDTH * i + j] = colour;
			}
		}
	}
}

void fill_unit_border(int xTile, int yTile, int borderWidth, uint16_t colour)
{
	if (xTile >= 0 && xTile < GAME_WIDTH/SIZE_OF_SQUARE && yTile >= 0 && yTile < GAME_HEIGHT/SIZE_OF_SQUARE)
	{
		for (int i = yTile * SIZE_OF_SQUARE; i < yTile * SIZE_OF_SQUARE + borderWidth; i++) {
            for (int j = xTile * SIZE_OF_SQUARE; j < xTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE; j++) {
                fb[LCD_WIDTH * i + j] = colour;
            }
        }
        for (int i = yTile * SIZE_OF_SQUARE; i < yTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE; i++) {
            for (int j = xTile * SIZE_OF_SQUARE; j < xTile * SIZE_OF_SQUARE + borderWidth; j++) {
                fb[LCD_WIDTH * i + j] = colour;
            }
        }
        for (int i = yTile * SIZE_OF_SQUARE; i < yTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE; i++) {
            for (int j = xTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE - borderWidth; j < xTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE; j++) {
                fb[LCD_WIDTH * i + j] = colour;
            }
        }
        for (int i = yTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE - borderWidth; i < yTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE; i++) {
            for (int j = xTile * SIZE_OF_SQUARE; j < xTile * SIZE_OF_SQUARE + SIZE_OF_SQUARE; j++) {
                fb[LCD_WIDTH * i + j] = colour;
            }
        }
	}
}

void move_to_food_position(int *head_x, int *head_y, int food_x, int food_y){
	fill_unit(*head_x, *head_y, BLACK);
	float diff_x = (float)((*head_x) - food_x);
	float diff_y = (float)((*head_y) - food_y);
	float distance = (float) sqrt(pow(diff_x,2)+pow(diff_y,2));
	int velocity_x = round((-1/distance)*diff_x);
	int velocity_y = round((-1/distance)*diff_y);
	(*head_x) += velocity_x;
	if (velocity_x==0)
	{
		(*head_y) += velocity_y;
	}
	fill_unit(*head_x, *head_y, WHITE);
	printf("%d, %d\n", velocity_y, velocity_x);
}

void draw(unsigned char *parlcd_mem_base)
{
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) 
	{
		parlcd_write_data(parlcd_mem_base, fb[i]);
	}
}

void led_line(uint32_t val_line, unsigned char *mem_base)
{
	*(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
}

void led_RGB1(uint32_t colour, unsigned char *mem_base)
{
	*(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = colour;
}

void led_RGB2(uint32_t colour, unsigned char *mem_base)
{
	*(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = colour;
}

bool is_tile_occupied(int x, int y)
{
	uint16_t tile = fb[LCD_WIDTH * (y * SIZE_OF_SQUARE) + (x * SIZE_OF_SQUARE)];
	return tile == DARK_RED || tile == DARK_BLUE;
}

void move(int *head_x, int *head_y)
{
	fill_unit(*head_x, *head_y, BLACK);
	switch(dir)
	{
	case UP:
		(*head_y)--;
		break;
	case RIGHT:
		(*head_x)++;
		break;
	case DOWN:
		(*head_y)++;
		break;
	case LEFT:
		(*head_x)--;
		break;
	default:
		break;
	}	
	fill_unit(*head_x, *head_y, WHITE);
	if ((*head_x) < 0 || (*head_x) >= GAME_WIDTH/SIZE_OF_SQUARE || (*head_y) < 0 || (*head_y) >= GAME_HEIGHT/SIZE_OF_SQUARE)
	{
		game_running = false;
	}
}

void input()
{
	if (kbhit())
	{
		switch (getch())
		{
		case 'w':
			if (dir != DOWN)
			{
				dir = UP;
			}
			break;
		case 'a':
			if (dir != RIGHT)
			{
				dir = LEFT;
			}
			break;
		case 's':
			if (dir != UP)
			{
				dir = DOWN;
			}
			break;
		case 'd':
			if (dir != LEFT)
			{
				dir = RIGHT;
			}
			break;
		case 'x':
			game_running = false;
			break;
		default:
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int head_x, head_y;
	
	//uint32_t val_line = 0xffffffff;

	head_x = 1;
	head_y = 1;

	dir = STOP;

	game_running = true;
	
	unsigned char *parlcd_mem_base;
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
    {
    	exit(1);
	}
	parlcd_hx8357_init(parlcd_mem_base);
	
	unsigned char *mem_base;
	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (mem_base == NULL)
    {
    	exit(1);
	}

	fb = (uint16_t *)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
	if (fb == NULL)
	{
		exit(1);
	}
	
	
	/*led_RGB1(LED_RED, mem_base); 
	parlcd_delay(1000);*/
	led_RGB2(LED_RED, mem_base); 
	parlcd_delay(1000);
	led_RGB1(LED_RED, mem_base); 
	parlcd_delay(1000);
	led_RGB2(LED_GREEN, mem_base); 
	parlcd_delay(1000);
	led_RGB1(LED_GREEN, mem_base); 
	parlcd_delay(1000);
	led_RGB2(LED_BLUE, mem_base); 
	parlcd_delay(1000);
	led_RGB1(LED_BLUE, mem_base); 
	parlcd_delay(1000);
	led_RGB1(LED_BLACK, mem_base);
	led_RGB2(LED_BLACK, mem_base); 
	parlcd_delay(1000);
	
	//PrepareKeyboardTtySettings();
	
	/*while (game_running)
	{
		draw(parlcd_mem_base);
		input();
		move(&head_x, &head_y);
		parlcd_delay(1000);
	}*/
	
	/*led_line(LED_LINE_1, mem_base);
	parlcd_delay(1000);
	led_line(LED_LINE_2, mem_base);
	parlcd_delay(1000);
	led_line(LED_LINE_3, mem_base);
	parlcd_delay(1000);
	led_line(LED_LINE_4, mem_base);
	parlcd_delay(1000);
	led_line(LED_LINE_0, mem_base);
  	*/
	/*parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int i = 0; i < 320 ; i++) 
	{
		for (int j = 0; j < 480 ; j++) 
		{
			c = DARK_RED;
			parlcd_write_data(parlcd_mem_base, c);
		}
	}

	parlcd_delay(2000);
	
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int i = 0; i < 320 ; i++) 
	{
		for (int j = 0; j < 160 ; j++) 
		{
			c = DARK_RED;
			parlcd_write_data(parlcd_mem_base, c);
		}
		for (int j = 0; j < 160 ; j++) 
		{
			c = DARK_GREEN;
			parlcd_write_data(parlcd_mem_base, c);
		}
		for (int j = 0; j < 160 ; j++) 
		{
			c = DARK_BLUE;
			parlcd_write_data(parlcd_mem_base, c);
		}
	}

	parlcd_delay(2000);
	*/
	/*parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int i = 0; i < 320 ; i++) 
	{
		for (int j = 0; j < 480 ; j++) 
		{
			fb[i * 480 + j] = BRIGHT_BLUE;
		}
	}

	draw(parlcd_mem_base);*/
	/*
	parlcd_delay(2000);*/
	
	/*for (int i = 0; i < GAME_WIDTH/SIZE_OF_SQUARE; i++)
	{
		add_unit(i, i);
	}*/

	
	/*fill_unit(head_x, head_y, WHITE);


	// for (size_t i = 0; i < GAME_HEIGHT/SIZE_OF_SQUARE; i++)
	// {
	// 	for (size_t j = 0; j < GAME_WIDTH/SIZE_OF_SQUARE; j++)
	// 	{
	// 			fill_unit_border(j,i, 3, DARK_BLUE);
	// 	}
	// }
	// draw(parlcd_mem_base);
	int foodX = 9;
	int foodY = 9;
	fill_unit_border(foodX,foodY, 3, WHITE);
	fill_unit(head_x, head_y, WHITE);

	draw(parlcd_mem_base);
	for (size_t i = 0; i < 20; i++)
	{
	move_to_food_position(&head_x, &head_y, foodX,foodY);
	draw(parlcd_mem_base);
	parlcd_delay(WAIT_TIME);
	}

	foodX = 1;
	foodY = 3;
	fill_unit_border(foodX,foodY, 3, WHITE);
	fill_unit(head_x, head_y, WHITE);
	draw(parlcd_mem_base);
	for (size_t i = 0; i < 20; i++)
	{
	move_to_food_position(&head_x, &head_y, foodX,foodY);
	draw(parlcd_mem_base);
	parlcd_delay(WAIT_TIME);
	}

	foodX = 9;
	foodY = 7;
	fill_unit_border(foodX,foodY, 3, WHITE);
	fill_unit(head_x, head_y, WHITE);
	draw(parlcd_mem_base);
	for (size_t i = 0; i < 20; i++)
	{
	move_to_food_position(&head_x, &head_y, foodX,foodY);
	draw(parlcd_mem_base);

	*/
	/*int x = 10;

	parlcd_delay(WAIT_TIME);
	}
	

	



	// fill_unit(head_x, head_y, WHITE);
	// draw(parlcd_mem_base);
	// parlcd_delay(1000);
	
	// move(&head_x, &head_y);
	// draw(parlcd_mem_base);
	// parlcd_delay(1000);
	
	// move(&head_x, &head_y);
	// draw(parlcd_mem_base);
	// parlcd_delay(1000);
	
	// move(&head_x, &head_y);
	// draw(parlcd_mem_base);
	// parlcd_delay(1000);
	
	// move(&head_x, &head_y);
	// draw(parlcd_mem_base);

	
	
	int xTile = 10;
	char str[]="Goodbye world";
	char *ch=str;
	font_descriptor_t* fdes = &font_winFreeSystem14x16;

	for (int ptr = 0; ptr < 320*480 ; ptr++) 
	{
		fb[ptr]=0u;
	}
	for (int i=0; i<13; i++) 
	{
		draw_char(xTile, 10, fdes, *ch);
		xTile+=char_width(fdes, *ch);
		ch++;
	}*/
	
	free(fb);
	return 0;
}
