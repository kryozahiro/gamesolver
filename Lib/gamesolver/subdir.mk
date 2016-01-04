################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/Standalone.cpp \
../gamesolver/main.cpp 

OBJS += \
./gamesolver/Standalone.o \
./gamesolver/main.o 

CPP_DEPS += \
./gamesolver/Standalone.d \
./gamesolver/main.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/%.o: ../gamesolver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -pedantic -Wall -Wextra -pipe -c -fmessage-length=0 -Wno-unused-parameter -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


