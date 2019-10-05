#ifndef BALL_H
#define BALL_H

#include "stm32f30x_conf.h"
#include "30010_io.h"

struct ball_t{
  int32_t x, y, vx, vy;
};

void initBall(struct ball_t *b, int32_t x0, int32_t y0, int32_t vx0, int32_t vy0);
void updatePosition(struct ball_t *b, int8_t del);
void drawPosition(struct ball_t *b);
int8_t collisionCheck(struct ball_t *b, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

#endif
