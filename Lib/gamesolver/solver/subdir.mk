################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/solver/Evaluator.cpp \
../gamesolver/solver/GeneticAlgorithm.cpp \
../gamesolver/solver/ProblemSolver.cpp \
../gamesolver/solver/SimulatedAnnealing.cpp \
../gamesolver/solver/Solution.cpp \
../gamesolver/solver/SolutionHistory.cpp \
../gamesolver/solver/TerminationCriteria.cpp 

OBJS += \
./gamesolver/solver/Evaluator.o \
./gamesolver/solver/GeneticAlgorithm.o \
./gamesolver/solver/ProblemSolver.o \
./gamesolver/solver/SimulatedAnnealing.o \
./gamesolver/solver/Solution.o \
./gamesolver/solver/SolutionHistory.o \
./gamesolver/solver/TerminationCriteria.o 

CPP_DEPS += \
./gamesolver/solver/Evaluator.d \
./gamesolver/solver/GeneticAlgorithm.d \
./gamesolver/solver/ProblemSolver.d \
./gamesolver/solver/SimulatedAnnealing.d \
./gamesolver/solver/Solution.d \
./gamesolver/solver/SolutionHistory.d \
./gamesolver/solver/TerminationCriteria.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/solver/%.o: ../gamesolver/solver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -pedantic -Wall -Wextra -pipe -c -fmessage-length=0 -Wno-unused-parameter -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


