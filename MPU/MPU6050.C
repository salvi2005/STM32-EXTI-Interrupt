#include "stm32f10x.h"
#include <math.h>

/*
==============================
MPU6050 CONFIG
==============================
Address = 0x68
*/

#define MPU6050_ADDR 0x68

float roll = 0, pitch = 0;
float dt = 0.01;
float alpha = 0.98;

/*
==============================
I2C BASIC FUNCTIONS
==============================
*/

void I2C1_Init()
{
    RCC->APB2ENR |= (1<<3);  // GPIOB clock
    RCC->APB1ENR |= (1<<21); // I2C1 clock

    // PB6=SCL, PB7=SDA
    GPIOB->CRL &= ~(0xFF<<24);
    GPIOB->CRL |= (0xFF<<24); // AF open drain

    I2C1->CR2 = 36;     // 36 MHz
    I2C1->CCR = 180;    // 100kHz
    I2C1->TRISE = 37;

    I2C1->CR1 |= 1;     // Enable I2C
}

void I2C_Write(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C1->CR1 |= (1<<8); // START
    while(!(I2C1->SR1 & (1<<0)));

    I2C1->DR = addr<<1;
    while(!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;

    I2C1->DR = reg;
    while(!(I2C1->SR1 & (1<<7)));

    I2C1->DR = data;
    while(!(I2C1->SR1 & (1<<7)));

    I2C1->CR1 |= (1<<9); // STOP
}

uint8_t I2C_Read(uint8_t addr, uint8_t reg)
{
    uint8_t data;

    I2C1->CR1 |= (1<<8);
    while(!(I2C1->SR1 & (1<<0)));

    I2C1->DR = addr<<1;
    while(!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;

    I2C1->DR = reg;
    while(!(I2C1->SR1 & (1<<7)));

    I2C1->CR1 |= (1<<8);
    while(!(I2C1->SR1 & (1<<0)));

    I2C1->DR = (addr<<1)|1;
    while(!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;

    I2C1->CR1 &= ~(1<<10);
    I2C1->CR1 |= (1<<9);

    while(!(I2C1->SR1 & (1<<6)));
    data = I2C1->DR;

    return data;
}

int16_t I2C_Read16(uint8_t addr, uint8_t reg)
{
    int16_t high = I2C_Read(addr, reg);
    int16_t low  = I2C_Read(addr, reg+1);
    return (high<<8) | low;
}

/*
==============================
MPU6050 INIT
==============================
*/

void MPU6050_Init()
{
    I2C_Write(MPU6050_ADDR, 0x6B, 0x00); // Wake up
}

/*
==============================
MAIN SENSOR UPDATE
==============================
*/

void MPU6050_Update()
{
    int16_t accX = I2C_Read16(MPU6050_ADDR, 0x3B);
    int16_t accY = I2C_Read16(MPU6050_ADDR, 0x3D);
    int16_t accZ = I2C_Read16(MPU6050_ADDR, 0x3F);

    int16_t gyroX = I2C_Read16(MPU6050_ADDR, 0x43);
    int16_t gyroY = I2C_Read16(MPU6050_ADDR, 0x45);

    float accXf = accX / 16384.0;
    float accYf = accY / 16384.0;
    float accZf = accZ / 16384.0;

    float roll_acc = atan2(accYf, accZf) * 57.3;
    float pitch_acc = atan2(-accXf, sqrt(accYf*accYf + accZf*accZf)) * 57.3;

    float gyroXf = gyroX / 131.0;
    float gyroYf = gyroY / 131.0;

    roll = alpha*(roll + gyroXf*dt) + (1-alpha)*roll_acc;
    pitch = alpha*(pitch + gyroYf*dt) + (1-alpha)*pitch_acc;
}
