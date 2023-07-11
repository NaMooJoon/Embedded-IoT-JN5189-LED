################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_jn5189.c 

C_DEPS += \
./startup/startup_jn5189.d 

OBJS += \
./startup/startup_jn5189.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_JN5189HN -DCPU_JN5189HN_cm4 -DCPU_JN5189 -DJN5189DK6 -DDK6 -DCPU_JN518X -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\nxp\Workspace\jn5189dk6_led_blinky\source" -I"C:\nxp\Workspace\jn5189dk6_led_blinky" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\drivers" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\device" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\utilities" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\component\serial_manager" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\component\lists" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\component\uart" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\CMSIS" -I"C:\nxp\Workspace\jn5189dk6_led_blinky\board" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_jn5189.d ./startup/startup_jn5189.o

.PHONY: clean-startup

