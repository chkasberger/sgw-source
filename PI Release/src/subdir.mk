################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Map.cpp \
../src/Modbus.cpp \
../src/Port.cpp \
../src/Register.cpp \
../src/SGW.cpp 

OBJS += \
./src/Map.o \
./src/Modbus.o \
./src/Port.o \
./src/Register.o \
./src/SGW.o 

CPP_DEPS += \
./src/Map.d \
./src/Modbus.d \
./src/Port.d \
./src/Register.d \
./src/SGW.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-unknown-linux-gnueabi-g++ -I/home/ptse/tool/Raspberry-GCC-4.7.3/rootfs/usr/local/lib -I/home/ptse/tool/Raspberry-GCC-4.7.3/rootfs/usr/lib -I/home/ptse/tool/Raspberry-GCC-4.7.3/rootfs/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" -lmodbus -std=c++11 -lpthread
	@echo 'Finished building: $<'
	@echo ' '


