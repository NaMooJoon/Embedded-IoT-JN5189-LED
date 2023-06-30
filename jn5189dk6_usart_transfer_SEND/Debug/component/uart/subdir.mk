################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/usart_adapter.c 

C_DEPS += \
./component/uart/usart_adapter.d 

OBJS += \
./component/uart/usart_adapter.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_JN5189 -DJN5189DK6 -DDK6 -DCPU_JN518X -DCPU_JN5189HN -DCPU_JN5189HN_cm4 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\source" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\drivers" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\device" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\utilities" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\component\serial_manager" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\component\lists" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\component\uart" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\CMSIS" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_SEND\board" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/usart_adapter.d ./component/uart/usart_adapter.o

.PHONY: clean-component-2f-uart

