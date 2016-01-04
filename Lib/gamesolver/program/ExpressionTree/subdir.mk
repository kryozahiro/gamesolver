################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/program/ExpressionTree/ConstantNode.cpp \
../gamesolver/program/ExpressionTree/ExpressionNode.cpp \
../gamesolver/program/ExpressionTree/ExpressionTree.cpp \
../gamesolver/program/ExpressionTree/OperatorNode.cpp \
../gamesolver/program/ExpressionTree/VariableNode.cpp 

OBJS += \
./gamesolver/program/ExpressionTree/ConstantNode.o \
./gamesolver/program/ExpressionTree/ExpressionNode.o \
./gamesolver/program/ExpressionTree/ExpressionTree.o \
./gamesolver/program/ExpressionTree/OperatorNode.o \
./gamesolver/program/ExpressionTree/VariableNode.o 

CPP_DEPS += \
./gamesolver/program/ExpressionTree/ConstantNode.d \
./gamesolver/program/ExpressionTree/ExpressionNode.d \
./gamesolver/program/ExpressionTree/ExpressionTree.d \
./gamesolver/program/ExpressionTree/OperatorNode.d \
./gamesolver/program/ExpressionTree/VariableNode.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/program/ExpressionTree/%.o: ../gamesolver/program/ExpressionTree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -pedantic -Wall -Wextra -pipe -c -fmessage-length=0 -Wno-unused-parameter -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


