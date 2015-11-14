################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/program/NeuralNetwork/FeedforwardNetwork.cpp 

OBJS += \
./gamesolver/program/NeuralNetwork/FeedforwardNetwork.o 

CPP_DEPS += \
./gamesolver/program/NeuralNetwork/FeedforwardNetwork.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/program/NeuralNetwork/%.o: ../gamesolver/program/NeuralNetwork/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -Wall -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -MMD -MT "$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


