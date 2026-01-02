#ifndef INC_MOTOR_CONTROLLER_H_
#define INC_MOTOR_CONTROLLER_H_

#include "stm32f0xx_hal.h"

typedef struct{
	uint32_t steps_target;
	uint32_t steps_done;
	uint8_t is_running;
} MotorController_t;

extern MotorController_t motor_ctrl;

// Initialize Motor States
void Motor_Init(void);

//Delay Generator in microseconds
void delay_us(uint32_t us);

// Motor Functions
void Motor_Steps(uint32_t steps,uint32_t direction,uint32_t frequency);
void Motor_Stop(void);


#endif /* INC_MOTOR_CONTROLLER_H_ */
