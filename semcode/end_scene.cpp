#include "end_scene.h"  
  
/*

All described in a file end_scene.h

*/

void End_Scene::end_scene_update(){
    counter++;
    if(counter %2 == 0){
        moveY--;
        counter = 0;
    }
}
  
  
void End_Scene::end_scenefill_array(uint16_t* fb, unsigned char *parlcd_mem_base){
        int xTile = moveX, yTile = moveY;
        if(yTile<LCD_HEIGHT){
            char str[]="THE END";
            int size_of_str = 0;
            while (str[size_of_str] != 0){size_of_str++;}
            char *begin = str;
            draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);
        }
        xTile = moveX; yTile = moveY+30;
        if(yTile<LCD_HEIGHT){
            char str2[]="CREATED BY";
            int size_of_str = 0;
            while (str2[size_of_str] != 0){size_of_str++;}
            char *begin = str2;
            draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);
        }
        if(yTile<LCD_HEIGHT){
            xTile = moveX; yTile = moveY+60;
            char str3[]="KROTIMA1";
            int size_of_str = 0;
            while (str3[size_of_str] != 0){size_of_str++;}
            char *begin = str3;
            draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);
        }
        if(yTile<LCD_HEIGHT){
            xTile = moveX; yTile = moveY+90;
            char str4[]="POVOLJI2";
            int size_of_str = 0;
            while (str4[size_of_str] != 0){size_of_str++;}
            char *begin = str4;
            draw_string(xTile, yTile, BASE_COLOR, begin, size_of_str, fb);
        }
}