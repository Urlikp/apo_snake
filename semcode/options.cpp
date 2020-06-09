#include "options.h"



int Menu_Options::options_selection(char key_pressed, int& pointer,  int& speed, int& size_of_tile, uint16_t& snake_1_color, uint16_t& snake_2_color){
        
    int return_value = 0;
    int number_of_items = 5;

    if (key_pressed == 'w'){
        printf("move up\n");
        pointer_to_item--;
    }

    if (key_pressed == 's'){
        printf("move down\n");
        pointer_to_item++;
    }
    
    if (key_pressed == 'x'){
        fprintf(stderr,"Game has been ended!\n");
        exit(0);
    }

    if (pointer_to_item<1)
    {
        printf("switched\n");
        pointer_to_item = number_of_items;
    }

    if(pointer_to_item>5){
        printf("switched2\n");
        pointer_to_item = 1;
    }

    if (key_pressed == 'f'){
        switch (pointer_to_item)
        {
            case 1:
                //printf("sp: %d\n", copy_speed);
                speed++;
                if(speed>4){
                    speed=1;
                }
                copy_speed = speed;
                break;
            case 2:
                if (size_of_tile == 16){
                    size_of_tile = 32;
                }else if(size_of_tile == 32){
                    size_of_tile = 16;
                }
                copy_size_of_tile = size_of_tile;
            case 3:
                pointer_to_color++;
                if(pointer_to_color>=size_of_selected_colors){
                    pointer_to_color = 0;
                }
                snake_1_color = selected_colors[pointer_to_color];
                break;

            case 4:
                pointer_to_color2++;
                if(pointer_to_color2>=size_of_selected_colors){
                    pointer_to_color2 = 0;
                }
                snake_2_color = selected_colors[pointer_to_color2];
                break;
            case 5:
                return_value = 1;
                break;
        }
        printf("%d\n",pointer_to_item);
    }
    return return_value;
}



void Menu_Options::options_fill_array(uint16_t* fb, unsigned char *parlcd_mem_base,  Game_Properties gp){
    uint16_t colors[2] = {BASE_COLOR_O, BASE_COLOR_O};
    colors[pointer_to_item%2] = SELECT_COLOR_O;

    //GAME OPTIONS
    int xTile = STARTX_1_O, yTile = 5;
    char str[]="OPTIONS";
    int size_of_str = 0;
    while (str[size_of_str] != 0){size_of_str++;}
    char *begin = str;
    draw_rect(STARTX_O, 0, WIDTH_O, HEIGHT_O, 3*BORDER_O, BASE_COLOR_O, fb);
    draw_string(xTile, yTile, BASE_COLOR_O, begin, size_of_str, fb);
    if(pointer_to_item<=2){
            int xTile = STARTX_1_O, yTile = STARTY_1_O;
            char str[]="SPEED:  ";
            int size_of_str = 0;
            while (str[size_of_str] != 0){size_of_str++;}
            str[size_of_str-1] = copy_speed + '0';
            char *begin = str;
            draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
            draw_rect(STARTX_O, STARTY_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[1], fb);
            xTile = STARTX_1_O; yTile = STARTY_2_O;
            char str2[] = "SIZE:";
            size_of_str = 0;
            while (str2[size_of_str] != 0){size_of_str++;}
            //sprintf(str2, "%d",copy_size_of_tile);
            begin = str2;
            draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
             xTile = STARTX_1_O+50; yTile = STARTY_2_O;
            char str3[10];
            sprintf(str3, "%d",copy_size_of_tile);
            size_of_str = 0;
            while (str3[size_of_str] != 0){size_of_str++;}
            begin = str3;
            draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
            draw_rect(STARTX_O, STARTY_O+HEIGHT_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[0], fb);
    }else if(pointer_to_item<=4){
            int xTile = STARTX_1_O, yTile = STARTY_1_O;
            char str[]="COLOR_1";
            int size_of_str = 0;
            while (str[size_of_str] != 0){size_of_str++;}
            char *begin = str;
            draw_filled_rect(STARTX_O+25, STARTY_O+25, 50, 50, selected_colors[pointer_to_color], fb);
            draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
            draw_rect(STARTX_O, STARTY_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[1], fb);
            xTile = STARTX_1_O; yTile = STARTY_2_O;
            char str2[] = "COLOR_2";
            size_of_str = 0;
            while (str2[size_of_str] != 0){size_of_str++;}
            begin = str2;
            draw_filled_rect(STARTX_O+25, STARTY_O+25+HEIGHT_O, 50, 50, selected_colors[pointer_to_color2], fb);
            draw_string(xTile, yTile, colors[0], begin, size_of_str, fb);
            draw_rect(STARTX_O, STARTY_O+HEIGHT_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[0], fb);
    }else if(pointer_to_item<=5){
            int xTile = STARTX_1_O, yTile = STARTY_1_O;
            char str[]="BACK";
            int size_of_str = 0;
            while (str[size_of_str] != 0){size_of_str++;}
            char *begin = str;
            draw_string(xTile, yTile, colors[1], begin, size_of_str, fb);
            draw_rect(STARTX_O, STARTY_O, WIDTH_O, HEIGHT_O, BORDER_O, colors[1], fb);
    }
}

