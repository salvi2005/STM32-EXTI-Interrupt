⏱️ STM32 Timer Programming (Register-Level)

This module documents my deep exploration of STM32 timers using register-level programming.

Timers are among the most powerful peripherals in microcontrollers and are widely used in real-time embedded systems.

Typical applications include:

🚁 Drone flight controllers

🤖 Robotics systems

⚙️ Motor control

📡 Signal measurement

⏱️ Real-time scheduling

🎛️ Sensor timing

This repository focuses on understanding timers from hardware level without using HAL libraries.

📚 Topics Covered

This module covers the following concepts:

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

⚙️ Timer Architecture

STM32 timers internally follow a pipeline structure.

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

Explanation:

Block	Function
System Clock	Timer clock source
Prescaler (PSC)	Divides clock frequency
Counter (CNT)	Current counter value
Auto Reload Register (ARR)	Maximum counter value
Compare Register (CCR)	Used for PWM / events

The timer continuously counts clock pulses and generates events when CNT matches CCR or overflows ARR.

🧠 Important Timer Registers
Register	Description
CR1	Timer control register
PSC	Prescaler register
ARR	Auto reload register
CNT	Counter register
CCR1-CCR4	Compare registers
DIER	Interrupt enable register
SR	Status register

These registers control timer operation, interrupts, and PWM outputs.

⏲️ Timer Clock Calculation

Timer frequency is calculated as:

Timer Clock = Fclk / (PSC + 1)

Example:

Fclk = 72 MHz
PSC  = 71
Timer Clock = 72MHz / 72
Timer Clock = 1 MHz

Timer period formula:

T = (PSC + 1) × (ARR + 1) / Fclk

Example:

PSC = 71
ARR = 999

Result:

Timer Period = 1 ms
🌊 PWM Generation

PWM (Pulse Width Modulation) controls power using digital pulses.

Applications:

Motor speed control

LED brightness control

Servo control

Drone ESC control

Duty cycle formula:

Duty Cycle = CCR / ARR

Example:

ARR = 1000
CCR = 500
Duty Cycle = 50%
PWM Waveform
HIGH   ────────        ────────
        │               │
        │               │
LOW  ___│_______________│______
🧾 Register Level PWM Example
#include "stm32f10x.h"

void PWM_Init()
{
    // Enable GPIOA clock
    RCC->APB2ENR |= (1<<2);

    // Enable TIM2 clock
    RCC->APB1ENR |= (1<<0);

    // Configure PA0 as alternate function push-pull
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
🔔 Timer Interrupts

Timer interrupts allow periodic execution of code.

Flow:

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

Registers used:

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

        // periodic task here
    }
}
📡 Input Capture

Input capture measures timing of external signals.

Applications:

PWM measurement

Frequency measurement

Ultrasonic sensors

RC receiver signals

Working principle:

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
🎮 RC Receiver Signals

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
🎯 Output Compare Mode

Output compare generates events when:

Counter == Compare Register

Example:

ARR = 1000
CCR1 = 500

Event occurs at:

500 timer ticks

Applications:

Precise timing events

Signal generation

Hardware scheduling

🔄 Encoder Interface Mode

STM32 timers can work as quadrature encoder decoders.

Connection:

Encoder A → CH1
Encoder B → CH2

The timer automatically detects:

Rotation direction

Position

Speed

Counter behavior:

Clockwise Rotation      → CNT++
Anti-clockwise Rotation → CNT--

Encoder modes:

Mode	Description
Mode 1	Count CH1 edges
Mode 2	Count CH2 edges
Mode 3	Count both edges

Mode 3 provides highest resolution.

🔧 Encoder Mode Example
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
🚁 Drone Firmware Context

Timers are heavily used in drone firmware.

Typical drone control pipeline:

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

⚡ Advanced PWM (TIM1 / TIM8)

Advanced timers provide motor-control features.

Features:

Complementary PWM

Dead Time Insertion

Center Aligned PWM

Complementary PWM

Used in motor driver circuits.

PWM   → High-side MOSFET
PWMN  → Low-side MOSFET

Waveform:

PWM   : |‾‾‾|____|‾‾‾|
PWMN  : ____|‾‾‾|____|
Dead Time

Dead time prevents MOSFET shoot-through.

PWM   : |‾‾‾|____|‾‾‾|
PWMN  : ____|‾‾‾|____|
        ↑
     Dead Time

Both MOSFETs are never ON simultaneously.

Center Aligned PWM

Normal PWM counting:

0 → ARR → reset

Center aligned PWM counting:

0 → ARR → 0

Benefits:

Lower switching noise

Better motor control

Used in BLDC motor drivers

🧠 Key Concepts Learned

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

📦 Repository

Complete register-level implementations for:

UART

SPI

I2C

Timers

are available in this repository:

👉

https://github.com/salvi2005/STM32
