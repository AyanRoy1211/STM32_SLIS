#include "Motor_Controller.h"
#include "gpio.h"

MotorController_t motor_control = {0,0,0};

extern volatile uint32_t uwTick;

void delay_us(uint32_t us)
{
	uint32_t start = uwTick;
	uint32_t delay_ms = (us + 999)/1000;

	if(delay_ms == 0)
		delay_ms = 1;

	while((uwTick - start) < delay_ms){

	}
}

void Motor_Init(void)
{
	HAL_GPIO_WritePin(ENN_GPIO_Port, ENN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);

}

void Motor_Steps(uint32_t steps,uint32_t direction,uint32_t frequency)
{
	if(motor_control.is_running)
		return;

	motor_control.is_running = 1;
	motor_control.steps_target = steps;
	motor_control.steps_done = 0;

	// Set Direction
	if(direction)
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(ENN_GPIO_Port, ENN_Pin, GPIO_PIN_RESET);

	for(uint32_t i = 0;i < steps;i++)
	{
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
		delay_us(frequency);
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
		delay_us(frequency);

		motor_control.steps_done++;
	}
	HAL_GPIO_WritePin(ENN_GPIO_Port, ENN_Pin, GPIO_PIN_SET);

	motor_control.is_running = 0;
}

void Motor_Stop(void)
{
	HAL_GPIO_WritePin(ENN_GPIO_Port, ENN_Pin, GPIO_PIN_SET);
	motor_control.is_running = 0;
	motor_control.steps_done = 0;
}


