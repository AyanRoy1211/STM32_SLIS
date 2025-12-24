/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define HOLD_TIME_MS       200u
#define CLICK_WINDOW_MS    200u
#define DEBOUNCE_TIME_MS   20u
#define DELAY_COUNT        4u

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

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
static uint8_t btn_stable = 0;
static uint8_t btn_prev = 0;
static uint32_t btn_debounce = 0;
static uint32_t btn_press_time = 0;
static uint32_t btn_click_timer = 0;
static uint8_t btn_clicks = 0;
static uint8_t btn_hold = 0;

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

/* LCD Functions */
static void LCD_EnablePulse(void);
static void LCD_Send4Bit(uint8_t data);
static void LCD_Command(uint8_t cmd);
static void LCD_Data(uint8_t data);
static void LCD_Init(void);
static void LCD_SetCursor(uint8_t row, uint8_t col);
static void LCD_Print(char *str);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* ================= LCD DRIVER (4-bit mode) ================= */

static void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

static void LCD_Send4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_EnablePulse();
}

static void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);

    LCD_Send4Bit(cmd >> 4);
    LCD_Send4Bit(cmd & 0x0F);

    HAL_Delay(2);
}

static void LCD_Data(uint8_t data)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);

    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);

    HAL_Delay(2);
}

static void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_Command(0x33);
    HAL_Delay(5);
    LCD_Command(0x32);
    HAL_Delay(5);
    LCD_Command(0x28);   /* 4-bit, 2 line, 5x8 dots */
    LCD_Command(0x0C);   /* Display ON, Cursor OFF */
    LCD_Command(0x06);   /* Entry mode: increment cursor */
    LCD_Command(0x01);   /* Clear display */
    HAL_Delay(5);
}

static void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    if (row == 0)
        addr = 0x80 + col;
    else
        addr = 0xC0 + col;

    LCD_Command(addr);
}

static void LCD_Print(char *str)
{
    while (*str)
        LCD_Data(*str++);
}

/* Keep delay_idx in valid range [0..DELAY_COUNT-1] */
static inline void WrapIndex(void)
{
    if (delay_idx >= (int8_t)DELAY_COUNT)
        delay_idx = 0;
    else if (delay_idx < 0)
        delay_idx = (int8_t)DELAY_COUNT - 1;
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
static void UpdateButton(void)
{
    uint8_t btn_raw = (uint8_t)HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin);

    /* --- Debounce --- */
    if (btn_raw != btn_stable)
    {
        btn_debounce++;
        if (btn_debounce >= DEBOUNCE_TIME_MS)
        {
            btn_stable = btn_raw;
            btn_debounce = 0;
        }
    }
    else
    {
        btn_debounce = 0;
    }

    /* --- Rising edge (press detected) --- */
    if (btn_stable && !btn_prev)
    {
        btn_press_time = 0;

        if (btn_clicks < 2)
            btn_clicks++;

        btn_click_timer = 0;
    }

    /* --- While pressed --- */
    if (btn_stable)
    {
        btn_press_time++;

        /* Hold detection */
        if (!btn_hold && (btn_press_time >= HOLD_TIME_MS))
        {
            btn_hold = 1;
            btn_clicks = 0;
            btn_click_timer = 0;
            current_mode = 3;  /* Set HOLD mode for display */

            /* Halve all delays */
            for (uint8_t i = 0; i < DELAY_COUNT; i++)
            {
                current_delays[i] = base_delays[i] / 2u;
                if (current_delays[i] == 0u)
                    current_delays[i] = 1u;
            }
        }
    }

    /* --- Falling edge (release detected) --- */
    if (!btn_stable && btn_prev)
    {
        btn_press_time = 0;

        if (btn_hold)
        {
            /* Restore normal delays */
            for (uint8_t i = 0; i < DELAY_COUNT; i++)
                current_delays[i] = base_delays[i];

            current_mode = 0;  /* Back to IDLE */
        }

        btn_hold = 0;
        btn_click_timer = 0;
    }

    /* --- Click resolution (button released, not holding) --- */
    if (!btn_hold && !btn_stable && (btn_clicks > 0))
    {
        btn_click_timer++;

        if (btn_click_timer >= CLICK_WINDOW_MS)
        {
            if (btn_clicks == 1)
            {
                /* Single click: move to next delay */
                delay_idx += direction;
                WrapIndex();
                led_timer = 0;
                toggle_count = 0;
                current_mode = 1;  /* SINGLE mode for display */
            }
            else
            {
                /* Double click: reverse direction */
                direction = -direction;
                current_mode = 2;  /* DOUBLE mode for display */
            }

            btn_clicks = 0;
            btn_click_timer = 0;
        }
    }

    btn_prev = btn_stable;
}

/* Display update - ONLY call from main loop, NOT from ISR! */
static void UpdateDisplay(void)
{
    static uint32_t last_mode_change = 0;

    /* Update mode display */
    if (current_mode != prev_mode)
    {
        LCD_SetCursor(0, 0);
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

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/
    HAL_Init();

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */

    /* Start with LED off */
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    /* Initialize LCD display */
    HAL_Delay(100);
    LCD_Init();

    LCD_SetCursor(0, 0);
    LCD_Print("STM32F0 ");
    LCD_SetCursor(1, 0);
    LCD_Print("Delay: 250 ms   ");

    HAL_Delay(2000);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        UpdateDisplay();
        HAL_Delay(200);  /* Update LCD every 200ms */
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
//static void MX_GPIO_Init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//    /* USER CODE BEGIN MX_GPIO_Init_1 */
//    /* USER CODE END MX_GPIO_Init_1 */
//
//    /* GPIO Ports Clock Enable */
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOC_CLK_ENABLE();
//
//    /* Configure LED pins output level */
//    HAL_GPIO_WritePin(GPIOC, LD4_Pin | LD3_Pin, GPIO_PIN_RESET);
//
//    /* Configure LCD pins output level (PA1-PA6) */
//    HAL_GPIO_WritePin(GPIOA, RS_Pin | E_Pin | DB4_Pin | DB5_Pin | DB6_Pin | DB7_Pin, GPIO_PIN_RESET);
//
//    /* Configure button input (PA0) */
//    GPIO_InitStruct.Pin = Button_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(Button_GPIO_Port, &GPIO_InitStruct);
//
//    /* Configure LCD output pins (PA1-PA6: RS, E, DB4-DB7) */
//    GPIO_InitStruct.Pin = RS_Pin | E_Pin | DB4_Pin | DB5_Pin | DB6_Pin | DB7_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    /* Configure LED outputs (PC8-PC9) */
//    GPIO_InitStruct.Pin = LD4_Pin | LD3_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//    /* USER CODE BEGIN MX_GPIO_Init_2 */
//    /* USER CODE END MX_GPIO_Init_2 */
//}

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
