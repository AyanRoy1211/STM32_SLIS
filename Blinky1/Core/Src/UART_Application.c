#include "UART_Application.h"
#include "LEDApplication.h"

extern UART_HandleTypeDef huart1;
extern uint8_t current_mode;

static uint8_t uart_rx_byte = 0;

// Debug counters - add these to Live Expressions
volatile uint32_t uart_tx_count = 0;  // Increments when sending
volatile uint32_t uart_rx_count = 0;  // Increments when receiving
volatile uint8_t last_rx_byte = 0;    // Last received byte

void UART_AppInit(void)
{
    HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
}

void UART_SendEvent(uint8_t evt)
{
    // Send event to the other board via UART
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &evt, 1, 10);

    if (status == HAL_OK)
    {
        uart_tx_count++;  // Debug: count successful transmissions
    }
}

void UART_OnByteReceived(void)
{
    // Debug counters
    uart_rx_count++;
    last_rx_byte = uart_rx_byte;

    // Receive event from the other board and update remote LED (LD4)
    switch(uart_rx_byte)
    {
        case EVT_SINGLE_CLICK:
            SetRemoteMode(1);  // Fast blink on LD4
            break;

        case EVT_DOUBLE_CLICK:
            SetRemoteMode(2);  // Slow blink on LD4
            break;

        case EVT_HOLD_START:
            SetRemoteMode(3);  // LED ON on LD4
            break;

        case EVT_HOLD_END:
            SetRemoteMode(0);  // LED OFF on LD4 (IDLE)
            break;

        default:
            break;
    }

    // Clear error flags and restart interrupt
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF);
    HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
}
