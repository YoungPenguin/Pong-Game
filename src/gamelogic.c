#include "gamelogic.h"
#include "gamewindows.h"

void initStriker(striker_t *striker, game_state_t *game, uint32_t x0, uint32_t y0, uint8_t zoneSize, uint8_t speed)
{
    striker->posLeft.x = x0;
    striker->posLeft.y = y0;
    striker->zoneSize = zoneSize;
    striker->speed = speed;

    //Populate game_state with position
    uint8_t i;
    for(i = 0; i < zoneSize*5; i++)
    {
        game->grid[y0][i + x0] = 0x02;
    }
}

void drawStriker(striker_t *striker)
{
    char solidBox = 219;
    char str[striker->zoneSize*5 + 1];
    int i;
    for(i = 0; i < striker->zoneSize*5; i++)
    {
        str[i] = solidBox;
    }
    str[striker->zoneSize*5] = 0;
    gotoxy(striker->posLeft.x, striker->posLeft.y);
    printf("%s", str);
}

void updateStriker(game_state_t *game, uint8_t joyIn, uint8_t keyIn)
{
    char str[game->striker.zoneSize*5 + 1];
    uint8_t i;
    for(i = 0; i < game->striker.zoneSize*5; i++)
    {
        game->grid[game->striker.posLeft.y][i + game->striker.posLeft.x] = 0x00;
        str[i] = 0x20;
    }
    str[game->striker.zoneSize*5] = 0;
    gotoxy(game->striker.posLeft.x, game->striker.posLeft.y);
    printf("%s", str);

    switch(joyIn | keyIn)
    {
        //Left
    case 0x04:
        if(game->striker.posLeft.x - game->striker.speed <= 1) break;
        game->striker.posLeft.x -= game->striker.speed;
        break;
        //Right
    case 0x08:
        if(game->striker.posLeft.x + game->striker.speed + game->striker.zoneSize * 5 - 1 >= TERM_X) break;
        game->striker.posLeft.x += game->striker.speed;
        break;
    }

    //Populate game_state with position
    for(i = 0; i < game->striker.zoneSize*5; i++)
    {
        game->grid[game->striker.posLeft.y][i + game->striker.posLeft.x] = 0x02;
    }
}

void initBox(game_state_t *game, uint32_t x0, uint32_t y0, uint8_t width, uint8_t height, uint8_t id, uint8_t hitpoints)
{
    game->boxes[id].leftTopPos.x = x0;
    game->boxes[id].leftTopPos.y = y0;
    game->boxes[id].width = width;
    game->boxes[id].height = height;
    game->boxes[id].id = id;
    game->boxes[id].hitpoints = hitpoints;

    uint8_t i, j;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            game->grid[y0 + i][x0 + j] = id;
        }
    }
}

void drawBox(box_t *box, game_state_t *game)
{
    char charbox;
    char boxline[box->width + 1];
    uint8_t color;
    uint8_t i, j;

    switch(box->hitpoints)
    {
    case 1:
        color = 1; //red
        break;
    case 2:
        color = 9; //light red
        break;
    case 3:
        color = 11; //yellow
        break;
    case 4:
        color = 4; //blue
        break;
    default:
        color = 2; //green
    }

    if(box->hitpoints == 0)
    {
        charbox = 0x20;
        //remove boxID from game grid.
        for(i = 0; i < box->height; i++)
        {
            for(j = 0; j < box->width; j++)
            {
                game->grid[box->leftTopPos.y + i][box->leftTopPos.x + j] = 0x00;
            }
        }
    }
    else
    {
        charbox = 219;
    }

    fgcolor(color);
    for(i = 0; i < box->height; i++)
    {
        gotoxy(box->leftTopPos.x, box->leftTopPos.y + i);
        for(j = 0; j < box->width; j++)
        {
            boxline[j] = charbox;
        }
        boxline[box->width] = 0;
        printf("%s", boxline);
    }
    fgcolor(7);
}

void initBall(ball_t *ball, game_state_t *game, uint32_t x0, uint32_t y0, int32_t speed, int32_t angle512)
{
    speed <<= FIX14_SHIFT;
    ball->speed = speed;
    ball->angle = angle512;
    ball->pos.x = x0 << FIX14_SHIFT;
    ball->pos.y = y0 << FIX14_SHIFT;
    ball->vel.x = FIX14_MULT(speed, cos(angle512));
    ball->vel.y = FIX14_MULT(speed, sin(angle512));

    game->grid[y0][x0] = 0x03;
}

void drawBall(ball_t *ball)
{
    gotoxy(ball->pos.x >> FIX14_SHIFT, ball->pos.y >> FIX14_SHIFT);
    printf("o");
}

int8_t collisionCheck(ball_t *ball, game_state_t *state)
{
    int32_t temp_x = (ball->pos.x + FIX14_MULT(ball->speed, cos(ball->angle))) >> FIX14_SHIFT;
    int32_t temp_y = (ball->pos.y + FIX14_MULT(ball->speed, sin(ball->angle))) >> FIX14_SHIFT;

    //If said ball hits a box (ID above 0x03)
    if(state->grid[temp_y][temp_x] > 0x03)
    {
        uint8_t id = state->grid[temp_y][temp_x];
        //Top and bottom sides
        if(temp_y == state->boxes[id].leftTopPos.y || temp_y == state->boxes[id].leftTopPos.y + state->boxes[id].height - 1)
        {
            ball->angle = -ball->angle;
        }
        //else
        //Left and right sides
        if(temp_x == state->boxes[id].leftTopPos.x || temp_x == state->boxes[id].leftTopPos.x + state->boxes[id].width - 1)
        {
            ball->angle = (256 - ball->angle) % 512; //mod 512 to avoid getting extremely low.
        }
        if(state->boxes[id].hitpoints < 3)
        {
            state->boxes[id].hitpoints--;
        }
        else if(state->boxes[id].hitpoints == 4)
        {
            state->boxes[id].hitpoints = 0;
            state->striker.zoneSize *= 2;
            drawBox(&(state->boxes[id]), state);
            return 0; //no points for powerup
        }
        drawBox(&(state->boxes[id]), state);
        return 1; //returns one so you can increment points.
    }

    //Ball to the wall
    if(state->grid[temp_y][temp_x] == 0x01)
    {
        //Left and right walls
        if(temp_x == 1 || temp_x == TERM_X)
        {
            ball->angle = (256 - ball->angle) % 512;
        }
        //Top and bottom walls
        if(temp_y == 1 || temp_y == TERM_Y)
        {
            ball->angle = -ball->angle;
        }
        return temp_y == TERM_Y ? -1 : 2;
    }

    //Striker
    if(state->grid[temp_y][temp_x] == 0x02)
    {
        if(FIX14_MULT(ball->speed, cos(ball->angle)) != 0)
        {
            //Split in to 5 zones
            if(temp_x >= state->striker.posLeft.x && temp_x < state->striker.posLeft.x + state->striker.zoneSize)
            {
                //ball->angle = ball->vel.x > 0 ? -(128 - (128 - ball->angle) / 3) % 512 : -(128 - 3 * (128 - ball->angle)) % 512; //Division by 3 will result in non-int result. The lost precision doesn't really matter
                ball->angle = -ball->angle + (ball->vel.x > 0 ?  32 : -32);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize && temp_x < state->striker.posLeft.x + state->striker.zoneSize*2)
            {
                //ball->angle = ball->vel.x > 0 ? -(128 - 2 * (128 - ball->angle) / 3) % 512 : -(128 - 3 * (128 - ball->angle) / 2) % 512;
                ball->angle = -ball->angle + (ball->vel.x > 0 ?  16 : -16);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize*2 && temp_x < state->striker.posLeft.x + state->striker.zoneSize*3)
            {
                //ball->angle = -(128 - (128 - ball->angle)) % 512;
                ball->angle = -ball->angle;
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize*3 && temp_x < state->striker.posLeft.x + state->striker.zoneSize*4)
            {
                //ball->angle = ball->vel.x > 0 ? -(128 - 3 * (128 - ball->angle) / 2) % 512 : -(128 - 2 * (128 - ball->angle) / 3) % 512;
                ball->angle = -ball->angle + (ball->vel.x > 0 ?  -16 : 16);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize*4 && temp_x < state->striker.posLeft.x + state->striker.zoneSize*5)
            {
                //ball->angle = ball->vel.x > 0 ? -(128 - 3 * (128 - ball->angle)) % 512 : -(128 - (128 - ball->angle) / 3) % 512;
                ball->angle = -ball->angle + (ball->vel.x > 0 ?  -32 : 32);
            }
        }
        else
        {
            if(temp_x >= state->striker.posLeft.x && temp_x < state->striker.posLeft.x + state->striker.zoneSize)
            {
                ball->angle = -(128+32);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize && temp_x < state->striker.posLeft.x + state->striker.zoneSize*2)
            {
                ball->angle = -(128+16);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize*2 && temp_x < state->striker.posLeft.x + state->striker.zoneSize*3)
            {
                ball->angle = -(128);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize*3 && temp_x < state->striker.posLeft.x + state->striker.zoneSize*4)
            {
                ball->angle = -(128-16);
            }
            if(temp_x >= state->striker.posLeft.x + state->striker.zoneSize*4 && temp_x < state->striker.posLeft.x + state->striker.zoneSize*5)
            {
                ball->angle = -(128-32);
            }
        }
        if(ball->angle % 256 == 0)
        {
            ball->angle += FIX14_MULT(ball->speed, cos(ball->angle)) > 0 ? 32 : -32;
        }
        return 2;
    }
    return 0;
}

void updateBallPosition(ball_t *ball, game_state_t *game)
{
    if(game->grid[ball->pos.y >> FIX14_SHIFT][ball->pos.x >> FIX14_SHIFT] == 0x03)
    {
        game->grid[ball->pos.y >> FIX14_SHIFT][ball->pos.x >> FIX14_SHIFT] = 0x00;
        gotoxy(ball->pos.x >> FIX14_SHIFT, ball->pos.y >> FIX14_SHIFT);
        printf(" ");
    }
    ball->pos.x += FIX14_MULT(ball->speed, cos(ball->angle));
    ball->pos.y += FIX14_MULT(ball->speed, sin(ball->angle));
    if(game->grid[ball->pos.y >> FIX14_SHIFT][ball->pos.x >> FIX14_SHIFT] == 0x00)
    {
        game->grid[ball->pos.y >> FIX14_SHIFT][ball->pos.x >> FIX14_SHIFT] = 0x03;
    }
}

void initGame(game_state_t *game, uint8_t state, uint8_t points, int8_t lives)
{
    game->state  = state;
    game->points = points;
    game->lives  = lives;
    //Initialize fields to 0x01 for walls and 0x00 for the rest
    int i, j;
    for(i = 1; i <= TERM_Y; i++)
    {
        for(j = 1; j <= TERM_X; j++)
        {
            if(i == 1 || i == TERM_Y) game->grid[i][j] = 0x01;
            else if(j == 1 || j == TERM_X) game->grid[i][j] = 0x01;
            else game->grid[i][j] = 0x00;
        }
    }
    for(i = 0; i < MAX_BOX; i++)
    {
        game->boxes[i].id = 0x00;
    }
}
//OBS: Hardcoded to start drawing at row 12.
void spawnBoxes(game_state_t *game, uint8_t rows, uint8_t nrow, uint8_t width, uint8_t height)
{
    uint8_t i,j;
    uint8_t id;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < nrow; j++)
        {
            initBox(game, 2+j*width, 12+i*height, width, height, 0x04+id, 2);
            id++;
        }
    }
}

void drawBoxes(game_state_t *game)
{
    uint16_t i;
    for(i = 0; i < MAX_BOX; i++)
    {
        if(game->boxes[i].id != 0)
        {
            drawBox(&(game->boxes[i]), game);
        }
    }
}

void updateHighScores(game_state_t *game, char names[5][4], uint8_t scores[5])
{
    char str[4];
    uint8_t i;
    uint8_t p = 5; //defined out of bounds so nothing will happens if score isn't high enough
    //find index to replace
    for(i = 0; i < 5; i++)
    {
        if(scores[i] <= game->points)
        {
            p = i;
            break;
        }
    }
    //shift values above said index
    for(i = 4; i > p; i--)
    {
        scores[i] = scores[i-1];
        strcpy(names[i], names[i-1]);
    }
    //replace original index
    if(p < 5)
    {
        enableOverrunDetection();
        scores[p] = game->points;
        drawNameInput();
        getNBytes(str, 3);
        disableOverrunDetection();
        strcpy(names[p], str);
    }
}
