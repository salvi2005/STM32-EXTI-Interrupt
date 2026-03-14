#include "stm32f10x.h"

volatile uint32_t ch1=0;
volatile uint32_t ch2=0;
volatile uint32_t ch3=0;
volatile uint32_t ch4=0;

volatile uint32_t last_capture1=0;
volatile uint32_t last_capture2=0;
volatile uint32_t last_capture3=0;
volatile uint32_t last_capture4=0;

void RC_Init()
{
    RCC->APB1ENR |= (1<<0);  // TIM2 clock
    RCC->APB2ENR |= (1<<2);  // GPIOA clock

    // PA0-PA3 input
    GPIOA->CRL = 0x4444;

    TIM2->PSC = 71;        // 1 MHz timer
    TIM2->ARR = 0xFFFF;

    // Input capture mode
    TIM2->CCMR1 = 0x0101;
    TIM2->CCMR2 = 0x0101;

    // Enable capture channels
    TIM2->CCER = 0x1111;

    // Enable interrupts
    TIM2->DIER = 0x1E;

    NVIC_EnableIRQ(TIM2_IRQn);

    TIM2->CR1 = 1;
}

void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<1))
    {
        TIM2->SR &= ~(1<<1);

        uint32_t capture = TIM2->CCR1;
        ch1 = capture - last_capture1;
        last_capture1 = capture;
    }

    if(TIM2->SR & (1<<2))
    {
        TIM2->SR &= ~(1<<2);

        uint32_t capture = TIM2->CCR2;
        ch2 = capture - last_capture2;
        last_capture2 = capture;
    }

    if(TIM2->SR & (1<<3))
    {
        TIM2->SR &= ~(1<<3);

        uint32_t capture = TIM2->CCR3;
        ch3 = capture - last_capture3;
        last_capture3 = capture;
    }

    if(TIM2->SR & (1<<4))
    {
        TIM2->SR &= ~(1<<4);

        uint32_t capture = TIM2->CCR4;
        ch4 = capture - last_capture4;
        last_capture4 = capture;
    }
}

int main()
{
    RC_Init();

    while(1)
    {

    }
}
