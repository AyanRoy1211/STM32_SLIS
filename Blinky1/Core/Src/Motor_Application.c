#include "Motor_Application.h"
#include "Motor_Controller.h"

void MotorApplicationInit(void)
{
    Motor_Init();
}

void SetMotorMode(EVENT_CODES_ENUM event)
{
    switch (event)
    {
        case EVT_SINGLE_CLICK:

            Motor_Steps(200, 500, 0);  // 200 steps, ~1 kHz, forward
            break;

        case EVT_DOUBLE_CLICK:

            Motor_Steps(200, 1000, 0);  // 200 steps, ~500 Hz, forward
            break;

        case EVT_HOLD_START:

            Motor_Steps(500, 500, 0);  // 500 steps at 1 kHz
            break;

        case EVT_HOLD_END:

            Motor_Stop();
            break;

        default:
            break;
    }
}
