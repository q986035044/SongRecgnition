################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/794QYH/EE/MCU/CH32V307VCT6/EVT/EXAM/SRC/Core/core_riscv.c 

OBJS += \
./Core/core_riscv.o 

C_DEPS += \
./Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/core_riscv.o: D:/794QYH/EE/MCU/CH32V307VCT6/EVT/EXAM/SRC/Core/core_riscv.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Debug" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Core" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\VoiceRcgExam\VoiceRcg\User" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
