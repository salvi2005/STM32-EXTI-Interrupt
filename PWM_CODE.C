#include "stm32f10x.h"

void PWM_Init()
{
    /*
    ---------------------------------
    STEP 1 : Enable clocks
    ---------------------------------
    */

    RCC->APB2ENR |= (1<<2);   // Enable GPIOA clock
    RCC->APB1ENR |= (1<<0);   // Enable TIM2 clock


    /*
    ---------------------------------
    STEP 2 : Configure PA0 as AF Output
    ---------------------------------
    */

    GPIOA->CRL &= ~(0xF << 0);  // Clear bits for PA0
    GPIOA->CRL |= (0xB << 0);   // Alternate function push-pull


    /*
    ---------------------------------
    STEP 3 : Configure Timer Frequency
    ---------------------------------
    */

    TIM2->PSC = 71;   // Prescaler
                     // 72MHz / (71+1) = 1 MHz

    TIM2->ARR = 999;  // Auto reload
                     // 1 MHz / 1000 = 1 kHz PWM


    /*
    ---------------------------------
    STEP 4 : Configure PWM mode
    ---------------------------------
    */

    TIM2->CCMR1 &= ~(0xFF);

    TIM2->CCMR1 |= (6 << 4);  // PWM mode 1

    TIM2->CCMR1 |= (1 << 3);  // Enable preload


    /*
    ---------------------------------
    STEP 5 : Enable channel output
    ---------------------------------
    */

    TIM2->CCER |= (1 << 0);  // Enable CH1 output


    /*
    ---------------------------------
    STEP 6 : Set duty cycle
    ---------------------------------
    */

    TIM2->CCR1 = 500;   // 50% duty cycle


    /*
    ---------------------------------
    STEP 7 : Enable Timer
    ---------------------------------
    */

    TIM2->CR1 |= (1 << 0);   // Start timer
}

int main()
{
    PWM_Init();

    while(1)
    {

    }
}
