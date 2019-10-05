#include "HAL.h"
void lcd_write_string(uint8_t* buffer, char* str, uint8_t slice, uint8_t line)
{
    //128 slices, 4 lines. 5 slices pr. character.
    int j = 0;
    while(str[j] != 0x00)
    {
        int8_t i;
        for(i = 0; i < 5; i++)
        {
            buffer[slice + (128 * line) + i + 5 * j ] = character_data[str[j] - 0x20][i];
        }
        j++;
    }
}

void lcd_update(uint8_t* buffer, uint8_t* flag)
{
    if(*flag)
    {
        lcd_push_buffer(buffer);
        *flag = 0;
    }
}

void lcd_scrollRightLeft(uint8_t* buffer)
{
    int8_t i, j;
    for(i = 0; i < 4; i++)
    {
        uint8_t temp = buffer[i * 128];
        for(j = 0; j < 127; j++)
        {
            buffer[j + i * 128] = buffer[j + 1 + i * 128];
        }
        buffer[127 + i * 128] = temp;
    }
}

uint8_t readJoystick(void)
{
    uint8_t value = 0x00;
    //Up
    if((GPIOA->IDR & (0x0001 << 4))) value |= 0x01;
    //Down
    if((GPIOB->IDR & (0x0001 << 0))) value |= 0x02;
    //Left
    if((GPIOC->IDR & (0x0001 << 1))) value |= 0x04;
    //Right
    if((GPIOC->IDR & (0x0001 << 0))) value |= 0x08;
    //Center
    if((GPIOB->IDR & (0x0001 << 5))) value |= 0x10;

    return value;
}

uint8_t readKeyboard(void)
{
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
    {
        switch((uint8_t) USART_ReceiveData(USART2))
        {
        case 119: //w
            return 0x01;
        case 97:  //a
            return 0x04;
        case 115: //s
            return 0x02;
        case 100: //d
            return 0x08;
        case 32: //space
            return 0x10;
        }
    }
    else
    {
        return 0x00;
    }
}

void disableOverrunDetection(void)
{
    USART2->CR1 &= ~0x00000001; // Disable USART2
    USART2->CR3 |=  0x00001000; // Set OVRDIS to 1;
    USART2->CR1 |=  0x00000001; // Enable USART2
}

void enableOverrunDetection(void)
{
    USART2->CR1 &= ~0x00000001; // Disable USART2
    USART2->CR3 &= ~0x00001000; // Set OVRDIS to 0;
    USART2->CR1 |=  0x00000001; // Enable USART2
}

void initJoystick(void)
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
}

void initLed(void)
{
    //Enable output on LED ports
    GPIOA->PUPDR &= ~(3 << (9 * 2));
    GPIOB->PUPDR &= ~(3 << (4 * 2));
    GPIOC->PUPDR &= ~(3 << (7 * 2));
    GPIOA->PUPDR |= (2 << (9 * 2));
    GPIOB->PUPDR |= (2 << (4 * 2));
    GPIOC->PUPDR |= (2 << (7 * 2));

    GPIOA->OTYPER &= ~(1 << 9);
    GPIOB->OTYPER &= ~(1 << 4);
    GPIOC->OTYPER &= ~(1 << 7);
    GPIOA->OTYPER |= (0 << 9);
    GPIOB->OTYPER |= (0 << 4);
    GPIOC->OTYPER |= (0 << 7);

    GPIOA->MODER &= ~(3 << (9 * 2));
    GPIOB->MODER &= ~(3 << (4 * 2));
    GPIOC->MODER &= ~(3 << (7 * 4));
    GPIOA->MODER |= (1 << (9 * 2));
    GPIOB->MODER |= (1 << (4 * 2));
    GPIOC->MODER |= (1 << (7 * 2));
}

void setLed(char color)
{
    //PA9 = Blue, PB4 = Red, PC7  = Green
    switch(color)
    {
    case 'r':
        GPIOA->ODR |=  (0x0001 << 9);
        GPIOB->ODR &= ~(0x0001 << 4);
        GPIOC->ODR |=  (0x0001 << 7);
        break;
    case 'g':
        GPIOA->ODR |=  (0x0001 << 9);
        GPIOB->ODR |=  (0x0001 << 4);
        GPIOC->ODR &= ~(0x0001 << 7);
        break;
    case 'b':
        GPIOA->ODR &= ~(0x0001 << 9);
        GPIOB->ODR |=  (0x0001 << 4);
        GPIOC->ODR |=  (0x0001 << 7);
        break;
    case 'c':
        GPIOA->ODR &= ~(0x0001 << 9);
        GPIOB->ODR |=  (0x0001 << 4);
        GPIOC->ODR &= ~(0x0001 << 7);
        break;
    case 'm':
        GPIOA->ODR &= ~(0x0001 << 9);
        GPIOB->ODR &= ~(0x0001 << 4);
        GPIOC->ODR |=  (0x0001 << 7);
        break;
    case 'y':
        GPIOA->ODR |=  (0x0001 << 9);
        GPIOB->ODR &= ~(0x0001 << 4);
        GPIOC->ODR &= ~(0x0001 << 7);
        break;
    case 'w':
        GPIOA->ODR &= ~(0x0001 << 9);
        GPIOB->ODR &= ~(0x0001 << 4);
        GPIOC->ODR &= ~(0x0001 << 7);
        break;
    case 'k':
        GPIOA->ODR |= (0x0001 << 9);
        GPIOB->ODR |= (0x0001 << 4);
        GPIOC->ODR |= (0x0001 << 7);
    }
}

void initADC(void)
{
    GPIOA->PUPDR &= ~(3 << (0 * 2));
    GPIOA->PUPDR |= (2 << (0 * 2));
    GPIOA->MODER &= ~(3 << (0 * 2));
    GPIOA->MODER |= (0 << (0 * 2));

    GPIOA->PUPDR &= ~(3 << (1 * 2));
    GPIOA->PUPDR |= (2 << (1 * 2));
    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |= (0 << (1 * 2));

    RCC->CFGR2 &= ~RCC_CFGR2_ADCPRE12; // Clear ADC12 prescaler bits
    RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_DIV6; // Set ADC12 prescaler to 6
    RCC->AHBENR |= RCC_AHBPeriph_ADC12;

    ADC1->CR = 0x00000000; // Clear CR register
    ADC1->CFGR &= 0xFDFFC007; // Clear ADC1 config register
    ADC1->SQR1 &= ~ADC_SQR1_L; // Clear regular sequence register 1

    ADC1->CR |= 0x10000000; // Enable internal ADC voltage regulator
    for (int i = 0 ; i < 1000 ; i++) {} // Wait for about 16 microsecs

    ADC1->CR |= 0x80000000; // Start ADC1 calibration
    while (!(ADC1->CR & 0x80000000)); // Wait for calibration to finish
    for (int i = 0 ; i < 100 ; i++) {} // Wait for a little while

    ADC1->CR |= 0x00000001; // Enable ADC1 (0x01 - Enable, 0x02 - Disable)
    while (!(ADC1->ISR & 0x00000001)); // Wait until ready
}

void initTimer(void)
{
    RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable clock line to timer 2;
    TIM15->CR1 = 0x0000;
    TIM15->ARR = 63999; // Set auto reload value (1ms)
    TIM15->PSC = 0; // Set pre-scaler value
    TIM15->DIER |= 0x0001; // Enable timer interrupt
    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0);
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
}

//5 names of 3 chars + null;
void writeScoresToFlashMem(char names[5][4], uint8_t scores[5])
{
    uint32_t address = 0x0800F800; //last page
    uint8_t i, j;
    FLASH_Unlock();
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage( address );
    for ( i = 0 ; i < 5 ; i++ )
    {
        for ( j = 0 ; j < 4; j++ )
        {
            FLASH_ProgramHalfWord(address + (i*4+j)* 2, (uint16_t) names[i][j]);
        }
    }
    for(i = 0; i < 5; i++)
    {
        FLASH_ProgramHalfWord(address + (5 * 4)*2 + i*2, (uint16_t) scores[i]);
    }
    FLASH_Lock();
}

void readScoresFromFlashMem(char names[5][4], uint8_t scores[5])
{
    uint32_t address = 0x0800F800; //last page
    uint8_t i, j;
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 4; j++)
        {
            names[i][j] = (char) *(uint16_t*)(address + (i * 4 + j) * 2);
        }
    }
    for(i = 0; i < 5; i++)
    {
        scores[i] = (uint8_t) *(uint16_t*)(address + (5 * 4)*2 + i*2);
    }
}

void getNBytes(char* arr, int8_t n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        *arr++ = uart_getc();
        if(*(arr-1) == 0x0D) *(arr-1) = 0x20;
    }
    *arr = 0x00;
}

void getBytes(char* arr)
{
    *arr++ = uart_getc();
    while(*(arr-1) != 0x0D)
    {
        *arr++ = uart_getc();
    }
    *(arr-1) = 0x00;
}

void initBuzzer(void)
{
    RCC->APB1ENR |= 0x00000001; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000; // Disable timer
    TIM2->ARR = 1000; // Set auto reload value
    TIM2->PSC = 99; // Set pre-scaler value
    TIM2->CR1 |= 0x0001; // Enable timer

    TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
    TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
    TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
    TIM2->CCMR2 |= TIM_OCPreload_Enable;
    TIM2->CCR3 = 500; // Set duty cycle to 50 %

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable clock line for GPIO bank B
    GPIOB->MODER &= ~(0x00000003 << (10 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000002 << (10 * 2)); // Set mode register

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);
}

void setFreq(uint16_t freq)
{
    uint32_t reload = 64e6 / freq / (99 + 1) - 1;
    TIM2->ARR = reload; // Set auto reload value
    TIM2->CCR3 = reload/2; // Set compare register
    TIM2->EGR |= 0x01;
}
