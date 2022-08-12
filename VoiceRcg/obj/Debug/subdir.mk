################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/794QYH/EE/MCU/CH32V307VCT6/EVT/EXAM/SRC/Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/debug.o: D:/794QYH/EE/MCU/CH32V307VCT6/EVT/EXAM/SRC/Debug/debug.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Debug" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Core" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\VoiceRcgExam\VoiceRcg\User" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

