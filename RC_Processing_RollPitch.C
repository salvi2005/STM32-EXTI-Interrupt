#include "stm32f10x.h"

uint16_t roll_raw;
uint16_t pitch_raw;
uint16_t throttle_raw;
uint16_t yaw_raw;

float roll_filtered;
float pitch_filtered;

float alpha = 0.1;

float lowpass(float filtered, float new_value)
{
return filtered + alpha*(new_value-filtered);
}

float normalize(uint16_t pulse)
{
return ((float)pulse -1500.0f)/500.0f;
}

int deadband(int value)
{

if(value>1480 && value<1520)
return 1500;

return value;

}

void process_rc()
{

roll_raw = TIM2->CCR1;
pitch_raw = TIM2->CCR2;
throttle_raw = TIM2->CCR3;
yaw_raw = TIM2->CCR4;

roll_raw = deadband(roll_raw);
pitch_raw = deadband(pitch_raw);

roll_filtered = lowpass(roll_filtered, roll_raw);
pitch_filtered = lowpass(pitch_filtered, pitch_raw);

float roll_input = normalize(roll_filtered);
float pitch_input = normalize(pitch_filtered);

}
