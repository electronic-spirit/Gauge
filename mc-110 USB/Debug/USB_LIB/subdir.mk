################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_LIB/usb_device.c \
../USB_LIB/usbd_cdc.c \
../USB_LIB/usbd_cdc_if.c \
../USB_LIB/usbd_conf.c \
../USB_LIB/usbd_core.c \
../USB_LIB/usbd_ctlreq.c \
../USB_LIB/usbd_desc.c \
../USB_LIB/usbd_ioreq.c \
../USB_LIB/usbd_msc.c \
../USB_LIB/usbd_msc_bot.c \
../USB_LIB/usbd_msc_data.c \
../USB_LIB/usbd_msc_scsi.c \
../USB_LIB/usbd_storage_if.c 

OBJS += \
./USB_LIB/usb_device.o \
./USB_LIB/usbd_cdc.o \
./USB_LIB/usbd_cdc_if.o \
./USB_LIB/usbd_conf.o \
./USB_LIB/usbd_core.o \
./USB_LIB/usbd_ctlreq.o \
./USB_LIB/usbd_desc.o \
./USB_LIB/usbd_ioreq.o \
./USB_LIB/usbd_msc.o \
./USB_LIB/usbd_msc_bot.o \
./USB_LIB/usbd_msc_data.o \
./USB_LIB/usbd_msc_scsi.o \
./USB_LIB/usbd_storage_if.o 

C_DEPS += \
./USB_LIB/usb_device.d \
./USB_LIB/usbd_cdc.d \
./USB_LIB/usbd_cdc_if.d \
./USB_LIB/usbd_conf.d \
./USB_LIB/usbd_core.d \
./USB_LIB/usbd_ctlreq.d \
./USB_LIB/usbd_desc.d \
./USB_LIB/usbd_ioreq.d \
./USB_LIB/usbd_msc.d \
./USB_LIB/usbd_msc_bot.d \
./USB_LIB/usbd_msc_data.d \
./USB_LIB/usbd_msc_scsi.d \
./USB_LIB/usbd_storage_if.d 


# Each subdirectory must supply rules for building sources it contributes
USB_LIB/%.o: ../USB_LIB/%.c USB_LIB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I"../Middlewares/u8g2" -I../Screens -I../USB_LIB -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

