#include <iostream>
#include <iomanip>

#include <giapi/GeminiUtil.h>
#include <string.h>
#include <ctime>
#include <chrono>

using namespace giapi;

using namespace std;


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
		int status = GeminiUtil::tcsApplyOffset(p,q, offsetType, 50000);
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end-start;
		cout << "Elapsed time sending the offset is: "<< elapsed_seconds.count() << " seconds" << endl;

		if (status == status::ERROR) {
			cout << "Can't apply the offset to TCS..." << endl;
			return 0;
		}

		cout<<"Was the TCS OFFSET applied: " << (status == 1) << endl;

	} catch (GiapiException &e) {
		cout << "exception " << endl;
		cerr << e.what() << endl;
	}
	return 0;
}
