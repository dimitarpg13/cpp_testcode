################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Test2.cpp \
../src/concurentqueue.cpp \
../src/concurentqueue_test.cpp 

OBJS += \
./src/Test2.o \
./src/concurentqueue.o \
./src/concurentqueue_test.o 

CPP_DEPS += \
./src/Test2.d \
./src/concurentqueue.d \
./src/concurentqueue_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


