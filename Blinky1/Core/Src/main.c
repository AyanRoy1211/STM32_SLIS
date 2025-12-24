/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define HOLD_TIME_MS       200u
#define CLICK_WINDOW_MS    200u
#define DEBOUNCE_TIME_MS   30u
#define DELAY_COUNT        4u

/* USER CODE END PTD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* Delay table */
static const uint32_t base_delays[DELAY_COUNT] = {250, 500, 1000, 2000};
static volatile uint32_t current_delays[DELAY_COUNT] = {250, 500, 1000, 2000};

/* LED state */
static volatile uint32_t led_timer = 0;
static volatile uint8_t toggle_count = 0;
static volatile int8_t delay_idx = 0;
static volatile int8_t direction = 1;

/* Button state */
static volatile uint8_t btn_stable = 0;
static volatile uint8_t btn_prev = 0;
static volatile uint32_t btn_debounce = 0;
static volatile uint32_t btn_press_time = 0;
static volatile uint32_t btn_click_timer = 0;
static volatile uint8_t btn_clicks = 0;
static volatile uint8_t btn_hold = 0;

/* LCD display tracking */
static uint8_t prev_mode = 255;
static uint32_t prev_delay = 0;
static volatile uint8_t current_mode = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void HandleTick(void);
static void UpdateLED(void);
static void UpdateButton(void);
static void UpdateDisplay(void);
static inline void WrapIndex(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Keep delay_idx in valid range [0..DELAY_COUNT-1] */
static inline void WrapIndex(void)
{
    if (delay_idx >= DELAY_COUNT)
        delay_idx = 0;
    else if (delay_idx < 0)
        delay_idx = DELAY_COUNT - 1;
}

/* LED update logic (called every ms) */
static void UpdateLED(void)
{
    led_timer++;

    if (led_timer >= current_delays[delay_idx])
    {
        led_timer = 0;
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

        toggle_count++;

        /* After 2 toggles (one complete blink: ON→OFF), move to next delay */
        if (toggle_count >= 2)
        {
            toggle_count = 0;
            delay_idx += direction;
            WrapIndex();
        }
    }
}

/* Button update logic (called every ms) */


/* Display update - ONLY call from main loop, NOT from ISR! */
static void UpdateDisplay(void)
{
    static uint32_t last_mode_change = 0;

    /* Update mode display */
    if (current_mode != prev_mode)
    {
        LCD_SetCursor(0,0);
        switch (current_mode)
        {
            case 3:
                LCD_Print("Mode: HOLD      ");
                break;
            case 2:
                LCD_Print("Mode: DOUBLE    ");
                last_mode_change = HAL_GetTick();
                break;
            case 1:
                LCD_Print("Mode: SINGLE    ");
                last_mode_change = HAL_GetTick();
                break;
            default:
                LCD_Print("Mode: IDLE      ");
                break;
        }
        prev_mode = current_mode;
    }

    /* Auto-reset mode to IDLE after 1 second */
    if ((current_mode == 1 || current_mode == 2) &&
        (HAL_GetTick() - last_mode_change >= 1000))
    {
        current_mode = 0;
    }

    /* Update delay display */
    if (current_delays[delay_idx] != prev_delay)
    {
        char buffer[20];
        LCD_SetCursor(1, 0);
        snprintf(buffer, sizeof(buffer), "Delay: %lu ms   ", current_delays[delay_idx]);
        LCD_Print(buffer);
        prev_delay = current_delays[delay_idx];
    }
}

/* Main tick handler (called every 1ms) */
static void HandleTick(void)
{
    UpdateLED();
    UpdateButton();
}

/* USER CODE END 0 */

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */

    /* Start with LED off */
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    /* Initialize LCD display */
    HAL_Delay(100);
    LCD_Init();

    LCD_SetCursor(0, 0);
    LCD_Print("Mode: IDLE      ");
    LCD_SetCursor(1, 0);
    LCD_Print("Delay: 250 ms   ");

    HAL_Delay(2000);

    /* USER CODE END 2 */

    while (1)
    {
        UpdateDisplay();
        HAL_Delay(200);
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

void HAL_SYSTICK_Callback(void)
{
    HandleTick();
}

/* USER CODE END 4 */

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
