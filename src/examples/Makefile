# Rule for building objects
-include ../../conf/config.mk
-include ../../conf/common.mk

INC_DIRS= -I../../ -I$(ACTIVEMQ_INCLUDE) -I$(APR_INCLUDE)

LIB_DIRS= -L../../ -L$(ACTIVEMQ_LIB) -L$(LOG4CXX_LIB)

LIBS= -lgiapi-glue-cc -lactivemq-cpp -llog4cxx

LD_LIBRARY_PATH := ../../:$(LOG4CXX_LIB):$(CPPUNIT_LIB):$(ACTIVEMQ_LIB)

all: seq-command tcscontext obsevent epicssubscription

seq-command: seq-command.o

tcscontext: tcscontext.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

pcsupdater: pcsupdater.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

obsevent: obsevent.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

epicssubscription: epicssubscription.o EpicsHandlerDemo.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $+

	
%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking $(OS) C++ Compiler'
	$(CXX) $(INC_DIRS) -O3 -Wall -fPIC -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%: %.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

run: 
	@echo $(LD_LIBRARY_PATH)
	
	
clean: 
	-$(RM) *.o *.d seq-command tcscontext obsevent epicssubscription pcsupdater