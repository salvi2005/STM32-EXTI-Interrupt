#include "stm32f10x.h"

void TIM2_Init()
{
    // Enable clock for TIM2
    RCC->APB1ENR |= (1<<0);

    // Prescaler
    TIM2->PSC = 7199;

    // Auto reload
    TIM2->ARR = 9999;

    // Enable update interrupt
    TIM2->DIER |= (1<<0);

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);

    // Start timer
    TIM2->CR1 |= (1<<0);
}

void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<0)) // Check update interrupt flag
    {
        TIM2->SR &= ~(1<<0); // Clear interrupt flag

        // Your periodic task here
    }
}

int main()
{
    TIM2_Init();

    while(1)
    {

    }
}
