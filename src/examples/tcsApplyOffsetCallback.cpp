#include <iostream>
#include <iomanip>

#include <giapi/GeminiUtil.h>
#include <string.h>
#include <ctime>
#include <chrono>
#include<unistd.h>

using namespace giapi;

using namespace std;

bool wasOffsetApplied=false;

void receiveMsg(int wasAplied, string msg) {
	string res = (wasAplied) ? "was applied " : " was not applied. ";
	cout << "the offset : " << res  << msg << endl;
        wasOffsetApplied=true;
}


int main(int argc, char **argv) {
	double p = 2.5;
	double q = 1.5;
	OffsetType offsetType = ACQ;

	if (argc == 1) {
		cout<< "No P, Q and type offset were not passed. The following values will be used by default " << endl;
		cout<<"p: "<< p << " q: " << q << " offsetType: " << ACQ << " (0)-> Acquistion. (1)->SlowGuidingCorrection"<<endl;
	} else {
		if (argc != 4 || ((argc == 2) && strcmp(argv[1], "-h")) ) {
			cout<<"Execution of the tcsApplyOffset. ./tcsApplyOffset [ p q offsetType ] "<< endl;
			cout<<"Paramenters: " << endl;
			cout<<"             p: Size of the P offset. Units in arcseconds. "<< endl;
			cout<<"             q: Size of the P offset. Units in arcseconds. "<< endl;
			cout<<"    offsetType: Type the offset. 0 -> Acquisition stage, 1-> SlowGuidingCorrection "<< endl << endl;
			return 0;
		}
		p = atof(argv[1]);
		q = atof(argv[2]);
		offsetType = argv[3][0] == '0' ? ACQ : SLOWGUIDING;
	}

	try {

           cout << "Starting TCS Apply Offset Example " << endl;
	   cout << "Offset to apply.  p: "<< p << " q: "<< q << " offsetType: "<< offsetType << endl;
	   auto start = std::chrono::system_clock::now();
           int status = GeminiUtil::tcsApplyOffset(p, q, offsetType, 10000, receiveMsg);
           auto end = std::chrono::system_clock::now();
           std::chrono::duration<double> elapsed_seconds = end-start;
	   cout << "The offset was sent: "<< elapsed_seconds.count() << " seconds" << endl;

	   if (status == status::ERROR) {
	      cout << "Can't apply the offset to TCS..." << endl;
	      return 0;
	   }

           int i=0; 
	   cout << "The main process could be execute some other task while the offset is being applying right now" << endl;
	   while (!wasOffsetApplied && i< 10) {
	      sleep(1);
	      i++;
	   }

	} catch (GiapiException &e) {
		cout << "exceptionnnn " << endl;
		cerr << e.what() << endl;
	}
	return 0;
}

