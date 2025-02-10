# Rule for building objects

-include ../../conf/config.mk
-include ../../conf/common.mk

INC_DIRS= -I../../ -I../ -I$(ACTIVEMQ_INCLUDE) -I$(APR_INCLUDE) -I$(APRUTIL_INCLUDE) -I$(LOG4CXX_INCLUDE) -I$(CURLPP_INCLUDE) -I$(CURL_INCLUDE)

LIB_DIRS= -L../../ -L$(ACTIVEMQ_LIB) -L$(LOG4CXX_LIB) -L$(APR_LIB) -L$(APRUTIL_LIB) -L$(CURLPP_LIB) -L$(CURLPP_LIB64)  -L$(CURL_LIB) -L$(CURL_LIB64)

LIBS= -lgiapi-glue-cc -lactivemq-cpp -llog4cxx -lapr-1 -lcurlpp -lcurl

LD_LIBRARY_PATH := ../../:$(LOG4CXX_LIB):$(CPPUNIT_LIB):$(ACTIVEMQ_LIB):$(APR_LIB):$(APRUTIL_LIB):$(CURLPP_LIB):$(CURLPP_LIB64)

#all: tcsApplyOffset tcsApplyOffsetCallback seq-command tcscontext pcsupdater obsevent epicssubscription epicsget logging fileevents status services apply-command any-command observe-simulator ghost-split-command igrins2obssimulator
all: seq-command tcscontext pcsupdater obsevent epicssubscription epicsget logging fileevents status services apply-command any-command observe-simulator ghost-split-command igrins2obssimulator

seq-command: seq-command.o


#tcsApplyOffsetCallback: tcsApplyOffsetCallback.o
#	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

#tcsApplyOffset: tcsApplyOffset.o
#	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

tcscontext: tcscontext.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

pcsupdater: pcsupdater.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

obsevent: obsevent.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

epicsget: epicsget.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

fileevents: fileevents.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

logging: logging.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

status: status.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

stresstest: stresstest.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

services: services.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

apply-command: apply-command.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

ghost-split-command: ghost-split-command.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

any-command: any-command.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

engineering: engineering.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

igrins2obssimulator: igrins2obssimulator.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

gpim1zernikes: gpim1zernikes.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

observe-simulator: observe-simulator.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

epicssubscription: epicssubscription.o EpicsHandlerDemo.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $+

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking $(OS) C++ Compiler'
	$(CXX) $(INC_DIRS) -g -O0 -std=c++20 -Wall -fPIC -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%: %.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

clean:
	-$(RM) *.o *.d stresstest seq-command pcsupdater tcscontext obsevent epicssubscription pcsupdater logging fileevents status services apply-command any-command obsacq gpim1zernikes gipsimulatior gpiobssimulator engineering ghost-split-command igrins2obssimulator
run:
	@echo $(LD_LIBRARY_PATH)

