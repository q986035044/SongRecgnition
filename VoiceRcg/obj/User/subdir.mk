################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Get_Data.c \
../User/ch32v30x_it.c \
../User/es8388.c \
../User/lcd.c \
../User/main.c \
../User/system_ch32v30x.c 

OBJS += \
./User/Get_Data.o \
./User/ch32v30x_it.o \
./User/es8388.o \
./User/lcd.o \
./User/main.o \
./User/system_ch32v30x.o 

C_DEPS += \
./User/Get_Data.d \
./User/ch32v30x_it.d \
./User/es8388.d \
./User/lcd.d \
./User/main.d \
./User/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Debug" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Core" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\VoiceRcgExam\VoiceRcg\User" -I"D:\794QYH\EE\MCU\CH32V307VCT6\EVT\EXAM\SRC\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

