#ifndef _ANSI_H_
#define _ANSI_H_

void fgcolor(int foreground);
void bgcolor(int background);
void color(int foreground, int background);
void resetbgcolor();
void clrscr();
void clreol();
void gotoxy(int x, int y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void disableCursor(void);
void enableCursor(void);
void drawWindow(int x1, int y1, int x2, int y2, char* string, int style);

#endif /* _ANSI_H_ */
