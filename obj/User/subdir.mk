################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v20x_it.c \
../User/main.c \
../User/system_ch32v20x.c 

OBJS += \
./User/ch32v20x_it.o \
./User/main.o \
./User/system_ch32v20x.o 

C_DEPS += \
./User/ch32v20x_it.d \
./User/main.d \
./User/system_ch32v20x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused  -g -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Debug" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Core" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\User" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Peripheral\inc" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\include" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\Common" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\GCC\RISC-V" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\GCC\RISC-V\chip_specific_extensions\RV32I_PFIC_no_extensions" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\MemMang" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

