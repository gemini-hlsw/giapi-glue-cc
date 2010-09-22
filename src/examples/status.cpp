/**
 * Example application to send status updates to Gemini
 */

#include <iostream>
#include <signal.h>
#include <sys/time.h>

#include <decaf/util/concurrent/CountDownLatch.h>

#include <giapi/GiapiErrorHandler.h>
#include <giapi/StatusUtil.h>
#include <giapi/GiapiUtil.h>


using namespace giapi;



void terminate(int signal) {
	std::cout << "Exiting... " << std::endl;
	exit(1);
}

int main(int argc, char **argv) {
 	struct timeval start, end;
    	long seconds, useconds;    
	double time;
	double throughput;
	int nReps = 10000;
	try {

		std::cout << "Starting Status Example" << std::endl;

		signal(SIGABRT, terminate);
		signal(SIGTERM, terminate);
		signal(SIGINT, terminate);


		decaf::util::concurrent::CountDownLatch lock(1);

		StatusUtil::createStatusItem("gpi:status1", type::INT);

		StatusUtil::createStatusItem("gpi:status2", type::INT);
	
    		gettimeofday(&start, NULL);
		for (int i = 0; i < nReps; i++) {
			StatusUtil::setValueAsInt("gpi:status1", i);
			StatusUtil::setValueAsInt("gpi:status2", nReps-i);
			StatusUtil::postStatus();
		}
    		gettimeofday(&end, NULL);

    		seconds  = end.tv_sec  - start.tv_sec;
    		useconds = end.tv_usec - start.tv_usec;

    		time = seconds + useconds/1000000.0;

		throughput = double(nReps * 2) / time;

		std::cout << "Elapsed Time: " << time << " [sec]" << std::endl;
		std::cout << "Throughput  : " << throughput << " [msg/sec]" << std::endl;


	} catch (GmpException &e) {
		std::cerr << e.getMessage() <<  ". Is the GMP up?" << std::endl;
	}
	return 0;
}

