-include src/transferdata/img_publisher/jms/sources.mk
# Add inputs and outputs from these tool invocations to the build variables 
OBJS += $(patsubst %.cpp,%.o,$(wildcard ./src/transferdata/img_publisher/*.cpp))

CPP_DEPS += $(patsubst %.cpp,%.d,$(wildcard ./src/transferdata/img_publisher/*.cpp))

# Each subdirectory must supply rules for building sources it contributes
src/transferdata/img_publisher/%.o: ../src/transferdata/img_publisher/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking $(OS) C++ Compiler'
	$(CXX) $(INC_DIRS) -std=c++14 -Wall -c -fmessage-length=0 -MMD -MP -MF"../$(@:%.o=%.d)" -MT"../$(@:%.o=%.d)" -o"../$@" "$<"
	@echo 'Finished building: $<'
	@echo ' ' 