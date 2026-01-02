#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"
#include "LCDApplication.h"
#include "UART_Application.h"
#include "LCDCore.h"
#include "LED_Controller.h"
#include "TMC2208_UART_Application.h"

extern UARTApplicationTypedefStruct UartData;
extern ButtonApplicationTypedefStruct ButtonData;
extern LED_Controller_t local_ctrl;
extern LED_Controller_t remote_ctrl;

void HandleTick(void) {
	UpdateButton();
	UpdateLED();
}

void ApplicationInit() {
	LED_Init();
	//LCD_Init();
	//UART_AppInit();
	TMC2208_ApplicationInit();
}

void ApplicationProcess() {

	LCDApplication_Process();

	if (UartData.NewEventReceived) {
		SetRemoteMode(UartData.CurrentEvent);

		LCD_UpdateRemoteMode(UartData.CurrentEvent);
		UartData.NewEventReceived = false;
	}
	if (ButtonData.NewEventReceived) {
		UART_SendEvent(ButtonData.CurrentEvent);
		SetLocalMode(ButtonData.CurrentEvent);

		LCD_UpdateLocalMode(ButtonData.CurrentEvent);

		ButtonData.NewEventReceived = false;
	}
	LCD_UpdateLocalLED(local_ctrl.mode);
	LCD_UpdateRemoteLED(remote_ctrl.mode);
}
