################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gamesolver/problem/DataSet.cpp \
../gamesolver/problem/HomoAdaptor.cpp \
../gamesolver/problem/Problem.cpp \
../gamesolver/problem/Regression.cpp \
../gamesolver/problem/SantaFeTrail.cpp \
../gamesolver/problem/TravellingSalesman.cpp 

OBJS += \
./gamesolver/problem/DataSet.o \
./gamesolver/problem/HomoAdaptor.o \
./gamesolver/problem/Problem.o \
./gamesolver/problem/Regression.o \
./gamesolver/problem/SantaFeTrail.o \
./gamesolver/problem/TravellingSalesman.o 

CPP_DEPS += \
./gamesolver/problem/DataSet.d \
./gamesolver/problem/HomoAdaptor.d \
./gamesolver/problem/Problem.d \
./gamesolver/problem/Regression.d \
./gamesolver/problem/SantaFeTrail.d \
./gamesolver/problem/TravellingSalesman.d 


# Each subdirectory must supply rules for building sources it contributes
gamesolver/problem/%.o: ../gamesolver/problem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -O3 -march=native -fopenmp -ffast-math -Wall -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -MMD -MT "$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


