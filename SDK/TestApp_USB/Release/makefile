################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
TestApp_USB.elf.size \

ELFCHECK += \
TestApp_USB.elf.elfcheck \


# All Target
all: TestApp_USB.elf secondary-outputs

# Tool invocations
TestApp_USB.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: PowerPC gcc linker
	powerpc-eabi-gcc -Wl,-T -Wl,../src/lscript.ld -L../../RobotRacers_bsp_0/ppc405_0/lib -o"TestApp_USB.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

TestApp_USB.elf.size: TestApp_USB.elf
	@echo Invoking: PowerPC Print Size
	powerpc-eabi-size TestApp_USB.elf  |tee "TestApp_USB.elf.size"
	@echo Finished building: $@
	@echo ' '

TestApp_USB.elf.elfcheck: TestApp_USB.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck TestApp_USB.elf -hw ../../VHDL_hw_platform/system.xml -pe ppc405_0  |tee "TestApp_USB.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) TestApp_USB.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
