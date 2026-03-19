#include "stm32f10x.h"

volatile uint32_t rise_val = 0;
volatile uint32_t fall_val = 0;
volatile uint32_t throttle_raw = 0;
volatile uint8_t capturing_state = 0; // 0: Waiting for Rise, 1: Waiting for Fall

void TIM3_Config(void) {
    // 1. Clock Enable
    RCC->APB1ENR |= (1 << 1);  // TIM3 ON
    RCC->APB2ENR |= (1 << 2);  // GPIOA ON

    // 2. PA6 (TIM3_CH1) as Input Floating
    GPIOA->CRL &= ~(0xF << 24); 
    GPIOA->CRL |= (0x4 << 24); 

    // 3. Time Base: 1 tick = 1 microsecond
    TIM3->PSC = 71;             // 72MHz / 72 = 1MHz
    TIM3->ARR = 0xFFFF;         // 65535 tak ginti chalne do

    // 4. Input Capture Config
    TIM3->CCMR1 |= (1 << 0);    // CC1S = 01 (Input mode)
    TIM3->CCMR1 |= (0x3 << 4);  // IC1F = 0011 (Filter active karo noise ke liye)
    
    // 5. Interrupt Enable
    TIM3->DIER |= (1 << 1);     // CC1IE (Capture 1 Interrupt Enable)
    NVIC_EnableIRQ(TIM3_IRQn);  [span_0](start_span)// NVIC ko bolo TIM3 ki suno[span_0](end_span)

    // 6. Start Timer
    TIM3->CCER |= (1 << 0);     // CC1E = 1 (Enable Capture)
    TIM3->CR1 |= (1 << 0);      // CEN = 1 (Counter Start)
}

// Interrupt Handler: Jab bhi Receiver signal badlega, ye chalega
void TIM3_IRQHandler(void) {
    if (TIM3->SR & (1 << 1)) {  // Check agar CC1IF flag set hai
        if (capturing_state == 0) {
            // STEP 1: RISING EDGE AAYA
            rise_val = TIM3->CCR1;            // Shuruati ginti note karo
            TIM3->CCER |= (1 << 1);           // CC1P = 1 (Ab Falling edge dhundo)
            capturing_state = 1;              // State badlo
        } 
        else {
            // STEP 2: FALLING EDGE AAYA
            fall_val = TIM3->CCR1;            // Akhiri ginti note karo
            
            // Width Calculation
            if (fall_val > rise_val) {
                throttle_raw = fall_val - rise_val;
            } else {
                // Agar counter overflow ho gaya ho (Rare case)
                throttle_raw = (0xFFFF - rise_val) + fall_val;
            }
            
            TIM3->CCER &= ~(1 << 1);          // CC1P = 0 (Wapas Rising dhundo)
            capturing_state = 0;              // Reset state
        }
        TIM3->SR &= ~(1 << 1);  // Clear flag (Zaroori hai!)
    }
}
