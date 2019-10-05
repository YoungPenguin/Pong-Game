#ifndef DISP_H
#define DISP_H
extern uint8_t dispbuffer[512];
lcd_write_string1(uint8_t *buffer, char *str, uint8_t slice, uint8_t line);
void sethighscorestring(int h);





#endif
