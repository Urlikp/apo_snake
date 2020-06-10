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

#include "Render.h"

#include "input.h"
#include "Game_Properites.h"

#include "Snake.cpp"
#include "Snake_Handler.cpp"
#include "Food.cpp"
#include "Collision.cpp"

#include "Menu.h"
#include "options.h"
#include "end_scene.h"
#include "score.h"
#include "game_over.h"


#define LCD_WIDTH			480
#define LCD_HEIGHT			320
#define GAME_WIDTH			320
#define GAME_HEIGHT			320


#define SIZE_OF_SQUARE		32
#define SIZE_OF_PIXEL		4

#define SPEED				1


#define WAIT_TIME			1000


uint16_t *fb;
unsigned char *parlcd_mem_base;
unsigned char *mem_base;

enum game_state{Game, Menu, Demo, Standard, Options, GameOver, End};
enum game_state gm_state;
//enum direction{STOP, UP, RIGHT, DOWN, LEFT};
enum Snake_Tile::Direction dir;

bool game_running;

uint16_t snake_1_color = SNAKE_1_COLOR;
uint16_t snake_2_color = SNAKE_2_COLOR;

void render(){
	
}

void update_demo_mode(Score* score, Game_Properties game_properties){
	clear_array(BLACK, fb);
	score->reset();
	Snake_Handler handler = Snake_Handler();
	Snake sn = Snake(2, 2, snake_1_color, game_properties, false);
	Snake sn2 = Snake(9, 9, snake_2_color, game_properties, false);
	sn.set_opposite_snake(&sn2);
	sn2.set_opposite_snake(&sn);
	handler.add_snake(&sn);
	handler.add_snake(&sn2);
	Food food = Food(&handler, 5, 5, 3, FOOD_COLOR, GAME_WIDTH, GAME_HEIGHT, game_properties.size_of_tile);
	food.fill_array(fb,LCD_WIDTH);
	handler.fill_array(fb, LCD_WIDTH);
	score->score_fill_array(fb, parlcd_mem_base);
	draw(parlcd_mem_base, fb);
	parlcd_delay(WAIT_TIME/game_properties.speed);
	while(gm_state == Demo){
		food.update(mem_base);
		handler.update();
		score->update(handler);
		if(collision_update(handler, game_properties, mem_base)){
			clear_array(BLACK, fb);
			handler.delete_snakes();
			gm_state = GameOver;
		}else{
			food.fill_array(fb,LCD_WIDTH);
			handler.fill_array(fb, LCD_WIDTH);
			score->score_fill_array(fb, parlcd_mem_base);

			draw(parlcd_mem_base, fb);
			parlcd_delay(WAIT_TIME/game_properties.speed);
		}
		if(kbhit() && getch()=='x'){
			gm_state = GameOver;
			clear_array(BLACK, fb);
			led_RGB1(LED_RED, mem_base);
			led_RGB2(LED_RED, mem_base);
		}
	}	
}

void input()
{
	if (kbhit())
	{
		switch (getch())
		{
		case 'w':
			if (dir != Snake_Tile::DOWN)
			{
				dir = Snake_Tile::UP;
			}
			break;
		case 'a':
			if (dir != Snake_Tile::RIGHT)
			{
				dir = Snake_Tile::LEFT;
			}
			break;
		case 's':
			if (dir != Snake_Tile::UP)
			{
				dir = Snake_Tile::DOWN;
			}
			break;
		case 'd':
			if (dir != Snake_Tile::LEFT)
			{
				dir = Snake_Tile::RIGHT;
			}
			break;
		case 'x':
			gm_state = GameOver;
			clear_array(BLACK, fb);
			led_RGB1(LED_RED, mem_base);
			led_RGB2(LED_RED, mem_base);
		default:
			break;
		}
	}
}


void update_standard_mode(Score* score, Game_Properties game_properties){
	score->reset();
	clear_array(BLACK, fb);
	printf("START\n");
	Snake_Handler handler = Snake_Handler();
	Snake sn = Snake(2, 2, snake_1_color, game_properties, true);
	Snake sn2 = Snake(9, 9, snake_2_color, game_properties, false);
	sn2.set_opposite_snake(&sn);
	sn.set_opposite_snake(&sn2);
	handler.add_snake(&sn);
	handler.add_snake(&sn2);
	Food food = Food(&handler, 5, 5, 3, FOOD_COLOR, GAME_WIDTH, GAME_HEIGHT, game_properties.size_of_tile);
	food.fill_array(fb,LCD_WIDTH);
	handler.fill_array(fb, LCD_WIDTH);
	score->score_fill_array(fb, parlcd_mem_base);
	draw(parlcd_mem_base, fb);
	parlcd_delay(WAIT_TIME/game_properties.speed);
	dir = Snake_Tile::NONE;
	while (dir == Snake_Tile::NONE)
	{
		input();
		parlcd_delay(WAIT_TIME/game_properties.speed);
	}
	sn.set_direction(dir);
	while (gm_state == Standard){
		food.update(mem_base);
		handler.update();
		score->update(handler);
		if(collision_update(handler, game_properties,mem_base)){
			clear_array(BLACK, fb);
			handler.delete_snakes();
			gm_state = GameOver;
		}else{
			food.fill_array(fb,LCD_WIDTH);
			handler.fill_array(fb, LCD_WIDTH);
			score->score_fill_array(fb, parlcd_mem_base);
			draw(parlcd_mem_base, fb);
			parlcd_delay(WAIT_TIME/game_properties.speed);
		}
		input();
		sn.set_direction(dir);
	}        
}

bool is_tile_occupied(int x, int y)
{
	uint16_t tile = fb[LCD_WIDTH * (y * SIZE_OF_SQUARE) + (x * SIZE_OF_SQUARE)];
	return tile == DARK_RED || tile == DARK_BLUE;
}



int main(int argc, char *argv[])
{

	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
    {
		fprintf(stderr,"Error, parlcd_mem_base map_phys_adress doesn't work!\n");
    	exit(1);
	}
	parlcd_hx8357_init(parlcd_mem_base);
	

	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (mem_base == NULL)
    {
		fprintf(stderr,"Error, mem_base map_phys_adress doesn't work!\n");
    	exit(1);
	}


	fb = (uint16_t *)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
	if (fb == NULL)
	{
		fprintf(stderr,"Error, fb array can't be allocated!\n");
		exit(1);
	}

	PrepareKeyboardTtySettings();
	
	led_RGB2(LED_PINK, mem_base);
	led_RGB1(LED_PINK, mem_base);
	led_line(LED_LINE_0, mem_base);


	bool end = true;
	int pointer = 1;
	bool jump = true;
	while(end){
		if(kbhit() && getch()=='x'){
			end = false;
		}
		for(int i=0; i<120; i++){
			if(i>=(pointer-1)*10 && i< pointer*10){
				draw_pixel(i, (int)(-round(10*sin((2*(i*M_PI/45))))+40), BRIGHT_WHITE, fb);
			}else{
				draw_pixel(i, (int)(-round(10*sin((2*(i*M_PI/45))))+40), DARK_WHITE, fb);
			}
			//draw_pixel(i,40,0x3333,fb);
		}
		pointer = (jump) ? pointer +1: pointer -1;
		if(pointer==120/10){
			jump = false;
		}
		if (pointer== 0){
			jump = true;
		}
		draw(parlcd_mem_base, fb);

		parlcd_delay(50);
	}

	led_RGB2(LED_BLACK, mem_base);
	led_RGB1(LED_BLACK, mem_base);
	
		
	gm_state = Menu;
	Score score = Score();
	Game_Menu menu = Game_Menu();
	Menu_Options menu_options = Menu_Options();
	Game_Properties game_properties{GAME_WIDTH, GAME_HEIGHT, SIZE_OF_SQUARE, LCD_WIDTH, LCD_HEIGHT, SPEED};


	clear_array(BLACK, fb);
	draw(parlcd_mem_base, fb);
	game_running = true;
	while (game_running){
		while(gm_state==Menu || gm_state== Options){
			clear_array(BLACK, fb);
			if(kbhit()){
				if(gm_state==Menu){
					switch (menu.menu_selection(getch()))
					{
					case 1:
						printf("SELECT GAME DEMO\n");
						gm_state = Demo;
						break;
					case 2:
						printf("SELECT GAME STANDAR\n");
						gm_state = Standard;
						break;

					case 3:
						printf("SELECT OPTIONS\n");
						gm_state = Options;
						break;
					case 4:
						printf("SELECT END!\n");
						gm_state = End;
						game_running = false;
						break;
					}
				}else if (gm_state == Options){
					int pointer = 1;
					int select = menu_options.options_selection(getch(),pointer, game_properties.speed, game_properties.size_of_tile, snake_1_color, snake_2_color, mem_base);
					if (select ==1){
						gm_state = Menu;
					}
				}
			}
			if(gm_state == Options){
				menu_options.options_fill_array(fb, parlcd_mem_base, game_properties);
			}else if (gm_state == Menu){
				menu.menu_fill_array(fb, parlcd_mem_base, game_properties);
			}
			led_RGB2(LED_PINK, mem_base);
			led_RGB1(LED_PINK, mem_base);
			draw(parlcd_mem_base, fb);
		}
		if (gm_state == Demo){
			update_demo_mode(&score, game_properties);	
		}
		if (gm_state == Standard){
			update_standard_mode(&score, game_properties);	
		}
		while(gm_state == GameOver){
			if(kbhit() && game_over_update(getch())==1){
				gm_state = Menu;
				score.reset();
			}
			game_over_fill_array(fb, parlcd_mem_base, score.get_score_1(), score.get_score_2(),score.get_color_1(),score.get_color_2());
			draw(parlcd_mem_base, fb);
		}
	}


	End_Scene es = End_Scene();
	for(int i = 0; i < es.get_max(); i++){
		if(kbhit() && getch()=='x'){
			break;
		}
		es.end_scene_update();
		clear_array(BLACK, fb);
		es.end_scenefill_array(fb, parlcd_mem_base);
		draw(parlcd_mem_base, fb);
	}
	led_RGB2(LED_PINK, mem_base);
	led_RGB1(LED_PINK, mem_base);
	parlcd_delay(WAIT_TIME); 
	clear_array(BLACK, fb);
	draw(parlcd_mem_base, fb);
	parlcd_delay(WAIT_TIME);
	led_RGB2(LED_BLACK, mem_base);
	led_RGB1(LED_BLACK, mem_base);
	led_line(LED_LINE_0, mem_base);

	free(fb);
	return 0;
}
