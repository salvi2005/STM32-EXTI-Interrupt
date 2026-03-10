#include <stdint.h>

#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   0x40020000UL
#define APB2PERIPH_BASE   0x40010000UL  

#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800)
#define SYSCFG_BASE       (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE         (APB2PERIPH_BASE + 0x3C00)

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
} GPIO_RegDef_t;

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
} RCC_RegDef_t;

typedef struct
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_RegDef_t;

#define GPIOA   ((GPIO_RegDef_t*) GPIOA_BASE)
#define RCC     ((RCC_RegDef_t*) RCC_BASE)
#define EXTI    ((EXTI_RegDef_t*) EXTI_BASE)

#define NVIC_ISER0   (*(volatile uint32_t*)0xE000E100)

void GPIO_Init()
{
    RCC->AHB1ENR |= (1<<0);

    GPIOA->MODER &= ~(3<<(2*0));

    GPIOA->MODER |= (1<<(2*5));
}

void EXTI_Config()
{
    RCC->APB2ENR |= (1<<14);

    EXTI->IMR |= (1<<0);

    EXTI->RTSR |= (1<<0);
}

void NVIC_Enable()
{
    NVIC_ISER0 |= (1<<6);
}

void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & (1<<0))
    {
        GPIOA->ODR ^= (1<<5);

        EXTI->PR |= (1<<0);
    }
}

int main()
{
    GPIO_Init();
    EXTI_Config();
    NVIC_Enable();

    while(1)
    {

    }
}
