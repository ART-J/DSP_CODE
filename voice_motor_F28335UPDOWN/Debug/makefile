################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := E:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.2

ORDERED_OBJS += \
"./DSP2833x_ADC_cal.obj" \
"./DSP2833x_Adc.obj" \
"./DSP2833x_CodeStartBranch.obj" \
"./DSP2833x_CpuTimers.obj" \
"./DSP2833x_DefaultIsr.obj" \
"./DSP2833x_ECap.obj" \
"./DSP2833x_EPwm.obj" \
"./DSP2833x_EQep.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DSP2833x_Gpio.obj" \
"./DSP2833x_MemCopy.obj" \
"./DSP2833x_PieCtrl.obj" \
"./DSP2833x_PieVect.obj" \
"./DSP2833x_Sci.obj" \
"./DSP2833x_Spi.obj" \
"./DSP2833x_SysCtrl.obj" \
"./DSP2833x_usDelay.obj" \
"./adc.obj" \
"./at25512.obj" \
"./cap5.obj" \
"./cap6.obj" \
"./cuptimer.obj" \
"./epwmdb2.obj" \
"./eqepwm1.obj" \
"./freq.obj" \
"./gpio48.obj" \
"./key14.obj" \
"./key34.obj" \
"./lcd12864spi.obj" \
"./main.obj" \
"./sci.obj" \
"./speed.obj" \
"../Cmd/28335_RAM_lnk.cmd" \
"../Cmd/DSP2833x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-l"E:\ti\ccsv6\tools\compiler\ti-cgt-c2000_6.4.2\lib\rts2800_fpu32.lib" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Cmd/subdir_vars.mk
-include subdir_rules.mk
-include Cmd/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
voice_motor_F28335UPDOWN.out \

EXE_OUTPUTS__QUOTED += \
"voice_motor_F28335UPDOWN.out" \

BIN_OUTPUTS += \
voice_motor_F28335UPDOWN.hex \

BIN_OUTPUTS__QUOTED += \
"voice_motor_F28335UPDOWN.hex" \


# All Target
all: voice_motor_F28335UPDOWN.out

# Tool invocations
voice_motor_F28335UPDOWN.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"E:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.2/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --advice:performance=all -g --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=full -z -m"voice_motor_F28335UPDOWN.map" --stack_size=0x300 --warn_sections -i"E:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.2/lib" -i"E:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.2/include" -i"E:/ti/controlSUITE/device_support/f2833x/v132/DSP2833x_common/cmd" -i"E:/ti/controlSUITE/device_support/f2833x/v132/DSP2833x_headers/cmd" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="voice_motor_F28335UPDOWN_linkInfo.xml" --rom_model -o "voice_motor_F28335UPDOWN.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

voice_motor_F28335UPDOWN.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"E:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.2/bin/hex2000"  -o "voice_motor_F28335UPDOWN.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "DSP2833x_Adc.pp" "DSP2833x_CpuTimers.pp" "DSP2833x_DefaultIsr.pp" "DSP2833x_ECap.pp" "DSP2833x_EPwm.pp" "DSP2833x_EQep.pp" "DSP2833x_GlobalVariableDefs.pp" "DSP2833x_Gpio.pp" "DSP2833x_MemCopy.pp" "DSP2833x_PieCtrl.pp" "DSP2833x_PieVect.pp" "DSP2833x_Sci.pp" "DSP2833x_Spi.pp" "DSP2833x_SysCtrl.pp" "adc.pp" "at25512.pp" "cap5.pp" "cap6.pp" "cuptimer.pp" "epwmdb2.pp" "eqepwm1.pp" "freq.pp" "gpio48.pp" "key14.pp" "key34.pp" "lcd12864spi.pp" "main.pp" "sci.pp" "speed.pp" 
	-$(RM) "DSP2833x_ADC_cal.obj" "DSP2833x_Adc.obj" "DSP2833x_CodeStartBranch.obj" "DSP2833x_CpuTimers.obj" "DSP2833x_DefaultIsr.obj" "DSP2833x_ECap.obj" "DSP2833x_EPwm.obj" "DSP2833x_EQep.obj" "DSP2833x_GlobalVariableDefs.obj" "DSP2833x_Gpio.obj" "DSP2833x_MemCopy.obj" "DSP2833x_PieCtrl.obj" "DSP2833x_PieVect.obj" "DSP2833x_Sci.obj" "DSP2833x_Spi.obj" "DSP2833x_SysCtrl.obj" "DSP2833x_usDelay.obj" "adc.obj" "at25512.obj" "cap5.obj" "cap6.obj" "cuptimer.obj" "epwmdb2.obj" "eqepwm1.obj" "freq.obj" "gpio48.obj" "key14.obj" "key34.obj" "lcd12864spi.obj" "main.obj" "sci.obj" "speed.obj" 
	-$(RM) "DSP2833x_ADC_cal.pp" "DSP2833x_CodeStartBranch.pp" "DSP2833x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

