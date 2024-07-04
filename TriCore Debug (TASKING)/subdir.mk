################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Cpu0_Main.c \
../Cpu1_Main.c \
../Cpu2_Main.c \
../Driver_Port.c \
../Driver_Stm.c \
../Encoder.c \
../GTM_TOM_PWM.c \
../GtmTomPwmHl.c \
../Scheduler.c \
../UltraSonic.c \
../drive.c \
../uart.c 

COMPILED_SRCS += \
./Cpu0_Main.src \
./Cpu1_Main.src \
./Cpu2_Main.src \
./Driver_Port.src \
./Driver_Stm.src \
./Encoder.src \
./GTM_TOM_PWM.src \
./GtmTomPwmHl.src \
./Scheduler.src \
./UltraSonic.src \
./drive.src \
./uart.src 

C_DEPS += \
./Cpu0_Main.d \
./Cpu1_Main.d \
./Cpu2_Main.d \
./Driver_Port.d \
./Driver_Stm.d \
./Encoder.d \
./GTM_TOM_PWM.d \
./GtmTomPwmHl.d \
./Scheduler.d \
./UltraSonic.d \
./drive.d \
./uart.d 

OBJS += \
./Cpu0_Main.o \
./Cpu1_Main.o \
./Cpu2_Main.o \
./Driver_Port.o \
./Driver_Stm.o \
./Encoder.o \
./GTM_TOM_PWM.o \
./GtmTomPwmHl.o \
./Scheduler.o \
./UltraSonic.o \
./drive.o \
./uart.o 


# Each subdirectory must supply rules for building sources it contributes
%.src: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/user/AURIX-v1.9.20-workspace/20240513/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

Cpu0_Main.o: ./Cpu0_Main.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Cpu1_Main.o: ./Cpu1_Main.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Cpu2_Main.o: ./Cpu2_Main.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Driver_Port.o: ./Driver_Port.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Driver_Stm.o: ./Driver_Stm.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Encoder.o: ./Encoder.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

GTM_TOM_PWM.o: ./GTM_TOM_PWM.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

GtmTomPwmHl.o: ./GtmTomPwmHl.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Scheduler.o: ./Scheduler.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

UltraSonic.o: ./UltraSonic.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drive.o: ./drive.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart.o: ./uart.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./Cpu0_Main.d ./Cpu0_Main.o ./Cpu0_Main.src ./Cpu1_Main.d ./Cpu1_Main.o ./Cpu1_Main.src ./Cpu2_Main.d ./Cpu2_Main.o ./Cpu2_Main.src ./Driver_Port.d ./Driver_Port.o ./Driver_Port.src ./Driver_Stm.d ./Driver_Stm.o ./Driver_Stm.src ./Encoder.d ./Encoder.o ./Encoder.src ./GTM_TOM_PWM.d ./GTM_TOM_PWM.o ./GTM_TOM_PWM.src ./GtmTomPwmHl.d ./GtmTomPwmHl.o ./GtmTomPwmHl.src ./Scheduler.d ./Scheduler.o ./Scheduler.src ./UltraSonic.d ./UltraSonic.o ./UltraSonic.src ./drive.d ./drive.o ./drive.src ./uart.d ./uart.o ./uart.src

.PHONY: clean--2e-

