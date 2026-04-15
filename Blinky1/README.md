# STM32 Embedded Control Project

> Student project from National Institute of Technology Sikkim
> Department of Electrical and Electronics Engineering
> Focused on Embedded Systems design and firmware development.

## Project Overview

This repository contains an STM32-based embedded application built for the STM32F0 family. The firmware integrates button and keypad input, UART communication, LED status control, LCD status display, and motor control logic. It demonstrates a modular embedded architecture using HAL drivers, custom input handling, and peripheral control.

## Key Features

- **Button Input Handling**
  - Supports single click, double click, hold start, and hold end events.
  - Converts physical button actions into application-level event codes.
- **Keypad Control**
  - Maps keypad keys to motor control commands.
  - Handles pressed, released, and hold events for precise operation.
- **UART Communication**
  - Receives events from external devices over USART1.
  - Sends detected events back over UART to enable remote synchronization.
- **LED Feedback**
  - Local and remote LED channels indicate active states.
  - Supports idle, fast blink, slow blink, and hold-on modes.
- **LCD Status Display**
  - Updates local and remote mode states on a four-line LCD.
  - Displays current mode and LED status for both local and remote inputs.
- **Motor Control**
  - Controls motor direction, movement, continuous operation, stop, and speed adjustments.
  - Includes support code for TMC2208 stepper motor drivers.

## Repository Structure

| Folder | Purpose |
|---|---|
| `Inc/` | Header files for application modules, peripheral drivers, and shared definitions |
| `Src/` | Source implementations for application logic, input handling, display, UART, and motor control |
| `Startup/` | MCU startup assembly file for STM32F051R8Tx |

## Main Application Flow

`Src/main.c` initializes the MCU and peripherals, then enters a continuous loop that calls `ApplicationProcess()`.

### Application lifecycle

1. `ApplicationInit()` initializes:
   - GPIO
   - USART1
   - button/keypad handlers
   - LED application
   - motor application
   - UART application
2. `HandleTick()` is called from `SysTick` to update:
   - buttons
   - LEDs
   - motor logic
   - keypad scanning
3. `ApplicationProcess()` updates the LCD and routes events from:
   - UART input
   - physical button input
   - keypad input

## Module Summary

### Input and control modules

| Module | Functionality |
|---|---|
| `ButtonHandler` | Reads button events and converts them to internal event codes |
| `KeypadHandler` | Scans keypad matrix and maps key events to motor commands |
| `UART_Application` | Manages asynchronous UART receive and transmit events |

### Output and feedback modules

| Module | Functionality |
|---|---|
| `LEDApplication` | Controls local and remote LED blink/hold states |
| `LCDApplication` | Updates LCD with mode and LED status information |
| `Motor_Application` | Issues motor commands based on keypad and event input |

### Peripheral support and driver modules

| Module | Notes |
|---|---|
| `gpio.c` / `gpio.h` | GPIO initialization and pin definitions |
| `usart.c` / `usart.h` | USART1 initialization and low-level UART support |
| `TMC2208_Core` | Stepper driver helper functions for TMC2208 registers and pin state parsing |

## Functional Capabilities Implemented

| Feature | Description |
|---|---|
| Local/Remote Mode Control | Button events change LED and LCD states locally and remotely via UART events |
| Motor Direction | Keypad keys rotate motor clockwise or anticlockwise |
| Motor Continuous Operation | Long key hold triggers continuous motor run, release stops it |
| Motor Speed Adjustment | Dedicated keypad input changes motor pulse timing |
| LCD Status Display | Local and remote mode/LED status displayed on a 4-line LCD panel |

## Hardware and Environment

- MCU: STM32F0 series
- Development: STM32 HAL-based firmware
- Peripherals: GPIO, USART1, LCD, LEDs, keypad matrix, motor driver interface
- Supports TMC2208 stepper driver configuration and status reading

## Notes

- The repository is organized to separate application logic from hardware interface code.
- Core event processing is centralized in `Application.c`, while peripheral-specific behavior is handled by dedicated modules.
- The project is a strong example of embedded firmware design with modular structure and clear control flow.

## Student Profile

- Student of **National Institute of Technology Sikkim**
- Department of **Electrical and Electronics Engineering**
- Passionate about **Embedded Systems** and firmware development

---

Thank you for reviewing this embedded systems project. The implementation reflects hands-on experience with STM32 MCU programming, peripheral integration, and event-driven firmware architecture.
