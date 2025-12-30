#ifndef LEDAPPLICATION_H
#define LEDAPPLICATION_H

#include "main.h"

void LED_Init();
void UpdateLED(void);
void SetRemoteMode(EVENT_CODES_ENUM event);
void SetLocalMode(EVENT_CODES_ENUM event);

#endif /* LEDAPPLICATION_H */
