-include src/transferdata/img_subscriber/sources.mk
-include src/transferdata/img_publisher/sources.mk
# Add inputs and outputs from these tool invocations to the build variables 
OBJS += $(patsubst %.cpp,%.o,$(wildcard ./src/transferdata/*.cpp))

CPP_DEPS += $(patsubst %.cpp,%.d,$(wildcard ./src/transferdata/*.cpp))

# Each subdirectory must supply rules for building sources it contributes
src/gemini/transferdata/%.o: ../src/transferdata/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking $(OS) C++ Compiler'
	$(CXX) $(INC_DIRS) -O3 -std=c++14 -Wall -c -fmessage-length=0 -MMD -MP -MF"../$(@:%.o=%.d)" -MT"../$(@:%.o=%.d)" -o"../$@" "$<"
	@echo 'Finished building: $<'
	@echo ' ' 
