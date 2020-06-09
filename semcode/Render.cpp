#include "Render.h"


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
}

void draw_pixel(int xTile, int yTile, uint16_t colour, uint16_t *pixels) 
{
	if (xTile>=0 && xTile<LCD_WIDTH/SIZE_OF_PIXEL && yTile>=0 && yTile<LCD_HEIGHT/SIZE_OF_PIXEL) 
	{	
		for (int i = yTile * SIZE_OF_PIXEL; i < SIZE_OF_PIXEL * yTile + SIZE_OF_PIXEL; i++)
		{
			for (int j = xTile * SIZE_OF_PIXEL; j < SIZE_OF_PIXEL * xTile + SIZE_OF_PIXEL; j++)
			{
				pixels[LCD_WIDTH * i + j] = colour;
			}
		}
	}
}

void draw_lineX(int xTile, int yTile, int size, uint16_t colour, uint16_t *pixels){
	for (size_t i = 0; i < size; i++)
	{
		draw_pixel(xTile + i, yTile, colour, pixels);
	}
}

void draw_lineY(int xTile, int yTile, int size, uint16_t colour, uint16_t *pixels){
	for (size_t i = 0; i < size; i++)
	{
		draw_pixel(xTile, yTile+i, colour, pixels);
	}
}

void draw_char(int xTile, int yTile, font_descriptor_t* fdes, int ch, int back, uint16_t colour, uint16_t *pixels) {
	int w, bw, i, j;
	const uint16_t *b;
	unsigned patt = 0;
	
	ch -= fdes->firstchar;
	if (!fdes->width)
	{
		w = fdes->maxwidth;
	}
	else
	{
		w = fdes->width[ch];
	}
	bw = (w + 15) / 16;
	
	if (!fdes->offset)
	{
		b = fdes->bits + ch * bw * fdes->height;
	}
	else
	{
		b = fdes->bits + fdes->offset[ch];
	}
	
	for (j = 0; j < fdes->height; j++)
	{
		for (i = 0; i < w; i++)
		{
			if (!(i & 15))
			{
				patt = *(b++);
			}
			if (patt & 0x8000 || back)
			{
				draw_pixel(xTile + i, yTile + j, patt & 0x8000 ? colour : 0, pixels);
			}
			patt <<= 1;
		}
	}
}


void draw_string(int xTile, int yTile, uint16_t colour, char *ch, int size_of_string, uint16_t *pixels)
{
	font_descriptor_t* fdes = &font_winFreeSystem14x16;

	for (int i = 0; i < size_of_string; i++) 
	{
		draw_char(xTile, yTile, fdes, *ch, 1, colour, pixels);
		xTile += char_width(fdes, *ch);
		ch++;
	}
}

// void draw_string_upgraded(int xTile, int yTile, uint16_t colour, char* str_draw){
// 	char str[] = str_draw;
// 	int size_of_str = 0;
// 	while (str[size_of_str] != 0)
// 	{
// 		size_of_str++;
// 	}
// 	char *begin = str;
// 	draw_string(xTile, yTile, DARK_RED, begin, size_of_str, fb);
// }

void draw_rect(int x, int y, int width, int height, int border, uint16_t color, uint16_t *fb){
	if(y<=LCD_HEIGHT && y>=0 && x<=LCD_WIDTH && x>=0){
		for (int i = y; i < y + border; i++) {
			for (int j = x; j < x + width; j++) {
				fb[LCD_WIDTH * i + j] = color;
			}
		}
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + border; j++) {
				fb[LCD_WIDTH * i + j] = color;
			}
		}
		for (int i = y; i < y + height; i++) {
			for (int j = x + width - border; j < x+ width; j++) {
				fb[LCD_WIDTH * i + j] = color;
			}
		}
		for (int i = y + height - border; i < y + height; i++) {
			for (int j = x; j < x + width; j++) {
				fb[LCD_WIDTH * i + j] = color;
			}
		}
	}
}

void draw_filled_rect(int x, int y, int width, int height, uint16_t color, uint16_t *fb){
	if(y<=LCD_HEIGHT && y>=0 && x<=LCD_WIDTH && x>=0){
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + width; j++) {
				fb[LCD_WIDTH * i + j] = color;
			}
		}
	}
}

void fill_unit(int xTile, int yTile, uint16_t colour, uint16_t* pixels)
{
	if (xTile >= 0 && xTile < GAME_WIDTH/SIZE_OF_SQUARE && yTile >= 0 && yTile < GAME_HEIGHT/SIZE_OF_SQUARE)
	{
		for (int i = yTile * SIZE_OF_SQUARE; i < SIZE_OF_SQUARE * yTile + SIZE_OF_SQUARE; i++)
		{
			for (int j = xTile * SIZE_OF_SQUARE; j < SIZE_OF_SQUARE * xTile + SIZE_OF_SQUARE; j++)
			{
				pixels[LCD_WIDTH * i + j] = colour;
			}
		}
	}
}

void clear_array(uint16_t colour, uint16_t* pixels)
{
	for (size_t i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
	{
		pixels[i] = colour;
	}
	
	// for (size_t i = 0; i < LCD_HEIGHT; i++)
	// {
	// 	for (size_t j = 0; j < LCD_WIDTH; j++)
	// 	{
	// 		fill_unit(j,i, colour, pixels);
	// 	}
	// }
}


void draw(unsigned char *parlcd_mem_base, uint16_t* pixels)
{
	//printf("START of Draw\n");
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) 
	{
		parlcd_write_data(parlcd_mem_base, pixels[i]);
	}
	//printf("End of Draw\n");
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



