################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/GCC/RISC-V/port.c 

S_UPPER_SRCS += \
../FreeRTOS/portable/GCC/RISC-V/portASM.S 

OBJS += \
./FreeRTOS/portable/GCC/RISC-V/port.o \
./FreeRTOS/portable/GCC/RISC-V/portASM.o 

S_UPPER_DEPS += \
./FreeRTOS/portable/GCC/RISC-V/portASM.d 

C_DEPS += \
./FreeRTOS/portable/GCC/RISC-V/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/GCC/RISC-V/%.o: ../FreeRTOS/portable/GCC/RISC-V/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused  -g -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Debug" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Core" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\User" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Peripheral\inc" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\include" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\Common" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\GCC\RISC-V" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\GCC\RISC-V\chip_specific_extensions\RV32I_PFIC_no_extensions" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\MemMang" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
FreeRTOS/portable/GCC/RISC-V/%.o: ../FreeRTOS/portable/GCC/RISC-V/%.S
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused  -g -x assembler-with-cpp -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\include" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\Common" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\MemMang" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\GCC\RISC-V" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\FreeRTOS\portable\GCC\RISC-V\chip_specific_extensions\RV32I_PFIC_no_extensions" -I"C:\Github\CH32V203-FreeRTOS\CH32V203-FreeRTOS\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

