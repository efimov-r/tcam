################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/stereo/filter.cpp \
../src/stereo/so.cpp \
../src/stereo/stereo.cpp \
../src/stereo/visualer.cpp 

CPP_DEPS += \
./src/stereo/filter.d \
./src/stereo/so.d \
./src/stereo/stereo.d \
./src/stereo/visualer.d 

OBJS += \
./src/stereo/filter.o \
./src/stereo/so.o \
./src/stereo/stereo.o \
./src/stereo/visualer.o 


# Each subdirectory must supply rules for building sources it contributes
src/stereo/%.o: ../src/stereo/%.cpp src/stereo/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/include -O3 -g3 -Wall -c -fmessage-length=0 -std=c++20 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-stereo

clean-src-2f-stereo:
	-$(RM) ./src/stereo/filter.d ./src/stereo/filter.o ./src/stereo/so.d ./src/stereo/so.o ./src/stereo/stereo.d ./src/stereo/stereo.o ./src/stereo/visualer.d ./src/stereo/visualer.o

.PHONY: clean-src-2f-stereo

