#define ESC 0x1B
#include "30010_io.h"

void fgcolor(int foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  int type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(int background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf("%c[%dm", ESC, background+40);
}

void color(int foreground, int background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
  int type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
  printf("%c[m", ESC);
}

void clrscr(){
    printf("%c[2J", ESC);
}

void clreol(){
    printf("%c[2K", ESC);
}

void gotoxy(int x, int y){
    printf("%c[%d;%dH", ESC, y, x);
}

void underline(uint8_t on){
    printf("%c[%dm", ESC, (on) ? 04 : 24);
}

void blink(uint8_t on){
    printf("%c[%dm", ESC, (on) ? 05 : 25);
}

void inverse(uint8_t on){
    printf("%c[%dm", ESC, (on) ? 07 : 27);
}

void disableCursor(void){
    printf("%c[?25l", ESC);
}

void enableCursor(void){
    printf("%c[?25h", ESC);
}

void drawWindow(int x1, int y1, int x2, int y2, char* string, int style){
    disableCursor();
    char  LEFT_TOP_CORNER, RIGHT_TOP_CORNER,
          LEFT_BOTTOM_CORNER, RIGHT_BOTTOM_CORNER,
          H_LINE, V_LINE,
          L_CONNECT, R_CONNECT;
    switch(style){
    case 0:
        LEFT_TOP_CORNER = 218;
        RIGHT_TOP_CORNER = 191;
        LEFT_BOTTOM_CORNER = 192;
        RIGHT_BOTTOM_CORNER = 217;
        H_LINE = 196;
        V_LINE = 179;
        L_CONNECT = 180;
        R_CONNECT = 195;
        break;
    default:
        LEFT_TOP_CORNER = 201;
        RIGHT_TOP_CORNER = 187;
        LEFT_BOTTOM_CORNER = 200;
        RIGHT_BOTTOM_CORNER = 188;
        H_LINE = 205;
        V_LINE = 186;
        L_CONNECT = 185;
        R_CONNECT = 204;
        break;
    }

    //print box of given size
    int i,j;
    for(i = 0; y1 + i <= y2; i++){
      gotoxy(x1, y1 + i);
      for(j = 0; x1 + j <= x2; j++){
        if(i==0){
          if(j==0) printf("%c", LEFT_TOP_CORNER);
          else if(j + x1 == x2) printf("%c", RIGHT_TOP_CORNER);
          else printf("%c",H_LINE);
        } else if(i + y1 == y2){
          if(j==0) printf("%c", LEFT_BOTTOM_CORNER);
          else if(j + x1 == x2) printf("%c", RIGHT_BOTTOM_CORNER);
          else printf("%c",H_LINE);
        } else {
          if(j == 0 || j + x1 == x2) printf("%c", V_LINE);
          else printf(" ");
        }
      }
      printf("\n");
    }

    //print "Window Title"
    if(style != 3){ //Style 3 simply disables printing window title.
      gotoxy(x1 + 2, y1);
      printf("%c",L_CONNECT);

      color(0,7);
      int k;
      for(k = 0; k + x1 <= x2 - 2; k++){
          if(string[k] == 0 || k + x1 == x2 - 2){
              color(7,0);
              printf("%c",R_CONNECT);
              break;
          }
          printf("%c", string[k]);
      }
    }
}
