################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf
LIBS := -lpcap
OBJS := *.c

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: LaBrea6

# Tool invocations
LaBrea6: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -g3 -O0 -o"LaBrea6" $(OBJS) $(USER_OBJS) $(LIBS)
	cp LaBrea6 ../bin/
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_DEPS)$(EXECUTABLES) LaBrea6
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
