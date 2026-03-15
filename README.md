This module documents my deep exploration of STM32 Timers using register-level programming.

Timers are one of the most powerful peripherals in microcontrollers and are used in:

Motor control

Robotics

Flight controllers

Signal measurement

Real-time systems

Sensor timing

This module covers:

Timer architecture

PWM generation

Timer interrupts

Input capture

RC receiver decoding

Output compare

Encoder interface

Advanced PWM

Timer Architecture

Timers internally follow this pipeline:

System Clock
     ↓
Prescaler (PSC)
     ↓
Counter (CNT)
     ↓
Auto Reload Register (ARR)
     ↓
Compare Register (CCR)
     ↓
Event / PWM / Interrupt

The timer counts clock pulses and generates events when conditions are met.

Important Timer Registers
Register	Description
CR1	Timer control register
PSC	Prescaler register
ARR	Auto reload register
CNT	Counter register
CCR1-CCR4	Compare registers
DIER	Interrupt enable register
SR	Status register
Timer Clock Calculation

Timer frequency:

Timer Clock = Fclk / (PSC + 1)

Example:

Fclk = 72 MHz
PSC = 71

Timer Clock = 72MHz / 72
Timer Clock = 1 MHz

Timer period:

T = (PSC + 1) × (ARR + 1) / Fclk

Example:

PSC = 71
ARR = 999

Timer Period = 1 ms
PWM Generation

PWM (Pulse Width Modulation) controls power using digital signals.

Applications:

Motor speed control

LED brightness

Servo control

ESC control in drones

Duty cycle formula:

Duty Cycle = CCR / ARR

Example:

ARR = 1000
CCR = 500

Duty Cycle = 50%
PWM Register Level Example
#include "stm32f10x.h"

void PWM_Init()
{
    // Enable GPIOA clock
    RCC->APB2ENR |= (1<<2);

    // Enable TIM2 clock
    RCC->APB1ENR |= (1<<0);

    // PA0 as alternate function push-pull
    GPIOA->CRL &= ~(0xF<<0);
    GPIOA->CRL |= (0xB<<0);

    // Prescaler
    TIM2->PSC = 71;     // Timer clock = 1MHz

    // Auto reload
    TIM2->ARR = 999;    // PWM frequency = 1kHz

    // PWM mode 1
    TIM2->CCMR1 |= (6<<4);

    // Enable channel
    TIM2->CCER |= 1;

    // Duty cycle
    TIM2->CCR1 = 500;

    // Start timer
    TIM2->CR1 |= 1;
}
Timer Interrupt

Timer interrupts allow periodic execution of code.

Flow:

Counter overflow
↓
Update event
↓
UIF flag set
↓
NVIC interrupt
↓
ISR executed

Registers used:

Register	Function
DIER	Enable interrupt
SR	Interrupt flag
NVIC	Enable CPU interrupt
Timer Interrupt Example
void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<0))
    {
        TIM2->SR &= ~(1<<0); // clear flag

        // periodic task here
    }
}
Input Capture

Input capture measures timing of external signals.

Examples:

PWM measurement

Frequency measurement

Ultrasonic sensors

RC receiver signals

Working principle:

External signal
↓
Edge detection
↓
Counter value captured
↓
Stored in CCR register
RC Receiver Signal

Typical RC PWM signals:

Pulse Width	Meaning
1000 µs	Minimum
1500 µs	Neutral
2000 µs	Maximum

Drone control channels:

Channel	Function
CH1	Roll
CH2	Pitch
CH3	Throttle
CH4	Yaw
Output Compare Mode

Output compare triggers events when:

Counter == Compare register

Example:

ARR = 1000
CCR1 = 500

Event occurs at:

500 timer ticks

Applications:

Precise timing events

Signal generation

Hardware scheduling

Encoder Interface Mode

Timers can act as quadrature decoders for rotary encoders.

Inputs:

Encoder A → CH1
Encoder B → CH2

The timer automatically detects:

Direction

Position

Speed

Counter behavior:

Clockwise rotation → CNT++
Anti-clockwise rotation → CNT--

Encoder modes:

Mode	Description
Mode1	Count CH1 edges
Mode2	Count CH2 edges
Mode3	Count both edges

Mode3 gives highest resolution.

Encoder Mode Register Example
void Encoder_Init()
{
    RCC->APB2ENR |= (1<<2);
    RCC->APB1ENR |= (1<<0);

    // PA0 PA1 input
    GPIOA->CRL &= ~(0xFF);
    GPIOA->CRL |= 0x44;

    // Configure channels as input
    TIM2->CCMR1 |= (1<<0);
    TIM2->CCMR1 |= (1<<8);

    // Encoder mode 3
    TIM2->SMCR |= (3<<0);

    TIM2->ARR = 0xFFFF;

    TIM2->CR1 |= 1;
}
Drone Firmware Context

In drones timers are used for:

PWM motor control
RC receiver input capture
Control loop timing

Typical control flow:

RC Receiver
↓
Flight Controller (STM32)
↓
PID Controller
↓
PWM Output
↓
ESC
↓
BLDC Motor
Advanced PWM

Advanced timers like TIM1 and TIM8 support advanced PWM features.

Features:

Complementary PWM

Dead time insertion

Center aligned PWM

Complementary PWM

Used in motor driver circuits.

PWM   → High side MOSFET
PWMN  → Low side MOSFET
Dead Time

Prevents MOSFET shoot-through.

PWM   : |‾‾‾|____|‾‾‾|
PWMN  : ____|‾‾‾|____|

Dead time ensures both switches never turn ON simultaneously.

Center Aligned PWM

Normal PWM:

0 → ARR → reset

Center aligned PWM:

0 → ARR → 0

Benefits:

Lower noise

Better motor control

Used in BLDC control

Key Concepts Learned

This module covered:

Timer architecture
Timer registers
PWM generation
Timer interrupts
Input capture
RC receiver decoding
Output compare
Encoder interface
Advanced PWM
Repository

Complete UART, SPI and I2C register-level implementations are available in my repository:

salvi2005/STM32
