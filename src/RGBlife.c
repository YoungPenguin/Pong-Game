#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "RGBlife.h"

int8_t calclife()
{
// I denne metode bliver ens liv udregnet. variablen "life" bruges blandt andet til at bestemme farven der er vist på RGB-lcd

    int8_t life = 0x01;

    return life;
}

void ledLife(int8_t life)
{

//Denne funktion bestemmer farven på RGB. variablen"life" som kommer fra metoden calclife.
    switch(life)
    {
    case 0x01:
        GPIOB->ODR &=~(0x0001 << 4);
        GPIOC->ODR |=(0x0001 << 7);
        GPIOA->ODR |=(0x0001 << 9);
        //Red
        break;
    case 0x02:
        GPIOA->ODR |=  (0x0001 << 9);
        GPIOB->ODR &= ~(0x0001 << 4);
        GPIOC->ODR &= ~(0x0001 << 7);
        //yellow
        break;
    case 0x03:
        GPIOB->ODR |= (0x0001 << 4);
        GPIOC->ODR &=~ (0x0001 << 7);
        GPIOA->ODR |= (0x0001 << 9);
        //Green
        break;
    case 0x00:
        GPIOB->ODR |=(0x0001 << 4);
        GPIOC->ODR |= (0x0001 << 7);
        GPIOA->ODR |= (0x0001 << 9);
        //ingen farve
        break;
    default:
        GPIOB->ODR |=(0x0001 << 4);
        GPIOC->ODR |= (0x0001 << 7);
        GPIOA->ODR |= (0x0001 << 9);
    }
}
