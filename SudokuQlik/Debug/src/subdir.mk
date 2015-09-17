################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Generator.cpp \
../src/Solver.cpp \
../src/Sudoku.cpp \
../src/SudokuQlik.cpp 

OBJS += \
./src/Generator.o \
./src/Solver.o \
./src/Sudoku.o \
./src/SudokuQlik.o 

CPP_DEPS += \
./src/Generator.d \
./src/Solver.d \
./src/Sudoku.d \
./src/SudokuQlik.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_DEBUG=1 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


