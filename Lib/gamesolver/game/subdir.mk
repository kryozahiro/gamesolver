################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/game/AgentGame.cpp \
../gamesolver/game/AverageAdaptor.cpp \
../gamesolver/game/Game.cpp 

OBJS += \
./gamesolver/game/AgentGame.o \
./gamesolver/game/AverageAdaptor.o \
./gamesolver/game/Game.o 

CPP_DEPS += \
./gamesolver/game/AgentGame.d \
./gamesolver/game/AverageAdaptor.d \
./gamesolver/game/Game.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/game/%.o: ../gamesolver/game/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -pedantic -Wall -Wextra -pipe -c -fmessage-length=0 -Wno-unused-parameter -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


