################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/vector/vector.cpp 

CPP_DEPS += \
./src/vector/vector.d 

OBJS += \
./src/vector/vector.o 


# Each subdirectory must supply rules for building sources it contributes
src/vector/%.o: ../src/vector/%.cpp src/vector/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/include -O3 -g3 -Wall -c -fmessage-length=0 -std=c++20 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-vector

clean-src-2f-vector:
	-$(RM) ./src/vector/vector.d ./src/vector/vector.o

.PHONY: clean-src-2f-vector

