################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/framebuffer/FrameBuffer.cpp 

CPP_DEPS += \
./src/framebuffer/FrameBuffer.d 

OBJS += \
./src/framebuffer/FrameBuffer.o 


# Each subdirectory must supply rules for building sources it contributes
src/framebuffer/%.o: ../src/framebuffer/%.cpp src/framebuffer/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/include -O3 -g3 -Wall -c -fmessage-length=0 -std=c++20 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-framebuffer

clean-src-2f-framebuffer:
	-$(RM) ./src/framebuffer/FrameBuffer.d ./src/framebuffer/FrameBuffer.o

.PHONY: clean-src-2f-framebuffer

