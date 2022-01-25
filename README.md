# stm32f429i-disco-lcd-demo
LCD demo for STM32F429I-DISCO board to use with STM32Cube software

# Introduction

This repository contains demonstation software for STMicroelectronics discovery kit with STM32F429ZI MCU.

It is ready to use with the STM32CubeMX (version 6.4.0) and STM32CubeIDE (version 1.8.0) software.

The code demonstrate base usage of following components using standard HAL library:
- LTDC (for LCD driving)
- FMC (external memory)
- DMA2D (basic demo)

# How to usage

Download project into ZKZ-Demo2 directory on your workstation.  
Import the project into STM32CubeIDE workspace using *File/Open Projects from File System* menu.  

If you want, you can modify STM32CubeMX project and generate a code again. All code was written between USER CODE BEGIN/USER CODE END clauses so above operation do not remove added code.

![usage](/ZKZ-Demo2/Resources/ZKZ-Demo2-usage1.jpg)

#

The code uses ssd1306 fonts file from [afiskon/stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306) library.
