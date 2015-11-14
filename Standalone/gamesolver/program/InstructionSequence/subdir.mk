################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/program/InstructionSequence/Instruction.cpp \
../gamesolver/program/InstructionSequence/InstructionSequence.cpp \
../gamesolver/program/InstructionSequence/ProgramState.cpp 

OBJS += \
./gamesolver/program/InstructionSequence/Instruction.o \
./gamesolver/program/InstructionSequence/InstructionSequence.o \
./gamesolver/program/InstructionSequence/ProgramState.o 

CPP_DEPS += \
./gamesolver/program/InstructionSequence/Instruction.d \
./gamesolver/program/InstructionSequence/InstructionSequence.d \
./gamesolver/program/InstructionSequence/ProgramState.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/program/InstructionSequence/%.o: ../gamesolver/program/InstructionSequence/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -Wall -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -MMD -MT "$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


