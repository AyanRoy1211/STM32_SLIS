#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"
#include "LCDApplication.h"
#include "UART_Application.h"
#include "LCDCore.h"
#include "LED_Controller.h"
#include "Motor_Application.h"
#include "Keypad.h"
#include "KeypadHandler.h"

extern UARTApplicationTypedefStruct UartData;
extern ButtonApplicationTypedefStruct ButtonData;
extern KeypadApplicationTypedefStruct KeypadData;
extern LED_Controller_t local_ctrl;
extern LED_Controller_t remote_ctrl;

void HandleTick(void) {
	UpdateButton();
	UpdateLED();
	MotorApplication_Update();
	UpdateKeypad();
}

void ApplicationInit() {
	LED_Init();
	UART_AppInit();
	//LCD_Init();
//	TMC2208_ApplicationInit();
	MotorApplication_Init();
	KeypadHandler_Init();
}

void ApplicationProcess() {

	LCDApplication_Process();

	if (UartData.NewEventReceived) {
		SetRemoteMode(UartData.CurrentEvent);
		MotorApplication_SetMotorMode(UartData.CurrentEvent);


		LCD_UpdateRemoteMode(UartData.CurrentEvent);
		UartData.NewEventReceived = false;
	}
	if (ButtonData.NewEventReceived) {
		UART_SendEvent(ButtonData.CurrentEvent);
		SetLocalMode(ButtonData.CurrentEvent);
		MotorApplication_SetMotorMode(ButtonData.CurrentEvent);

		LCD_UpdateLocalMode(ButtonData.CurrentEvent);

		ButtonData.NewEventReceived = false;
	}
    if (KeypadData.NewEventReceived)
    {
   		UART_SendEvent(KeypadData.CurrentEvent);
   		MotorApplication_SetMotorMode(KeypadData.CurrentEvent);
   		SetLocalMode(KeypadData.CurrentEvent);
   		LCD_UpdateLocalMode(KeypadData.CurrentEvent);

   		KeypadData.NewEventReceived = false;
   	}
	LCD_UpdateLocalLED(local_ctrl.mode);
	LCD_UpdateRemoteLED(remote_ctrl.mode);
}
