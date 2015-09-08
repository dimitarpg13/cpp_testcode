################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Solver.cpp \
../src/Sudoku.cpp \
../src/SudokuQlik.cpp 

OBJS += \
./src/Solver.o \
./src/Sudoku.o \
./src/SudokuQlik.o 

CPP_DEPS += \
./src/Solver.d \
./src/Sudoku.d \
./src/SudokuQlik.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


