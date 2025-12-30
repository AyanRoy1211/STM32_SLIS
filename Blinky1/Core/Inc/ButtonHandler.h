#ifndef INC_BUTTONHANDLER_H_
#define INC_BUTTONHANDLER_H_

#include "main.h"
#include "stm32f0xx_hal.h"

typedef struct {
	EVENT_CODES_ENUM PreviousEvent;
	EVENT_CODES_ENUM CurrentEvent;

	bool NewEventReceived;
} ButtonApplicationTypedefStruct;

void UpdateButton(void);

#endif /* INC_BUTTONHANDLER_H_ */
