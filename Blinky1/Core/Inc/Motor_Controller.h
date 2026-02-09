#ifndef INC_MOTOR_CONTROLLER_H_
#define INC_MOTOR_CONTROLLER_H_

#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"

typedef struct{
	volatile uint8_t running;
	uint32_t steps_remaining;
	uint32_t period_us;
	int8_t direction;
	uint32_t accumulator_us;
	uint32_t step_pulse_width;
} MotorController_t;

//#define STEP_PULSE_WIDTH_US 40
extern MotorController_t motor_control;

void Motor_Init(void);

//Delay Generator in microseconds
void Delay_us(uint32_t us);

// Motor Functions
void Motor_Update(void);

#endif /* INC_MOTOR_CONTROLLER_H_ */
