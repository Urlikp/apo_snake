/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

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

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#include "font_types.h"

#define LCD_WIDTH			480
#define LCD_HEIGHT			320
#define GAME_WIDTH			320
#define GAME_HEIGHT			320
#define SIZE_OF_SQUARE		32
#define BRIGHT_RED 			0xf800
#define BRIGHT_GREEN 		0x07e0
#define BRIGHT_BLUE 		0x001f
#define DARK_RED 			0x7800
#define DARK_GREEN	 		0x01e0
#define DARK_BLUE 			0x000f
#define WHITE 				0xffff
#define BLACK 				0x0000

uint16_t *fb;
enum direction{UP, RIGHT, DOWN, LEFT};
enum direction dir;

/*void draw_pixel(int x, int y, unsigned short color) 
{
	if (x>=0 && x<480 && y>=0 && y<320) 
	{
		fb[x+480*y] = color;
	}
}

void draw_char(int x, int y, font_descriptor_t* fdes, char ch) {
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

void fill_unit(int x, int y, unsigned short colour)
{
	if (x >= 0 && x < GAME_WIDTH/SIZE_OF_SQUARE && y >= 0 && y < GAME_HEIGHT/SIZE_OF_SQUARE)
	{
		for (int i = y * SIZE_OF_SQUARE; i < SIZE_OF_SQUARE * y + SIZE_OF_SQUARE; i++)
		{
			for (int j = x * SIZE_OF_SQUARE; j < SIZE_OF_SQUARE * x + SIZE_OF_SQUARE; j++)
			{
				fb[LCD_WIDTH * i + j] = colour;
			}
		}
	}
}

void draw(unsigned char *parlcd_mem_base)
{
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) 
	{
		parlcd_write_data(parlcd_mem_base, fb[i]);
	}
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
}

int main(int argc, char *argv[])
{
	//unsigned short c;
	int head_x, head_y;

	head_x = 4;
	head_y = 4;

	dir = RIGHT;

	unsigned char *parlcd_mem_base;
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	parlcd_hx8357_init(parlcd_mem_base);

	fb = (uint16_t *)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));

  
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
	
	fill_unit(head_x, head_y, WHITE);
	draw(parlcd_mem_base);
	parlcd_delay(1000);
	
	move(&head_x, &head_y);
	draw(parlcd_mem_base);
	parlcd_delay(1000);
	
	move(&head_x, &head_y);
	draw(parlcd_mem_base);
	parlcd_delay(1000);
	
	move(&head_x, &head_y);
	draw(parlcd_mem_base);
	parlcd_delay(1000);
	
	move(&head_x, &head_y);
	draw(parlcd_mem_base);
	
	/*int x = 10;
	char str[]="Goodbye world";
	char *ch=str;
	font_descriptor_t* fdes = &font_winFreeSystem14x16;

	for (int ptr = 0; ptr < 320*480 ; ptr++) 
	{
		fb[ptr]=0u;
	}
	for (int i=0; i<13; i++) 
	{
		draw_char(x, 10, fdes, *ch);
		x+=char_width(fdes, *ch);
		ch++;
	}*/

  return 0;
}
