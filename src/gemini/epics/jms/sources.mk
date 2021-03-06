# Add inputs and outputs from these tool invocations to the build variables 
OBJS += $(patsubst %.cpp,%.o,$(wildcard ./src/gemini/epics/jms/*.cpp))

CPP_DEPS += $(patsubst %.cpp,%.d,$(wildcard ./src/gemini/epics/jms/*.cpp))

# Each subdirectory must supply rules for building sources it contributes
src/gemini/epics/jms/%.o: ../src/gemini/epics/jms/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking $(OS) C++ Compiler'
	$(CXX) $(INC_DIRS) -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"../$(@:%.o=%.d)" -MT"../$(@:%.o=%.d)" -o"../$@" "$<"
	@echo 'Finished building: $<'
	@echo ' ' 
