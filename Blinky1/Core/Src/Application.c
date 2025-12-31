#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"
#include "LCDApplication.h"
#include "UART_Application.h"
#include "LCDCore.h"
#include "LED_Controller.h"

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
	LCD_Init();
	UART_AppInit();
}

void ApplicationProcess() {

	LCDApplication_Process();

	if (UartData.NewEventReceived) {
		SetRemoteMode(UartData.CurrentEvent);

		LCD_UpdateRemoteMode((DisplayMode_t) UartData.CurrentEvent);
		UartData.NewEventReceived = false;
	}
	if (ButtonData.NewEventReceived) {
		UART_SendEvent(ButtonData.CurrentEvent);
		SetLocalMode(ButtonData.CurrentEvent);

		LCD_UpdateLocalMode((DisplayMode_t) ButtonData.CurrentEvent);

		ButtonData.NewEventReceived = false;
	}
	LCD_UpdateLocalLED((LEDStatus_t) local_ctrl.mode);
	LCD_UpdateRemoteLED((LEDStatus_t) remote_ctrl.mode);
}
