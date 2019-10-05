#include "gamewindows.h"

void drawLeaderboard(char names[5][4], uint8_t scores[5]){
  clrscr();
  drawWindow(1, 1, 17, 10, "Leaderboard", 1);
  gotoxy(4, 2);
  printf("1. %s - %d", names[0], scores[0]);
  gotoxy(4, 3);
  printf("2. %s - %d", names[1], scores[1]);
  gotoxy(4, 4);
  printf("3. %s - %d", names[2], scores[2]);
  gotoxy(4, 5);
  printf("4. %s - %d", names[3], scores[3]);
  gotoxy(4, 6);
  printf("5. %s - %d", names[4], scores[4]);
  gotoxy(5, 9);
  printf(">Return");
}
int8_t navCount(int8_t navCon, int8_t val) {
  switch (val) {
  case 0x02:
      navCon++;
      if(navCon>0x02){navCon=0x00;}
      return navCon;
  case 0x01:
      navCon--;
      if(navCon<0x00){navCon=0x02;}
      return navCon;
  default:
      return navCon;
  }
}
void draw_menu(int8_t navCon, int8_t val, game_state_t *game) {
 switch(navCon) {
    case 0x00:
       drawMenu1();
    break;
    case 0x01:
        drawMenu2();
        break;
    case 0x02:
        drawMenu3();
        break;
  }
  if(val == 0x10){
    // 0x00 = Main Menu, 0x01 = Leaderboard, 0x02 = Help, 0x03 = Game, 0x04 = Paused, 0x08 = Boss-key
    switch(navCon) {
    case 0x00:
      game->state = 0x03;
    break;
    case 0x01:
      game->state = 0x01;
      break;
    case 0x02:
      game->state = 0x02;
      break;
    }
  }
}
void drawMenu1(void){
  gotoxy(3, 2);
  printf(">Start");
  gotoxy(3, 3);
  printf(" Leaderboard");
  gotoxy(3, 4);
  printf(" Help");
}
void drawMenu2(void){
  gotoxy(3, 2);
  printf(" Start");
  gotoxy(3, 3);
  printf(">Leaderboard");
  gotoxy(3, 4);
  printf(" Help");
}
void drawMenu3(void){
  gotoxy(3, 2);
  printf(" Start");
  gotoxy(3, 3);
  printf(" Leaderboard");
  gotoxy(3, 4);
  printf(">Help");
}
void drawStart(void){
  clrscr();
  drawWindow(1, 1, 18, 5, " Main  Menu ", 1);
  gotoxy(3, 2);
  printf(">Start");
  gotoxy(3, 3);
  printf(" Leaderboard");
  gotoxy(3, 4);
  printf(" Help");
}
void drawHelp(void){
clrscr();
  drawWindow(1, 1, 45, 16, " User-manual ", 1);
  gotoxy(4, 2);
  printf("Reflect ball to break the boxes.");
  gotoxy(4, 4);
  printf("Use joystick or WASD to control striker");
  gotoxy(4, 6);
  printf("Break blue box to increase striker size");
  gotoxy(4, 7);
  printf("by a factor of 2");
  gotoxy(4, 9);
  printf("Up to 3 balls can be on the field.");
  gotoxy(4, 11);
  printf("Spawns a ball every 30s");
  gotoxy(4, 15);
  printf(">Return");
}
void drawNameInput(void){
  clrscr();
  drawWindow(1, 1, 30, 5, "Game Over!", 1);
  gotoxy(4, 3);
  printf("Enter your name: ___");
  gotoxy(21, 3);
}
