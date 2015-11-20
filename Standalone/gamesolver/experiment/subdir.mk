################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/experiment/Experiment.cpp \
../gamesolver/experiment/GeneratorStage.cpp \
../gamesolver/experiment/Output.cpp \
../gamesolver/experiment/SolverStage.cpp \
../gamesolver/experiment/ValidatorStage.cpp 

OBJS += \
./gamesolver/experiment/Experiment.o \
./gamesolver/experiment/GeneratorStage.o \
./gamesolver/experiment/Output.o \
./gamesolver/experiment/SolverStage.o \
./gamesolver/experiment/ValidatorStage.o 

CPP_DEPS += \
./gamesolver/experiment/Experiment.d \
./gamesolver/experiment/GeneratorStage.d \
./gamesolver/experiment/Output.d \
./gamesolver/experiment/SolverStage.d \
./gamesolver/experiment/ValidatorStage.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/experiment/%.o: ../gamesolver/experiment/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -DGAMESOLVER_STANDALONE -O3 -march=native -fopenmp -ffast-math -Wall -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -MMD -MT "$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


