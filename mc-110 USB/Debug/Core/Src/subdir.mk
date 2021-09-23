################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ADC.c \
../Core/Src/Basic_LCD.c \
../Core/Src/Buttons.c \
../Core/Src/Display.c \
../Core/Src/ELSPIRE_OS.c \
../Core/Src/Global_Variables.c \
../Core/Src/MAX77960.c \
../Core/Src/Units_Converter.c \
../Core/Src/Usart_Parser.c \
../Core/Src/W25N01.c \
../Core/Src/calibration.c \
../Core/Src/logging_fatfs.c \
../Core/Src/main.c \
../Core/Src/sound.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_hal_timebase_tim.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/temperature.c \
../Core/Src/u8g2_LCD.c 

OBJS += \
./Core/Src/ADC.o \
./Core/Src/Basic_LCD.o \
./Core/Src/Buttons.o \
./Core/Src/Display.o \
./Core/Src/ELSPIRE_OS.o \
./Core/Src/Global_Variables.o \
./Core/Src/MAX77960.o \
./Core/Src/Units_Converter.o \
./Core/Src/Usart_Parser.o \
./Core/Src/W25N01.o \
./Core/Src/calibration.o \
./Core/Src/logging_fatfs.o \
./Core/Src/main.o \
./Core/Src/sound.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_hal_timebase_tim.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/temperature.o \
./Core/Src/u8g2_LCD.o 

C_DEPS += \
./Core/Src/ADC.d \
./Core/Src/Basic_LCD.d \
./Core/Src/Buttons.d \
./Core/Src/Display.d \
./Core/Src/ELSPIRE_OS.d \
./Core/Src/Global_Variables.d \
./Core/Src/MAX77960.d \
./Core/Src/Units_Converter.d \
./Core/Src/Usart_Parser.d \
./Core/Src/W25N01.d \
./Core/Src/calibration.d \
./Core/Src/logging_fatfs.d \
./Core/Src/main.d \
./Core/Src/sound.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_hal_timebase_tim.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/temperature.d \
./Core/Src/u8g2_LCD.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I"../Middlewares/u8g2" -I../Screens -I../USB_LIB -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

