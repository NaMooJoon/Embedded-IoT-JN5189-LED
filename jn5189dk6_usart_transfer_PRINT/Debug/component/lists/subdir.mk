################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 

OBJS += \
./component/lists/fsl_component_generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_JN5189 -DJN5189DK6 -DDK6 -DCPU_JN518X -DCPU_JN5189HN -DCPU_JN5189HN_cm4 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\source" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\drivers" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\device" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\utilities" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\component\serial_manager" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\component\lists" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\component\uart" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\CMSIS" -I"C:\nxp\Workspace\jn5189dk6_usart_transfer_study_debug_console_PRINT\board" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/fsl_component_generic_list.d ./component/lists/fsl_component_generic_list.o

.PHONY: clean-component-2f-lists

