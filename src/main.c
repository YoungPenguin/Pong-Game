/*
**
**                           Main.c
*/
#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "math.h"
#include "ansi.h"
#include "gamewindows.h"
#include "gamelogic.h" //Includes TERM_X and TERM_Y definition
#include "HAL.h"
#include "RGBlife.h"
#include <string.h>
volatile uint8_t flag;


int main(void)
{
    //Init UART
    init_usb_uart(115200);

    //Init variables
    game_state_t game;
    ball_t ball[3];                                         //Up to 3 balls
    uint8_t lcd_buffer[512] = {0};                          //Empty LCD buffer
    char score[11];                                         //String for LCD. 10 chars + null character ("Score: ABC")
    char names[5][4] = {"aaa", "bbb", "ccc", "ddd", "eee"}; //Array of 5 strings with length 3 + null.
    uint8_t scores[5] = {0};                                //Array of 5 high scores.
    uint16_t cnt = 0;                                       //Primary counter for timekeeping. In milliseconds
    uint8_t prevState = 0x00;                               //Previous GameState
    uint8_t currBalls = 1;                                  //Initial amount of balls
    uint8_t prevJoy = 0x00;                                 //Previous joystick input
    uint8_t joyIn = 0x00;                                   //Current joystick input
    uint8_t keyIn = 0x00;                                   //Current keyboard input
    uint8_t BuzzOn = 0;                                     //Buzzer timer flag
    uint8_t BuzzCount = 0;                                  //Buzzer counter
    int8_t navCon = 0;                                      //Menu position
    uint8_t i;

    //Init hardware and clear PuTTY
    clrscr();
    initTimer();
    initJoystick();
    initLed();
    init_spi_lcd();
    initBuzzer();
    setFreq(0);
    setLed('k'); //turn off the LED
    lcd_push_buffer(lcd_buffer); //push empty buffer.
    disableOverrunDetection(); //for keyboard input. Wont hang if you hold down a key.
    //Init game instance.
    initGame(&game, 0x00, 0, 3);
    //writeScoresToFlashMem(names, scores); //uncomment to reset FLASH
    readScoresFromFlashMem(names, scores);
    //Draw main menu
    drawStart();
    //Start timer
    TIM15->CR1 = 0x0001;
    while(1)
    {
        switch(game.state)
        {
            // 0x00 = Main Menu, 0x01 = Leaderboard, 0x02 = Help, 0x03 = Game, 0x04 = Boss-Key (paused)
        case 0x00:
            keyIn = readKeyboard();
            joyIn = readJoystick();
            if(prevState != 0x00)
            {
                navCon = 0x00;
            }
            if(prevJoy != joyIn || keyIn != 0x00)
            {
                prevState = game.state;
                navCon = navCount(navCon, joyIn | keyIn);
                draw_menu(navCon, joyIn | keyIn, &game);
                prevJoy = joyIn;
                cnt = 0;
            }
            break;
        case 0x01:
            keyIn = readKeyboard();
            joyIn = readJoystick();
            if(prevState == 0x00)
            {
                drawLeaderboard(names, scores);
                prevState = game.state;
            }
            if(prevJoy != joyIn || keyIn != 0x00)
            {
                if(joyIn == 0x10 || keyIn == 0x10)
                {
                    drawStart();
                    game.state = 0x00;
                }
                prevJoy = joyIn;
            }
            break;
        case 0x02:
            keyIn = readKeyboard();
            joyIn = readJoystick();
            if(prevState == 0x00)
            {
                drawHelp();
                prevState = game.state;
            }
            if(prevJoy != joyIn || keyIn != 0x00)
            {
                if(joyIn == 0x10 || keyIn == 0x10)
                {
                    drawStart();
                    game.state = 0x00;
                }
                prevJoy = joyIn;
            }
            break;
        case 0x03:
            if(flag)
            {
                cnt++;
                flag = 0;
                TIM15->CNT = 0x0;
            } //increment counter and reset flag/TIM15.CNT
            //Update every 50ms
            if(cnt % 50 == 0)
            {
                keyIn = readKeyboard();
                joyIn = readJoystick();
                if((prevJoy != joyIn && joyIn == 0x10) || keyIn == 0x10)
                {
                    clrscr();
                    prevJoy = joyIn;
                    prevState = game.state;
                    game.state = 0x04;
                    break;
                }
                if(prevState == 0x00)
                {
                    initGame(&game, 0x03, 0, 3);
                    initStriker(&(game.striker), &game, TERM_X / 2, TERM_Y-1, TERM_X / 50, 1);
                    initBall(&(ball[0]), &game, TERM_X / 2, TERM_Y - 5, 0, 128);
                    initBall(&(ball[1]), &game, TERM_X / 2, TERM_Y - 5, 0, 128);
                    initBall(&(ball[2]), &game, TERM_X / 2, TERM_Y - 5, 0, 128);
                    spawnBoxes(&game, 3, 8, (TERM_X-2)/8, (TERM_Y-2*TERM_Y/3)/3);
                    initBox(&game, TERM_X / 2 - 4, 3, 4, 2, 60, 4); //4hp treated as a power up.
                    drawWindow(1, 1, TERM_X, TERM_Y, "", 3);
                    drawBoxes(&game);

                    ledLife((int8_t) game.lives);
                    sprintf(score,"Score: %d", game.points);
                    lcd_write_string(lcd_buffer, score, 1, 1);
                    lcd_push_buffer(lcd_buffer);
                }
                prevState = game.state;
                updateStriker(&game, joyIn, keyIn);
                drawStriker(&(game.striker));

                //increment points or decrement lives depending on return value of collisionCheck
                for(i = 0; i < currBalls; i++)
                {
                    switch(collisionCheck(&(ball[i]), &game))
                    {
                    case -1:
                        game.lives--;
                        setFreq(600);
                        BuzzOn = 2;
                        break;
                    case 1:
                        game.points++;
                        setFreq(1000);
                        BuzzOn = 1;
                        break;
                    case 2:
                        setFreq(2000);
                        BuzzOn = 1;
                        break;
                    }
                    updateBallPosition(&(ball[i]), &game);
                    drawBall(&(ball[i]));
                }

                //Update score on LCD and display life on RGB
                ledLife((int8_t) game.lives);
                sprintf(score, "Score: %d", game.points);
                lcd_write_string(lcd_buffer, score, 1, 1);
                lcd_push_buffer(lcd_buffer);

                //Print points and lives below game-screen
                gotoxy(1, TERM_Y + 1);
                printf("Score: %d", game.points);
                gotoxy(1, TERM_Y + 2);
                printf("Lives: %d", game.lives);

                if(BuzzOn) BuzzCount++;
                if(BuzzCount == 4 && BuzzOn == 2)
                {
                    setFreq(500);
                    BuzzCount = 0;
                    BuzzOn--;
                }
                if(BuzzCount == 4 && BuzzOn == 1)
                {
                    setFreq(0);
                    BuzzCount = 0;
                    BuzzOn = 0;
                }
                for(i = 0; i < currBalls; i++)
                {
                    if(cnt == 2000+i*1000)
                    {
                        ball[i].speed = ball[i].speed = 1 << (FIX14_SHIFT - 1); //Set speed after a bit.
                    }
                }
                if(cnt  == 30000)
                {
                    currBalls += currBalls < 3 ? 1 : 0;
                    cnt = 0; //reset count every 30s and spawn a ball
                }
                if(game.lives <= 0 && !BuzzOn)
                {
                    updateHighScores(&game, names, scores);
                    writeScoresToFlashMem(names, scores);
                    currBalls = 1;
                    prevState = game.state;
                    game.state = 0x01;
                    game.points = 0;
                    drawLeaderboard(names, scores);
                    memset(lcd_buffer, 0x00, 512);
                    lcd_push_buffer(lcd_buffer);
                }
                prevJoy = joyIn;
            }
            break;
        case 0x04:
            prevJoy = joyIn;
            keyIn = readKeyboard();
            joyIn = readJoystick();
            if((prevJoy != joyIn && joyIn == 0x10) || keyIn == 0x10)
            {
                drawWindow(1, 1, TERM_X, TERM_Y, "", 3);
                drawBoxes(&game);
                prevJoy = joyIn;
                prevState = game.state;
                game.state = 0x03;
            }
            break;
        }
    }
}

void TIM1_BRK_TIM15_IRQHandler()
{
    flag = 1;
    TIM15->SR &= ~0x0001;
}

