#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "ansi.h"
#include "math.h"
#include "HAL.h"
#include <string.h>

#define TERM_X 90
#define TERM_Y 30
#define MAX_BOX 64
typedef struct Striker{
  vector_t posLeft;
  uint8_t zoneSize, speed;
} striker_t;

typedef struct Box{
  vector_t leftTopPos;
  uint8_t width, height, hitpoints, id;
} box_t;

typedef struct Ball{
  vector_t pos, vel;
  int32_t speed, angle;
} ball_t;

typedef struct GameState{
  uint8_t state; // 0x00 = Main Menu, 0x01 = Leaderboard, 0x02 = Help, 0x03 = Game, 0x04 = Boss-key (paused)
  uint8_t points;
  int8_t lives;
  uint8_t grid[TERM_Y+1][TERM_X+1]; //Point 0,0 not used. 0x00 = blank, 0x01 = wall, 0x02 = striker, 0x03 = ball, rest are box id
  box_t boxes[MAX_BOX];
  striker_t striker;
} game_state_t;

void initGame(game_state_t *game, uint8_t state, uint8_t points, int8_t lives);
void initStriker(striker_t *striker, game_state_t *game, uint32_t x0, uint32_t y0, uint8_t zoneSize, uint8_t speed);
void initBox(game_state_t *game, uint32_t x0, uint32_t y0, uint8_t width, uint8_t height, uint8_t id, uint8_t hitpoints);
void initBall(ball_t *ball, game_state_t *game, uint32_t x0, uint32_t y0, int32_t speed, int32_t angle);
void drawBox(box_t *box, game_state_t *game);
void drawStriker(striker_t *striker);
void drawBall(ball_t *ball);
int8_t collisionCheck(ball_t *ball, game_state_t *state);
void updateBallPosition(ball_t *ball, game_state_t *game);
void updateStriker(game_state_t *game, uint8_t joyIn, uint8_t keyIn);
void spawnBoxes(game_state_t *game, uint8_t rows, uint8_t nrow, uint8_t width, uint8_t height);
void drawBoxes(game_state_t *game);
void updateHighScores(game_state_t *game, char names[5][4], uint8_t scores[5]);
#endif
