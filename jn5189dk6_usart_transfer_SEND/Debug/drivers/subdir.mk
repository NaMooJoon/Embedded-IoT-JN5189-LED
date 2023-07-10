################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_dma.c \
../drivers/fsl_flash.c \
../drivers/fsl_flexcomm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_inputmux.c \
../drivers/fsl_power.c \
../drivers/fsl_reset.c \
../drivers/fsl_usart.c \
../drivers/fsl_usart_dma.c 

C_DEPS += \
./drivers/fsl_adc.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_dma.d \
./drivers/fsl_flash.d \
./drivers/fsl_flexcomm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_inputmux.d \
./drivers/fsl_power.d \
./drivers/fsl_reset.d \
./drivers/fsl_usart.d \
./drivers/fsl_usart_dma.d 

OBJS += \
./drivers/fsl_adc.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_dma.o \
./drivers/fsl_flash.o \
./drivers/fsl_flexcomm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_inputmux.o \
./drivers/fsl_power.o \
./drivers/fsl_reset.o \
./drivers/fsl_usart.o \
./drivers/fsl_usart_dma.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_JN5189 -DJN5189DK6 -DDK6 -DCPU_JN518X -DCPU_JN5189HN -DCPU_JN5189HN_cm4 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\source" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\drivers" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\device" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\utilities" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\component\serial_manager" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\component\lists" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\component\uart" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\CMSIS" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\board" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_adc.d ./drivers/fsl_adc.o ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_dma.d ./drivers/fsl_dma.o ./drivers/fsl_flash.d ./drivers/fsl_flash.o ./drivers/fsl_flexcomm.d ./drivers/fsl_flexcomm.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_inputmux.d ./drivers/fsl_inputmux.o ./drivers/fsl_power.d ./drivers/fsl_power.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_usart.d ./drivers/fsl_usart.o ./drivers/fsl_usart_dma.d ./drivers/fsl_usart_dma.o

.PHONY: clean-drivers
