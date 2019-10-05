#ifndef HAL_H
#define HAL_H
#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "charset.h"

void lcd_write_string(uint8_t* buffer, char* str, uint8_t slice, uint8_t line);
void lcd_update(uint8_t* buffer, uint8_t* flag);
void lcd_scrollRightLeft(uint8_t* buffer);
uint8_t readJoystick(void);
uint8_t readKeyboard(void);
void disableOverrunDetection(void);
void enableOverrunDetection(void);
void initJoystick(void);
void initLed(void);
void setLed(char color);
void initADC(void);
void initTimer(void);
void writeScoresToFlashMem(char names[5][4], uint8_t scores[5]);
void readScoresFromFlashMem(char names[5][4], uint8_t scores[5]);
void getNBytes(char* arr, int8_t n);
void getBytes(char* arr);
void setFreq(uint16_t freq);
void initBuzzer(void);
#endif
