#ifndef _GAMEWINDOWS_H_
#define _GAMEWINDOWS_H_
#include "30010_io.h"
#include "ansi.h"
#include "gamelogic.h"

void drawLeaderboard(char names[5][4], uint8_t scores[5]);
int8_t navCount(int8_t navCon, int8_t val);
void draw_menu(int8_t navCon, int8_t val, game_state_t *game);
void drawMenu1(void);
void drawMenu2(void);
void drawMenu3(void);
void drawStart(void);
void drawHelp(void);
void drawNameInput(void);

#endif
