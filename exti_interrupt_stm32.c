#include <stdint.h>

#define PERIPH_BASE       0x40000000UL  //flash : 0x08000000, SRAM : 0X20000000, Periperal: 0x40000000
#define AHB1PERIPH_BASE   0x40020000UL  // periperal conncetcted to buses ( AHB:GPIO, APB1:TIMERS, APB2:SPI OR UART)
#define APB2PERIPH_BASE   0x40010000UL  
// PERIPERAL BASE CALCU.

#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800)
#define SYSCFG_BASE       (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE         (APB2PERIPH_BASE + 0x3C00)

/* GPIO Structure */
// GPIO ADDRESS: OX40020000 , RCC: 0X40023800

typedef struct
{
    volatile uint32_t MODER; //PIN MODE
    volatile uint32_t OTYPER;//PUSS-PULL/PULL-DOWN
    volatile uint32_t OSPEEDR;//PIN SPEED
    volatile uint32_t PUPDR;//PULL-UP/PULL-DOWN PIN
    volatile uint32_t IDR;//INPUT PIN
    volatile uint32_t ODR;//OUTPUT PIN
    volatile uint32_t BSRR;//FAST SET/RESET PIN
} GPIO_RegDef_t;

/* RCC Structure */

typedef struct
{
    volatile uint32_t CR; // CLOCK CONTROL (INTERNAL OSSCILATOR ENABLE/DISABLE)
    volatile uint32_t PLLCFGR;//PLL CONFIGRSTION REG. (SYS CLK INCREASE) 8MHZ TO 72 MHK
    volatile uint32_t CFGR;//CLK CONGI. (SYS CLK SOURCE SELECT)
    volatile uint32_t CIR;//CLK INTERRUPT REG.
	volatile uint32_t AHB1RSTR;//RESET REG:RESET GPIO
    volatile uint32_t AHB2RSTR;// APPB2 PERIPHERAL RESET
    volatile uint32_t RESERVED0[2];//RESERVED REG. (H/W ALLINGMENT MAINTAIN, PROGRAMMER NOT USE IT NORMALLY)
    volatile uint32_t APB1RSTR;//PERIPHERAL RESET: TIMERS, UART
    volatile uint32_t APB2RSTR;//PERIPHERAL RESET: SPI,ADC
    volatile uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;//AHB1 peripheral (GPIO CLK ENABLE)clock enable
    volatile uint32_t AHB2ENR;//AHB2 peripheral clock enable 
    volatile uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;//APB1 peripheral clock enable; TIMERS, USART2
	volatile uint32_t APB2ENR;//APB2 peripherals clock enable: SPI1, USART1, ADC
} RCC_RegDef_t;

/* EXTI Structure: EXTERNAL INTERRUPT CONTROLLER */

typedef struct
{
    volatile uint32_t IMR;//INT. MASK REG. (INT. ENABLE/DISABLE)
    volatile uint32_t EMR;//EVENT MASK REG. 
    volatile uint32_t RTSR;//RISING TRIGGER SELECTION REG.(BUTTON PRESS)
    volatile uint32_t FTSR;//FALLING TRIGGER SELECTION REG.(BUTTON RELEASE)
    volatile uint32_t SWIER;//SOFTWARE INTERRUPT REG. 
    volatile uint32_t PR;//PENDING REG.
} EXTI_RegDef_t;

/* Peripheral pointers */

#define GPIOA   ((GPIO_RegDef_t*) GPIOA_BASE)
#define RCC     ((RCC_RegDef_t*) RCC_BASE)
#define EXTI    ((EXTI_RegDef_t*) EXTI_BASE)

/* NVIC register */

#define NVIC_ISER0   (*(volatile uint32_t*)0xE000E100)
//INETRRUPT SET ENABLE
/* GPIO Initialization */

void GPIO_Init()
{
    RCC->AHB1ENR |= (1<<0); //GPIO ENABLE CLK

    GPIOA->MODER &= ~(3<<(2*0));//PA0 INPUT MODE SET

    GPIOA->MODER |= (1<<(2*5));//PA5 OUTPUT MODE SET
}

/* EXTI Configuration */

void EXTI_Config()
{
    RCC->APB2ENR |= (1<<14);//SYSCFG CLK ENABLE

    EXTI->IMR |= (1<<0);//LINE 0 ME INTERRUPT ENABLE

    EXTI->RTSR |= (1<<0);//RISING EDGE TRIGGER ENABLE
}

/* NVIC Enable */

void NVIC_Enable()
{
    NVIC_ISER0 |= (1<<6);//EXTI0 = INETRRUPT NUMBER IS 6
}

/* Interrupt Service Routine */

void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & (1<<0))
    {
        GPIOA->ODR ^= (1<<5); //TOGGELE

        EXTI->PR |= (1<<0);//CLEAR PIN
    }
}

/* Main Program */

int main()
{
    GPIO_Init();
    EXTI_Config();
    NVIC_Enable();

    while(1)
    {

    }
}
