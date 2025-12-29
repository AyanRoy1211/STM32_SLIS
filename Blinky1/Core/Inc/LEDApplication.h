#ifndef LEDAPPLICATION_H
#define LEDAPPLICATION_H

#include "main.h"
#include "LED_Controller.h"

extern LED_Controller_t local_ctrl;
extern LED_Controller_t remote_ctrl;

void UpdateLED(void);
void SetRemoteMode(uint8_t mode);

#endif /* LEDAPPLICATION_H */
