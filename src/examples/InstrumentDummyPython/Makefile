# Rule for building objects
-include ../../../conf/config.mk
-include ../../../conf/common.mk

INC_DIRS= -I../../../ -I../../ -I$(ACTIVEMQ_INCLUDE) -I$(APR_INCLUDE) -I$(APRUTIL_INCLUDE) -I$(LOG4CXX_INCLUDE) -I$(CURLPP_INCLUDE) -I$(CURL_INCLUDE)

LIB_DIRS= -L../../../ -L$(ACTIVEMQ_LIB) -L$(LOG4CXX_LIB) -L$(APR_LIB) -L$(APRUTIL_LIB) -L$(CURLPP_LIB) -L$(CURLPP_LIB64)  -L$(CURL_LIB) -L$(CURL_LIB64)

LIBS= -lgiapi-glue-cc -lactivemq-cpp -llog4cxx -lapr-1 -lcurlpp -lcurl

LD_LIBRARY_PATH := ../../../:$(LOG4CXX_LIB):$(CPPUNIT_LIB):$(ACTIVEMQ_LIB):$(APR_LIB):$(APRUTIL_LIB):$(CURLPP_LIB)

all: InstDummyTest 


InstDummyTest: InstDummyTest.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking $(OS) C++ Compiler'
	$(CXX) $(INC_DIRS) -std=c++11 -g -O0 -Wall -fPIC -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%: %.o
	$(CXX) $(LIB_DIRS) $(LIBS) -o $@  $<

run: 
	@echo $(LD_LIBRARY_PATH)


clean: 
	-$(RM) *.o *.d InstDummyTest
