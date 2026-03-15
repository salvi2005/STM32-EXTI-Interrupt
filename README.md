# STM32 Timers (Register-Level Programming) ⏱️

This repository demonstrates **STM32 Timer programming using bare-metal register access** on the STM32F103 microcontroller.

The goal of this module is to explain **how STM32 timers work internally**, including:

• Timer initialization  
• Prescaler configuration  
• Counter operation  
• PWM generation  
• Timer interrupts  
• Input capture  
• Encoder interface  
• Advanced PWM features  

This type of implementation is useful for **embedded firmware engineers working without HAL libraries**.

---

# 1. What is a Timer?

A **Timer** is a hardware peripheral used to measure time and generate periodic events.

Timers are essential for **real-time embedded systems**.

Common applications include:

* Motor control  
* Robotics systems  
* Drone flight controllers  
* Signal measurement  
* Frequency generation  
* Sensor timing  

Timers operate by **counting clock pulses from the system clock**.

---

# 2. STM32 Timer Architecture

![Image](https://deepbluembedded.com/wp-content/uploads/2021/04/STM32-Timers-Block-Diagram.jpg)

![Image](https://controllerstech.com/wp-content/uploads/2021/06/STM32-Timer-Block-Diagram.png)

![Image](https://www.researchgate.net/publication/332421586/figure/fig1/AS:747543950688256@1555235120968/Block-diagram-of-the-timer-module-in-STM32-microcontrollers.png)

STM32 timers internally follow this pipeline:

```
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
```

Explanation:

| Block | Function |
|------|---------|
| System Clock | Timer clock source |
| Prescaler (PSC) | Divides clock frequency |
| Counter (CNT) | Counts timer ticks |
| Auto Reload Register (ARR) | Maximum counter value |
| Compare Register (CCR) | Used for PWM generation and events |

The timer continuously counts clock pulses and generates events when **counter reaches specific values**.

---

# 3. Timer Clock Calculation

Timer frequency is calculated using the prescaler.

Formula:

```
Timer Clock = Fclk / (PSC + 1)
```

Example:

```
Fclk = 72 MHz
PSC = 71
```

Result:

```
Timer Clock = 72 MHz / 72
Timer Clock = 1 MHz
```

---

# 4. Timer Period Calculation

Timer period depends on **PSC and ARR values**.

Formula:

```
T = (PSC + 1) × (ARR + 1) / Fclk
```

Example:

```
PSC = 71
ARR = 999
```

Result:

```
Timer Period = 1 ms
```

---

# 5. Important STM32 Timer Registers

| Register | Purpose |
|--------|---------|
| CR1 | Timer control register |
| PSC | Prescaler register |
| ARR | Auto reload register |
| CNT | Counter register |
| CCR1 – CCR4 | Compare registers |
| DIER | Interrupt enable register |
| SR | Status register |

These registers control **timer operation, PWM generation, and interrupts**.

---

# 6. PWM Generation

![Image](https://deepbluembedded.com/wp-content/uploads/2021/05/STM32-PWM-Signal.png)

![Image](https://upload.wikimedia.org/wikipedia/commons/0/0f/PWM_signal.svg)

PWM (**Pulse Width Modulation**) is used to control power using digital signals.

Applications:

* Motor speed control  
* LED brightness control  
* Servo motors  
* Drone ESC control  

Duty cycle formula:

```
Duty Cycle = CCR / ARR
```

Example:

```
ARR = 1000
CCR = 500
```

Result:

```
Duty Cycle = 50%
```

---

# 7. Timer Interrupts

![Image](https://www.researchgate.net/publication/344595352/figure/fig1/AS:946913808846849@1602926623846/Timer-interrupt-flow-diagram.png)

Timer interrupts allow the CPU to execute **periodic tasks automatically**.

Working flow:

```
Counter Overflow
↓
Update Event
↓
UIF Flag Set
↓
NVIC Interrupt
↓
ISR Executed
```

Registers used:

| Register | Function |
|-------|----------|
| DIER | Enable interrupt |
| SR | Interrupt flag |
| NVIC | CPU interrupt controller |

Example interrupt handler:

```c
void TIM2_IRQHandler()
{
    if(TIM2->SR & (1<<0))
    {
        TIM2->SR &= ~(1<<0);

        // periodic task
    }
}
```

---

# 8. Input Capture

![Image](https://www.researchgate.net/publication/327193211/figure/fig2/AS:664606737436672@1535443855673/Input-capture-mode.png)

Input capture measures **timing of external signals**.

Applications:

* PWM measurement  
* Frequency measurement  
* Ultrasonic sensors  
* RC receiver signals  

Working principle:

```
External Signal
↓
Edge Detection
↓
Counter Value Captured
↓
Stored in CCR Register
```

---

# 9. RC Receiver Signals

Typical RC receiver signals:

| Pulse Width | Meaning |
|-------------|--------|
| 1000 µs | Minimum |
| 1500 µs | Neutral |
| 2000 µs | Maximum |

Drone control channels:

| Channel | Function |
|-------|----------|
| CH1 | Roll |
| CH2 | Pitch |
| CH3 | Throttle |
| CH4 | Yaw |

STM32 timers decode these signals using **input capture mode**.

---

# 10. Output Compare Mode

Output compare triggers events when:

```
Counter == Compare Register
```

Example:

```
ARR = 1000
CCR1 = 500
```

Event occurs at:

```
500 timer ticks
```

Applications:

* Precise timing events  
* Signal generation  
* Hardware scheduling  

---

# 11. Encoder Interface Mode

![Image](https://upload.wikimedia.org/wikipedia/commons/4/4c/Quadrature_Encoder.svg)

STM32 timers can work as **quadrature encoder decoders**.

Connections:

```
Encoder A → CH1
Encoder B → CH2
```

The timer automatically detects:

* Rotation direction  
* Position  
* Speed  

Counter behavior:

```
Clockwise rotation → CNT++
Anti-clockwise rotation → CNT--
```

---

# 12. Advanced PWM (TIM1 / TIM8)

Advanced timers provide special features for **motor control applications**.

Features include:

* Complementary PWM  
* Dead time insertion  
* Center aligned PWM  

Complementary PWM example:

```
PWM   → High-side MOSFET
PWMN  → Low-side MOSFET
```

Waveform:

```
PWM   : |‾‾‾|____|‾‾‾|
PWMN  : ____|‾‾‾|____|
```

Dead time prevents **MOSFET shoot-through in motor drivers**.

---

# 13. Applications in Embedded Systems

Timers are used in many embedded applications.

Examples:

| Application | Use |
|-------------|-----|
| Motor control | PWM |
| LED dimming | PWM |
| Frequency measurement | Input capture |
| Encoders | Position measurement |
| Drone ESC | PWM control |

In drone flight controllers, STM32 timers control **motor speed and decode RC receiver signals**.

---

# 14. Author

Salvi Kashyap  
Embedded Systems Learner | STM32 Firmware Development 🚀
