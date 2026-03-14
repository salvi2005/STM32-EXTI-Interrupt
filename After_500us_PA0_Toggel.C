#include "stm32f10x.h"

void OC_Init()
{
    /*
    ----------------------------------
    STEP 1 : Enable clocks
    ----------------------------------
    */

    RCC->APB2ENR |= (1<<2);   // Enable GPIOA clock
    RCC->APB1ENR |= (1<<0);   // Enable TIM2 clock


    /*
    ----------------------------------
    STEP 2 : Configure PA0 as
    Alternate Function Output
    ----------------------------------
    */

    GPIOA->CRL &= ~(0xF<<0);  // Clear PA0 config
    GPIOA->CRL |= (0xB<<0);   // AF push-pull output


    /*
    ----------------------------------
    STEP 3 : Configure timer speed
    ----------------------------------

    System clock = 72 MHz
    Prescaler = 71
    Timer clock = 1 MHz
    */

    TIM2->PSC = 71;


    /*
    ----------------------------------
    STEP 4 : Set timer period
    ----------------------------------
    */

    TIM2->ARR = 1000;


    /*
    ----------------------------------
    STEP 5 : Set compare value
    ----------------------------------

    When counter reaches 500
    event will occur
    */

    TIM2->CCR1 = 500;


    /*
    ----------------------------------
    STEP 6 : Configure Output Compare Mode
    ----------------------------------

    Toggle output when compare match
    */

    TIM2->CCMR1 |= (3<<4);


    /*
    ----------------------------------
    STEP 7 : Enable channel output
    ----------------------------------
    */

    TIM2->CCER |= (1<<0);


    /*
    ----------------------------------
    STEP 8 : Start timer
    ----------------------------------
    */

    TIM2->CR1 |= 1;
}

int main()
{
    OC_Init();

    while(1)
    {

    }
}
