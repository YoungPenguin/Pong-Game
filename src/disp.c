#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "disp.h"
#include "charset.h"
#include <string.h>





lcd_write_string1(uint8_t *buffer, char *str, uint8_t slice, uint8_t line){
    int j=0;
    while(str[j] != 0x00){
    int8_t i;
    for(i =0; i < 5; i++){
    buffer[slice + (128 * line) + i +5 * j] = character_data[str[j] - 0x20 ][i];
    }
    j++;
    }
}
//denne funktion, bruges til at omdanne en tal variable (h) til en string som kan printes med "sethighscorestring(highscore);"
uint8_t dispbuffer[512] = {0};
void sethighscorestring(int h){
char highscorestring[4];
sprintf (highscorestring, "%d",h);

lcd_write_string1(dispbuffer,highscorestring,1,1);
lcd_push_buffer(dispbuffer);
}



