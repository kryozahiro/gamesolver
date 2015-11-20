################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/program/IoMapping/GrayCodeMapping.cpp \
../gamesolver/program/IoMapping/IoMapping.cpp 

OBJS += \
./gamesolver/program/IoMapping/GrayCodeMapping.o \
./gamesolver/program/IoMapping/IoMapping.o 

CPP_DEPS += \
./gamesolver/program/IoMapping/GrayCodeMapping.d \
./gamesolver/program/IoMapping/IoMapping.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/program/IoMapping/%.o: ../gamesolver/program/IoMapping/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -DGAMESOLVER_STANDALONE -O3 -march=native -fopenmp -ffast-math -Wall -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -MMD -MT "$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


