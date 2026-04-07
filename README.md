# Flappy Bird on STM32 with FreeRTOS

An embedded implementation of the classic **Flappy Bird game** built on **STM32 microcontroller** using **FreeRTOS**, custom peripheral drivers, and a self-written graphics stack for SH1106 OLED display.

This project demonstrates real-time embedded software design using task scheduling, interrupt-driven input handling, software timers, and low-level hardware abstraction.

---

## Features

- Flappy Bird gameplay on OLED display
- FreeRTOS-based multitasking architecture
- Responsive button input with software debouncing
- Custom SH1106 OLED display driver
- Custom graphics library for rendering game objects
- Obstacle generation and collision detection
- Score tracking
- Timer-controlled game update loop

## Demo Video

[Watch Flappy bird Demo](https://www.youtube.com/watch?v=rguRnjICWhw)

## How to Setup
### 1. Clone the Repository
```bash
git clone <repo-link>
cd <project-folder>
```
### 2. Build the project
```bash
cmake -B build
cmake --build build
```
### 3. Load and Flash
```bash
openocd -f /usr/share/openocd/scripts/board/stm32f429disc1.cfg #for stm32f407xx discovery board
arm-none-eabi-gdb Flappy_bird.elf
target remote localhost:3333
monitor reset halt
load
monitor reset init
continue
```
---
## Software Used

- Embedded C
- FreeRTOS
- CMake build system
- ARM GCC Toolchain
- OpenOCD
- GDB for flashing and debugging

### Embedded Components Used

- FreeRTOS Tasks
- Direct Task Notification
- Software Timer
- External Interrupt Handling
- Software Debouncing

### Custom Drivers Implemented

- GPIO Driver
- I2C Driver
- SPI Driver
- UART Driver
- Timer Driver
- SH1106 OLED Display Driver
- Graphics Library

---

## Hardware Used

- STM32F407 Discovery Board
- 128x64 SH1106 OLED Display

### Connections

- PB7 → SDA  
- PB6 → SCL  
- 3.3V → VCC  
- GND → GND  

### Input Device

- STM32 Discovery onboard user button used for gameplay

---


