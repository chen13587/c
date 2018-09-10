################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/server.cpp \
../src/sha1_base64.cpp \
../src/websocket.cpp 

OBJS += \
./src/main.o \
./src/server.o \
./src/sha1_base64.o \
./src/websocket.o 

CPP_DEPS += \
./src/main.d \
./src/server.d \
./src/sha1_base64.d \
./src/websocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/openssl/include -I"/home/chen/桌面/C++ project/eclipse project/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


