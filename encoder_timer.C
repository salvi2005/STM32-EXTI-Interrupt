#include "stm32f10x.h"

void Encoder_Init()
{

    /*
    ---------------------------------------
    STEP 1 : Enable clocks
    ---------------------------------------
    */

    RCC->APB2ENR |= (1<<2);   // Enable GPIOA clock
    RCC->APB1ENR |= (1<<0);   // Enable TIM2 clock


    /*
    ---------------------------------------
    STEP 2 : Configure encoder pins
    PA0 = CH1
    PA1 = CH2
    ---------------------------------------
    */

    GPIOA->CRL &= ~(0xFF);   // Clear config
    GPIOA->CRL |= 0x44;      // Floating input


    /*
    ---------------------------------------
    STEP 3 : Configure timer channels
    as input capture
    ---------------------------------------
    */

    TIM2->CCMR1 |= (1<<0);   // CH1 input
    TIM2->CCMR1 |= (1<<8);   // CH2 input


    /*
    ---------------------------------------
    STEP 4 : Enable encoder mode
    ---------------------------------------
    */

    TIM2->SMCR |= (3<<0);    // Encoder mode 3


    /*
    ---------------------------------------
    STEP 5 : Set counter limit
    ---------------------------------------
    */

    TIM2->ARR = 0xFFFF;


    /*
    ---------------------------------------
    STEP 6 : Start timer
    ---------------------------------------
    */

    TIM2->CR1 |= 1;

}

int main()
{

    Encoder_Init();

    while(1)
    {

        uint32_t position = TIM2->CNT;

        // position variable contains encoder value

    }

}
