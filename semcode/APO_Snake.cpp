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

//#include "Snake_Computer.cpp"



#define LCD_WIDTH			480
#define LCD_HEIGHT			320
#define GAME_WIDTH			320
#define GAME_HEIGHT			320


#define SIZE_OF_SQUARE		32
#define SIZE_OF_PIXEL		4

// #define SIZE_OF_SQUARE		24


#define WAIT_TIME			100


// #define PURPLE				0x780f
// //#define YELLOW  			0x0e5f
// #define BRIGHT_RED 			0xf800
// #define BRIGHT_GREEN 		0x07e0
// #define BRIGHT_BLUE 		0x001f
// #define DARK_RED 			0x7800
// #define DARK_GREEN	 		0x01e0
// #define DARK_BLUE 			0x000f
// #define BRIGHT_WHITE		0xffff
// #define DARK_WHITE			0x5555
// #define BLACK 				0x0000
// #define WHITE				0xffff 


// #define LED_LINE_0			0x00000000
// #define LED_LINE_1			0xff000000
// #define LED_LINE_2			0xffff0000
// #define LED_LINE_3			0xffffff00
// #define LED_LINE_4			0xffffffff

// #define LED_RED				0xff0000
// #define LED_GREEN			0x00ff00
// #define LED_BLUE			0x0000ff
// #define LED_BLACK			0x000000

uint16_t *fb;
unsigned char *parlcd_mem_base;
unsigned char *mem_base;

enum game_state{Game, Menu, Demo, Standard, Options, GameOver, End};
enum game_state gm_state;
enum direction{STOP, UP, RIGHT, DOWN, LEFT};
enum direction dir;

bool game_running;

uint16_t snake_1_color = SNAKE_1_COLOR;
uint16_t snake_2_color = SNAKE_2_COLOR;

void render(){
	
}

void update_demo_mode(Score* score, Game_Properties game_properties){
	clear_array(BLACK, fb);
	Snake_Handler handler = Snake_Handler();
	Snake sn = Snake(2, 2, snake_1_color, game_properties);
	Snake sn2 = Snake(9, 9, snake_2_color, game_properties);
	sn.set_opposite_snake(&sn2);
	sn2.set_opposite_snake(&sn);
	handler.add_snake(&sn);
	handler.add_snake(&sn2);
	Food food = Food(&handler, 5, 5, 3, FOOD_COLOR, GAME_WIDTH, GAME_HEIGHT, game_properties.size_of_tile);
	food.fill_array(fb,LCD_WIDTH);
	handler.fill_array(fb, LCD_WIDTH);
	draw(parlcd_mem_base, fb);
	parlcd_delay(WAIT_TIME);
	int counter = 0;
	while(gm_state == Demo){
		food.update();
		handler.update();
		score->update(handler);
		if(collision_update(handler, game_properties)){
			clear_array(BLACK, fb);
			handler.delete_snakes();
			gm_state = GameOver;
		}else{
			food.fill_array(fb,LCD_WIDTH);
			handler.fill_array(fb, LCD_WIDTH);
			score->score_fill_array(fb, parlcd_mem_base);

			draw(parlcd_mem_base, fb);
			parlcd_delay(WAIT_TIME);
			counter++;
			if(counter > 400){
				clear_array(BLACK, fb);
				counter = 0;
				printf("SNAKES ENDED!\n");
				gm_state = GameOver;
			}
		}
	}	
}

bool is_tile_occupied(int x, int y)
{
	uint16_t tile = fb[LCD_WIDTH * (y * SIZE_OF_SQUARE) + (x * SIZE_OF_SQUARE)];
	return tile == DARK_RED || tile == DARK_BLUE;
}

void move(int *head_x, int *head_y)
{
	fill_unit(*head_x, *head_y, BLACK, fb);
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
	fill_unit(*head_x, *head_y, DARK_WHITE, fb);
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

	/*
	int head_x, head_y;
	uint32_t val_line = 0xffffffff;

	head_x = 1;
	head_y = 1;

	dir = STOP;

	game_running = true;
	*/
	
	//unsigned char *parlcd_mem_base;
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	if (parlcd_mem_base == NULL)
    {
    	exit(1);
	}
	parlcd_hx8357_init(parlcd_mem_base);
	

	//unsigned char *mem_base;
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

	// game_over_fill_array(fb, parlcd_mem_base, 99, 99,BRIGHT_GREEN, BRIGHT_RED);
	// draw(parlcd_mem_base, fb);

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
		
	PrepareKeyboardTtySettings();
	gm_state = Menu;
	Score score = Score();
	Game_Menu menu = Game_Menu();
	Menu_Options menu_options = Menu_Options();
	Game_Properties game_properties{GAME_WIDTH, GAME_HEIGHT, SIZE_OF_SQUARE, LCD_WIDTH, LCD_HEIGHT};

	int speed = 1;

	int  marked_item= 1;
	int counter2 = 0;
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
						printf("SELECT GAME STANDARD\n");
						gm_state = Standard;
						exit(1); //doesnt work yet
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
					int select = menu_options.options_selection(getch(),pointer, speed, game_properties.size_of_tile, snake_1_color, snake_2_color);
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
			draw(parlcd_mem_base, fb);
			//parlcd_delay(100);
		}
		if (gm_state == Demo){
			update_demo_mode(&score, game_properties);	
		}
		if (gm_state == Standard){
			score.reset();
			clear_array(BLACK, fb);
			Snake_Handler handler = Snake_Handler();
			Snake sn = Snake(2, 2, snake_1_color, game_properties);
			Food food = Food(&handler, 5, 5, 3, FOOD_COLOR, GAME_WIDTH, GAME_HEIGHT, game_properties.size_of_tile);
			food.fill_array(fb,LCD_WIDTH);
			handler.fill_array(fb, LCD_WIDTH);
			draw(parlcd_mem_base, fb);
			parlcd_delay(WAIT_TIME);
			while(gm_state == Standard){
				food.update();
				handler.update();
				//score.update(handler);
				if(collision_update(handler, game_properties)){
					clear_array(BLACK, fb);
					handler.delete_snakes();
					gm_state = GameOver;
				}else{
					food.fill_array(fb,LCD_WIDTH);
					handler.fill_array(fb, LCD_WIDTH);
					score.score_fill_array(fb, parlcd_mem_base);

					draw(parlcd_mem_base, fb);
					parlcd_delay(WAIT_TIME);
					counter2++;
					if(counter2 > 150){
						counter2 = 0;
						printf("SNAKES EDE\n");
						gm_state = GameOver;
					}
				}
			}	
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


	free(fb);
	return 0;



	led_RGB2(LED_RED, mem_base); 
	parlcd_delay(2*WAIT_TIME);
	led_RGB1(LED_RED, mem_base); 
	parlcd_delay(2*WAIT_TIME);
	led_RGB2(LED_GREEN, mem_base); 
	parlcd_delay(2*WAIT_TIME);
	led_RGB1(LED_GREEN, mem_base); 
	parlcd_delay(2*WAIT_TIME);
	led_RGB2(LED_BLUE, mem_base); 
	parlcd_delay(2*WAIT_TIME);
	led_RGB1(LED_BLUE, mem_base); 
	parlcd_delay(2*WAIT_TIME);
	led_RGB1(LED_BLACK, mem_base);
	led_RGB2(LED_BLACK, mem_base); 
	parlcd_delay(2*WAIT_TIME);

	
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

	// PrepareKeyboardTtySettings();
	
	// while (game_running)
	// {
	// 	draw(parlcd_mem_base);
	// 	input();
	// 	move(&head_x, &head_y);
	// 	parlcd_delay(1000);
	// }
  
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
	}*/
	

	



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
	
	
	// int xTile = 2, yTile = 10;
	// char str[]="HA ";
	// int size_of_str = 0;
	// while (str[size_of_str] != 0)
	// {
	// 	size_of_str++;
	// }
	// char *begin = str;
	// draw_string(xTile, yTile, PURPLE, begin, size_of_str, fb);
	// draw(parlcd_mem_base, fb);
	// parlcd_delay(100);

	// int xTile2 = 10, yTile2 = 30;
	// char str2[]="PLAY DEMO";
	// int size_of_str2 = 0;
	// while (str2[size_of_str2] != 0)
	// {
	// 	size_of_str2++;
	// }
	// char *begin2 = str2;
	// draw_string(xTile2, yTile2, DARK_BLUE, begin2, size_of_str2);
	// draw(parlcd_mem_base);

	// xTile2 = 10; yTile2 = 50;
	// char str3[] = "PLAY STANDARD";
	// size_of_str2 = 0;
	// while (str3[size_of_str2] != 0)
	// {
	// 	size_of_str2++;
	// }
	// begin2 = str3;
	// draw_string(xTile2, yTile2, DARK_BLUE, begin2, size_of_str2);
	// for(int i = 0; i<100; i++){
	// 	draw_pixel(i,2, PURPLE);
	// }
	// for(int i=1; i<120; i++){
	// 	printf("1: %d\n", (int)(-round(sin((5*(i*M_PI/45))))+40));
	// 	printf("2: %d\n", (int)(-round(7*sin((i*M_PI)/180))+40));
	// 	draw_pixel(i, (int)(-round(10*sin((2*(i*M_PI/45))))+40), 0xffff);
	// 	//draw_pixel(i, (int)(-round(10*log((2*(i*M_PI/45))))+40), 0xffff);
		
	// 	draw_pixel(i,40,PURPLE);
	// }
	

	//handler.delete_snakes();
	free(fb);
	return 0;
}
