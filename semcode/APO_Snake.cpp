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

#include "Snake_Handler.h"
#include "Food.h"
#include "Collision.h"

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
//#define SIZE_OF_PIXEL		4

#define SPEED				1

//one second
#define WAIT_TIME			1000



//VARIABLES USED IN WHOLE GAME
/*
Variable that holds array of LCD display to render
*/
uint16_t *fb;

/*
Variables that holds phys adress of mz_apo board
*/
unsigned char *parlcd_mem_base;
unsigned char *mem_base;

//VARIABLES USED IN GAME LOOP
/*
Enum that has states of game
*/
enum game_state{Game, Menu, Demo, Standard, Options, GameOver, End};
enum game_state gm_state;

/*
It is self-descriptive, :D
*/
bool game_running;

/*
dir - direction of player's snake that is set by pressed keys on PC keyboard 
	in STANDARD mode 
*/
enum Snake_Tile::Direction dir;
Game_Properties game_properties{GAME_WIDTH, GAME_HEIGHT, SIZE_OF_SQUARE, 
								LCD_WIDTH, LCD_HEIGHT, SPEED, SNAKE_1_COLOR, 
								SNAKE_2_COLOR};


//FUNCTIONS USED IN GAME LOOP
/*
update_demo_mode, it is called if game state equals DEMO state. It creates two 
	snakes with simple AI and lets them play. There is a while loop in which 
	game states are updated and	then are rendered. Demo mode ends if any snake 
	died or	if [x] is pressed.
@params 
	Score - to save score of snakes
	Game_Properties - to know game properties
*/
void update_demo_mode(Score* score, Game_Properties game_properties);


/*
Input, it sets global variable dir (direction of snake)	based on pressed key on
	PC keyboard. This function is called only in Standard mode and only snake 
	ONE can be controled by this Input. [W] = UP, [S] = DOWN, [D] = RIGHT, 
	[A] = LEFT, [X] Ends the Standard game mode.
*/
void input();

/*
update_standard_mode, it is called if game state equals STANDARD state.	It 
	creates two snakes, one with AI and other controled by player input. There 
	is a while loop in which game states are updated and then are rendered. 
	Standard mode ends if any snake died or	if [x] is pressed.
@params 
	Score - to save score of snakes
	Game_Properties - to know game properties
*/
void update_standard_mode(Score* score, Game_Properties game_properties);


/*
update_menu_option, it is called if game state equals MENU or OPTIONS state. In 
	menu there are four buttons that can be selected: [1] DEMO mode, 
	[2] STANDARD mode, [3] OPTIONS and [4] QUIT. [1] Starts demo mode, 
	[2] starts standard mode, [3] opens options	in which some parameters of game
	can be changed. [4] Ends game.
	In options there are five buttons: [1] Speed - sets speed (from 1 to 5)	of 
	moves in game (one second to 200 miliseconds). [2] - sets size of Snake 
	tiles in pixels [16..20..32]. [3] - sets color of first snake. [4] - sets
	color of second snake. [5] back to menu (back to menu is also represented
	by pressing key [X]).
	In both Menu and Options selection is represented by pressing keys [W] = up,
	[S] = down, [F] = SELECT, [X] = Back (in options) / End (in menu).
@params 
	Game_Menu - to save previous selection
	Menu_Options - to save previous selection
*/
void update_menu_option(Game_Menu* menu, Menu_Options* menu_options);


/*
update_menu_option, it is called if game state equals GameOver state. It renders
	statistics of snakes: their score and their color. To skip this and return 
	to Menu press [X] or [F].
@params 
	Score - to use its parameteres to render
*/
void update_game_over(Score* score);



/*
update_end_scene starts at the end of game.	To skip this press [X].
*/
void update_end_scene();

/*
loading function starts at the beggining of game and is there just for fun. To 
	skip this press [F].
*/
void loading();

//MAIN**************************************
/*
	Main FUNCTION 
*/
int main(int argc, char* argv[])
{

	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
    {
		fprintf(stderr, "Error, parlcd_mem_base map_phys_adress doesn't work!\n");
    	exit(1);
	}
	parlcd_hx8357_init(parlcd_mem_base);
	

	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	if (mem_base == NULL)
    {
		fprintf(stderr, "Error, mem_base map_phys_adress doesn't work!\n");
    	exit(1);
	}


	fb = (uint16_t*)malloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
	if (fb == NULL)
	{
		fprintf(stderr, "Error, fb array can't be allocated!\n");
		exit(1);
	}

	//to prepare keyboard
	PrepareKeyboardTtySettings();

	led_RGB2(LED_PINK, mem_base);
	led_RGB1(LED_PINK, mem_base);
	led_line(LED_LINE_0, mem_base);

	loading();

	led_RGB2(LED_BLACK, mem_base);
	led_RGB1(LED_BLACK, mem_base);
	
		
	gm_state = Menu;
	Score score = Score();
	Game_Menu menu = Game_Menu();
	Menu_Options menu_options = Menu_Options();
	
	clear_array(BLACK, fb);
	draw(parlcd_mem_base, fb);
	game_running = true;
	//game loop
	while (game_running)
	{
		while (gm_state == Menu || gm_state == Options)
		{
			update_menu_option(&menu, &menu_options);
		}
		if (gm_state == Demo)
		{
			update_demo_mode(&score, game_properties);	
		}
		if (gm_state == Standard)
		{
			update_standard_mode(&score, game_properties);	
		}
		while (gm_state == GameOver)
		{
			update_game_over(&score);
		}
	}

	update_end_scene();

	clear_array(BLACK, fb);
	led_RGB2(LED_BLACK, mem_base);
	led_RGB1(LED_BLACK, mem_base);
	led_line(LED_LINE_0, mem_base);

	free(fb);
	return 0;
}

//END OF MAIN*********************************

void update_demo_mode(Score* score, Game_Properties game_properties)
{
	clear_array(BLACK, fb);
	score->reset();
	bool is_player = false;
	Snake_Handler handler = Snake_Handler();
	Snake sn = Snake(2, 2, game_properties.color_1, game_properties, is_player);
	Snake sn2 = Snake(9, 9, game_properties.color_2, game_properties, is_player);
	sn.set_opposite_snake(&sn2);
	sn2.set_opposite_snake(&sn);
	handler.add_snake(&sn);
	handler.add_snake(&sn2);
	Food food = Food(&handler, 5, 5, 3, FOOD_COLOR, GAME_WIDTH, GAME_HEIGHT, 
					game_properties.size_of_tile);
	food.fill_array(fb, LCD_WIDTH);
	handler.fill_array(fb, LCD_WIDTH);
	score->score_fill_array(fb, parlcd_mem_base);
	draw(parlcd_mem_base, fb);
	parlcd_delay(WAIT_TIME / game_properties.speed);
	
	while (gm_state == Demo)
	{
		food.update(mem_base);
		handler.update();
		score->update(handler);
		
		if (collision_update(handler, game_properties, mem_base))
		{
			clear_array(BLACK, fb);
			handler.delete_snakes();
			gm_state = GameOver;
		}
		else
		{
			food.fill_array(fb, LCD_WIDTH);
			handler.fill_array(fb, LCD_WIDTH);
			score->score_fill_array(fb, parlcd_mem_base);

			draw(parlcd_mem_base, fb);
			parlcd_delay(WAIT_TIME / game_properties.speed);
		}
		
		if (kbhit() && getch() == 'x')
		{
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

void update_standard_mode(Score* score, Game_Properties game_properties)
{
	score->reset();
	clear_array(BLACK, fb);
	printf("Game STANDARD mode: starts.\n");
	bool is_player = true;
	Snake_Handler handler = Snake_Handler();
	Snake sn = Snake(2, 2, game_properties.color_1, game_properties, is_player);
	Snake sn2 = Snake(9, 9, game_properties.color_2, game_properties, !is_player);
	sn2.set_opposite_snake(&sn);
	sn.set_opposite_snake(&sn2);
	handler.add_snake(&sn);
	handler.add_snake(&sn2);
	Food food = Food(&handler, 5, 5, 3, FOOD_COLOR, GAME_WIDTH, GAME_HEIGHT, 
					game_properties.size_of_tile);
	food.fill_array(fb, LCD_WIDTH);
	handler.fill_array(fb, LCD_WIDTH);
	score->score_fill_array(fb, parlcd_mem_base);
	draw(parlcd_mem_base, fb);
	parlcd_delay(WAIT_TIME / game_properties.speed);
	
	dir = Snake_Tile::NONE;
	while (dir == Snake_Tile::NONE)
	{
		input();
		parlcd_delay(WAIT_TIME / game_properties.speed);
	}
	sn.set_direction(dir);
	
	while (gm_state == Standard)
	{
		food.update(mem_base);
		handler.update();
		score->update(handler);
		
		if (collision_update(handler, game_properties, mem_base))
		{
			clear_array(BLACK, fb);
			handler.delete_snakes();
			gm_state = GameOver;
		}
		else
		{
			food.fill_array(fb, LCD_WIDTH);
			handler.fill_array(fb, LCD_WIDTH);
			score->score_fill_array(fb, parlcd_mem_base);
			draw(parlcd_mem_base, fb);
			parlcd_delay(WAIT_TIME / game_properties.speed);
		}
		
		input();
		sn.set_direction(dir);
	}        
}


void update_game_over(Score* score)
{
	if (kbhit() && game_over_update(getch()) == 1)
	{
		gm_state = Menu;
		score->reset();
	}
	game_over_fill_array(fb, parlcd_mem_base, score->get_score_1(), 
						score->get_score_2(), score->get_color_1(), 
						score->get_color_2());
	draw(parlcd_mem_base, fb);
}

void update_menu_option(Game_Menu* menu, Menu_Options* menu_options)
{
	clear_array(BLACK, fb);
	if (kbhit())
	{
		if (gm_state==Menu)
		{
			switch (menu->menu_selection(getch()))
			{
			case 1:
				printf("Selected Game DEDMO mode.\n");
				gm_state = Demo;
				break;
			case 2:
				printf("Selected Game STANDARD mode\n");
				gm_state = Standard;
				break;
			case 3:
				printf("Selected Options.\n");
				gm_state = Options;
				break;
			case 4:
				printf("Selected END!\n");
				gm_state = End;
				game_running = false;
				break;
			}
		}
		else if (gm_state == Options)
		{
			int select = menu_options->options_selection(getch(), 
														game_properties.speed, 
														game_properties.size_of_tile, 
														game_properties.color_1, 
														game_properties.color_2, 
														mem_base);
			if (select ==1)
			{
				gm_state = Menu;
			}
		}
	}
	
	if (gm_state == Options)
	{
		menu_options->options_fill_array(fb, parlcd_mem_base, game_properties);
	}
	else if (gm_state == Menu)
	{
		menu->menu_fill_array(fb, parlcd_mem_base, game_properties);
	}
	led_RGB2(LED_PINK, mem_base);
	led_RGB1(LED_PINK, mem_base);
	draw(parlcd_mem_base, fb);
}

void update_end_scene()
{
	End_Scene es = End_Scene();
	for (int i = 0; i < es.get_max(); i++)
	{
		if (kbhit() && getch() == 'x')
		{
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
}

void loading()
{
	int xTile = (LCD_WIDTH / 8) - 35, yTile = 3 * (LCD_HEIGHT / 16);
	char str[] = "LOADING...";
	int size_of_str = 0;
	while (str[size_of_str] != 0)
	{
		size_of_str++;
	}
	char* begin = str;
	draw_string(xTile, yTile, BRIGHT_WHITE, begin, size_of_str, fb);

	bool end = true;
	int pointer = 1;
	bool jump = true;
	while (end)
	{
		if (kbhit() && getch() == 'f')
		{
			printf("PRESSED F.\n");
			printf("MENU.\n");
			end = false;
		}
		for (int i=0; i<120; i++)
		{
			if (pointer==0)
			{
				//clear_array(BLACK, fb);
				int xTile = (LCD_WIDTH / 8) - 35, yTile = 3 * (LCD_HEIGHT / 16);
            	char str[] = "PRESS:  F ";
				int size_of_str = 0;
				while (str[size_of_str] != 0)
				{
					size_of_str++;
				}
				char* begin = str;
				draw_string(xTile, yTile, BRIGHT_WHITE, begin, size_of_str, fb);
			}
			
			if (i >= (pointer - 1) * 10 && i < pointer * 10)
			{
				draw_pixel(i, (int)(-round(10 * sin(2 * (i * (M_PI / 45)))) + 40), 
							BRIGHT_WHITE, fb);
			}
			else
			{
				draw_pixel(i, (int)(-round(10 * sin(2 * (i * (M_PI / 45)))) + 40), 
							DARK_WHITE, fb);
			}
		}
		pointer = (jump) ? pointer + 1 : pointer - 1;
		if (pointer == 120 / 10)
		{
			jump = false;
		}
		if (pointer == 0)
		{
			jump = true;
		}
		draw(parlcd_mem_base, fb);

		parlcd_delay(50);
	}
}
