#include "Motor_Controller.h"
#include "gpio.h"
#include "core_cm0.h"

MotorController_t motor_control = { 0 };

void Delay_us(uint32_t us) {
	uint32_t cycles = us * (SystemCoreClock / 1000000UL) / 4U;
	volatile uint32_t i;
	for (i = 0; i < cycles; ++i) {
		__NOP();
	}
}

void Motor_Init(void) {
	HAL_GPIO_WritePin(ENN_GPIO_Port, ENN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);

//	static uint8_t counter = 100;
//
//	while (1) {
//		while (counter--) {
//			HAL_GPIO_TogglePin(STEP_GPIO_Port, STEP_Pin);
//			HAL_Delay(1);
//		}
//
//		counter = 100;
//		HAL_GPIO_TogglePin(DIR_GPIO_Port, DIR_Pin);
//
//		while (counter--) {
//			HAL_GPIO_TogglePin(STEP_GPIO_Port, STEP_Pin);
//			HAL_Delay(1);
//		}
//
//	}

	motor_control.running = 0;
	motor_control.steps_remaining = 0;
	motor_control.accumulator_us = 0;
	motor_control.period_us = 0;
	motor_control.direction = 0;
	motor_control.step_pulse_width = 40;

}

void Motor_Update(void) {
	if (!motor_control.running)
		return;

	motor_control.accumulator_us += 1000;

	while (motor_control.accumulator_us >= motor_control.period_us
			&& motor_control.running) {
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin,(motor_control.direction ? GPIO_PIN_SET : GPIO_PIN_RESET));

		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
		Delay_us(motor_control.step_pulse_width);
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);

		motor_control.accumulator_us -= motor_control.period_us;

		if (motor_control.steps_remaining > 0) {
			motor_control.steps_remaining--;
			if (motor_control.steps_remaining == 0)
				motor_control.running = 0;

		}
	}
}

