################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Screens/ADC_screen.c \
../Screens/Auto_Off_option.c \
../Screens/Display_Options.c \
../Screens/Measure_options.c \
../Screens/Measure_sreen.c \
../Screens/Real_Time_Clock.c \
../Screens/about_screen_options.c \
../Screens/logging_option_screen.c \
../Screens/menu.c 

OBJS += \
./Screens/ADC_screen.o \
./Screens/Auto_Off_option.o \
./Screens/Display_Options.o \
./Screens/Measure_options.o \
./Screens/Measure_sreen.o \
./Screens/Real_Time_Clock.o \
./Screens/about_screen_options.o \
./Screens/logging_option_screen.o \
./Screens/menu.o 

C_DEPS += \
./Screens/ADC_screen.d \
./Screens/Auto_Off_option.d \
./Screens/Display_Options.d \
./Screens/Measure_options.d \
./Screens/Measure_sreen.d \
./Screens/Real_Time_Clock.d \
./Screens/about_screen_options.d \
./Screens/logging_option_screen.d \
./Screens/menu.d 


# Each subdirectory must supply rules for building sources it contributes
Screens/%.o: ../Screens/%.c Screens/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I"../Middlewares/u8g2" -I../Screens -I../USB_LIB -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

