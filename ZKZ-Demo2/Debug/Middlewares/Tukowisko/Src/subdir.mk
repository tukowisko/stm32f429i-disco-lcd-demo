################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Tukowisko/Src/ssd1306_fonts.c \
../Middlewares/Tukowisko/Src/tk_grs.c \
../Middlewares/Tukowisko/Src/tk_inits.c 

OBJS += \
./Middlewares/Tukowisko/Src/ssd1306_fonts.o \
./Middlewares/Tukowisko/Src/tk_grs.o \
./Middlewares/Tukowisko/Src/tk_inits.o 

C_DEPS += \
./Middlewares/Tukowisko/Src/ssd1306_fonts.d \
./Middlewares/Tukowisko/Src/tk_grs.d \
./Middlewares/Tukowisko/Src/tk_inits.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Tukowisko/Src/%.o: ../Middlewares/Tukowisko/Src/%.c Middlewares/Tukowisko/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../USB_HOST/App -I../Middlewares/Tukowisko/Inc -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Tukowisko-2f-Src

clean-Middlewares-2f-Tukowisko-2f-Src:
	-$(RM) ./Middlewares/Tukowisko/Src/ssd1306_fonts.d ./Middlewares/Tukowisko/Src/ssd1306_fonts.o ./Middlewares/Tukowisko/Src/tk_grs.d ./Middlewares/Tukowisko/Src/tk_grs.o ./Middlewares/Tukowisko/Src/tk_inits.d ./Middlewares/Tukowisko/Src/tk_inits.o

.PHONY: clean-Middlewares-2f-Tukowisko-2f-Src

