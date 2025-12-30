#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"
#include "LCDApplication.h"
#include "UART_Application.h"

extern UARTApplicationTypedefStruct UartData;
extern ButtonApplicationTypedefStruct ButtonData;

void HandleTick(void)
{
	UpdateButton();
	UpdateLED();
}

void ApplicationInit()
{
	LED_Init();
	LCD_Init();
	UART_AppInit();
}

void ApplicationProcess()
{
	// *TODO* Integrate this when LCD is enabled
	// UpdateDisplay();

	if (UartData.NewEventReceived) {
		SetRemoteMode(UartData.CurrentEvent);
		UartData.NewEventReceived = false;
	}

	if (ButtonData.NewEventReceived) {
		UART_SendEvent(ButtonData.CurrentEvent);
		SetLocalMode(ButtonData.CurrentEvent);
		ButtonData.NewEventReceived = false;
	}
}
