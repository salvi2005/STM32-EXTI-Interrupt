⏱️ STM32 TIMER PROGRAMMING (REGISTER LEVEL)

This module documents my deep exploration of STM32 Timers using register-level programming.

Timers are one of the most powerful peripherals in microcontrollers and are widely used in real-time embedded systems.

Applications

🚁 Drone flight controllers

🤖 Robotics

⚙️ Motor control

📡 Signal measurement

⏱️ Real-time scheduling

🎛️ Sensor timing

This project focuses on understanding STM32 timers from hardware level without using HAL libraries.

📚 TOPICS COVERED

This module covers the following timer concepts:

Timer Architecture

Timer Registers

Timer Clock Calculation

PWM Generation

Timer Interrupts

Input Capture

RC Receiver Signal Decoding

Output Compare Mode

Encoder Interface

Advanced PWM (TIM1 / TIM8)

⚙️ TIMER ARCHITECTURE

STM32 timers internally follow this pipeline.

System Clock
     │
     ▼
Prescaler (PSC)
     │
     ▼
Counter (CNT)
     │
     ▼
Auto Reload Register (ARR)
     │
     ▼
Compare Register (CCR)
     │
     ▼
Event / PWM / Interrupt
Block Explanation
Block	Function
System Clock	Main clock source
Prescaler (PSC)	Divides the clock frequency
Counter (CNT)	Counts timer ticks
Auto Reload Register (ARR)	Maximum counter value
Compare Register (CCR)	Used for PWM and events

The timer counts clock pulses and generates events when conditions are met.

🧠 IMPORTANT TIMER REGISTERS
Register	Description
CR1	Timer control register
PSC	Prescaler register
ARR	Auto reload register
CNT	Counter register
CCR1–CCR4	Compare registers
DIER	Interrupt enable register
SR	Status register

These registers control timer operation, interrupts, and PWM output.

⏲️ TIMER CLOCK CALCULATION
Timer Frequency
Timer Clock = Fclk / (PSC + 1)

Example:

Fclk = 72 MHz
PSC  = 71
Timer Clock = 72MHz / 72
Timer Clock = 1 MHz
Timer Period
T = (PSC + 1) × (ARR + 1) / Fclk

Example:

PSC = 71
ARR = 999
Timer Period = 1 ms
🌊 PWM GENERATION

PWM (Pulse Width Modulation) controls power using digital pulses.

Applications

Motor speed control

LED brightness control

Servo control

Drone ESC control

Duty Cycle Formula
Duty Cycle = CCR / ARR

Example:

ARR = 1000
CCR = 500
Duty Cycle = 50%
PWM Waveform
HIGH  ────────      ────────
        │            │
        │            │
LOW  ___│____________│______
🧾 REGISTER LEVEL PWM EXAMPLE
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
    TIM2->PSC = 71;

    // Auto reload
    TIM2->ARR = 999;

    // PWM mode 1
    TIM2->CCMR1 |= (6<<4);

    // Enable channel
    TIM2->CCER |= 1;

    // Duty cycle
    TIM2->CCR1 = 500;

    // Start timer
    TIM2->CR1 |= 1;
}
🔔 TIMER INTERRUPTS

Timer interrupts allow periodic execution of code.

Interrupt Flow
Counter Overflow
       │
       ▼
Update Event
       │
       ▼
UIF Flag Set
       │
       ▼
NVIC Interrupt
       │
       ▼
ISR Executed
Interrupt Registers
Register	Function
DIER	Enable interrupt
SR	Interrupt flag
NVIC	CPU interrupt controller
Interrupt Example
void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<0))
    {
        TIM2->SR &= ~(1<<0); 

        // periodic task
    }
}
📡 INPUT CAPTURE

Input capture measures timing of external signals.

Applications

PWM measurement

Frequency measurement

Ultrasonic sensors

RC receiver signals

Working Principle
External Signal
       │
       ▼
Edge Detection
       │
       ▼
Counter Value Captured
       │
       ▼
Stored in CCR Register
🎮 RC RECEIVER SIGNALS

Typical RC PWM signals:

Pulse Width	Meaning
1000 µs	Minimum
1500 µs	Neutral
2000 µs	Maximum
Drone Channels
Channel	Function
CH1	Roll
CH2	Pitch
CH3	Throttle
CH4	Yaw
🎯 OUTPUT COMPARE MODE

Output compare generates events when:

Counter == Compare Register

Example:

ARR = 1000
CCR1 = 500
Event occurs at 500 timer ticks

Applications:

Precise timing events

Signal generation

Hardware scheduling

🔄 ENCODER INTERFACE MODE

STM32 timers can work as quadrature encoder decoders.

Encoder Connections
Encoder A → CH1
Encoder B → CH2

The timer automatically detects:

Direction

Position

Speed

Counter Behavior
Clockwise Rotation      → CNT++
Anti-clockwise Rotation → CNT--
Encoder Modes
Mode	Description
Mode 1	Count CH1 edges
Mode 2	Count CH2 edges
Mode 3	Count both edges

Mode 3 provides highest resolution.

🚁 DRONE FIRMWARE CONTEXT

Timers are heavily used in drone firmware.

Drone Control Pipeline
RC Receiver
     │
     ▼
Flight Controller (STM32)
     │
     ▼
PID Controller
     │
     ▼
PWM Output
     │
     ▼
ESC
     │
     ▼
BLDC Motor

Timers handle:

Motor PWM control

RC signal decoding

Control loop timing

⚡ ADVANCED PWM (TIM1 / TIM8)

Advanced timers support motor control features.

Features

Complementary PWM

Dead Time Insertion

Center Aligned PWM

Complementary PWM
PWM   → High-side MOSFET
PWMN  → Low-side MOSFET

Waveform:

PWM   : |‾‾‾|____|‾‾‾|
PWMN  : ____|‾‾‾|____|
Dead Time

Prevents MOSFET shoot-through.

PWM   : |‾‾‾|____|‾‾‾|
PWMN  : ____|‾‾‾|____|
        ↑
     Dead Time
Center Aligned PWM

Normal PWM:

0 → ARR → reset

Center aligned PWM:

0 → ARR → 0

Benefits:

Lower noise

Better motor control

Used in BLDC control

🧠 KEY CONCEPTS LEARNED

This module covers:

Timer architecture

Timer registers

PWM generation

Timer interrupts

Input capture

RC receiver decoding

Output compare

Encoder interface

Advanced PWM

📦 REPOSITORY

Complete register-level implementations for:

UART

SPI

I2C

Timers

Available here:

https://github.com/salvi2005/STM32
