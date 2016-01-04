################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/program/DataType.cpp \
../gamesolver/program/Program.cpp \
../gamesolver/program/ProgramAdaptor.cpp \
../gamesolver/program/ProgramType.cpp 

OBJS += \
./gamesolver/program/DataType.o \
./gamesolver/program/Program.o \
./gamesolver/program/ProgramAdaptor.o \
./gamesolver/program/ProgramType.o 

CPP_DEPS += \
./gamesolver/program/DataType.d \
./gamesolver/program/Program.d \
./gamesolver/program/ProgramAdaptor.d \
./gamesolver/program/ProgramType.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/program/%.o: ../gamesolver/program/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -pedantic -Wall -Wextra -pipe -c -fmessage-length=0 -Wno-unused-parameter -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


