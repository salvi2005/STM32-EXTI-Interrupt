#include "stm32f10x.h"

volatile uint32_t capture1=0;
volatile uint32_t capture2=0;

void TIM2_Init()
{
    RCC->APB1ENR |= (1<<0);  // Enable TIM2 clock
    RCC->APB2ENR |= (1<<2);  // Enable GPIOA clock

    // PA0 input
    GPIOA->CRL &= ~(0xF<<0);
    GPIOA->CRL |= (0x4<<0);

    TIM2->PSC = 71;
    TIM2->ARR = 0xFFFF;

    // Input capture mode
    TIM2->CCMR1 |= (1<<0);

    // Enable capture
    TIM2->CCER |= (1<<0);

    // Enable interrupt
    TIM2->DIER |= (1<<1);

    NVIC_EnableIRQ(TIM2_IRQn);

    TIM2->CR1 |= 1;
}

void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<1))
    {
        TIM2->SR &= ~(1<<1);

        capture1 = capture2;
        capture2 = TIM2->CCR1;

        uint32_t pulse = capture2 - capture1;
    }
}

int main()
{
    TIM2_Init();

    while(1)
    {

    }
}
