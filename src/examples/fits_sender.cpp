#include "ScorpioData.h"
#include <giapi/InstTransferData.h>
#include <giapi/giapiexcept.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>

using namespace std;

struct PerfData { 
    uint64_t timeStamp;
    string dataLabel;
    uint64_t fileSize;
    uint64_t compressSize;
    uint64_t serializationT;
    uint64_t compressT;
    uint64_t networkTime;
};

vector<PerfData> perfResults;


void writeToFile(const string& filename, const vector<unsigned char>& buffer) {
    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        cout<< "Unable to write buffer to " << filename<<endl;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    outFile.close();
    cout<< "Saved ScorpioData to " << filename <<endl;
}

ScorpioData generateRandomScorpioData(const string& labelPrefix, int row, int col) {
    ostringstream oss;
    oss << labelPrefix;
    string label = oss.str();

    ScorpioData data(label, row, col);

    mt19937 rng(random_device{}());
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (auto& val : data.data) {
        val = dist(rng);
    }

    // Example WCS metadata
    data.wcs["radesys"] = "ICRS";
    data.wcs["equinox"] = "2000.0";
    data.wcs["ctype1"] = "RA--TAN";
    data.wcs["ctype2"] = "DEC-TAN";
    data.wcs["wat001"] = "This is a WCS";
    data.wcs["wat002"] = "";
    data.wcs["wat003"] = "";
    data.wcs["crval1"] = to_string(dist(rng));
    data.wcs["crval2"] = to_string(dist(rng));
    data.wcs["crpix1"] = to_string(row/2);
    data.wcs["crpix2"] = to_string(col/2);
    data.wcs["cd1_1"]  = to_string(dist(rng));
    data.wcs["cd1_2"]  = to_string(dist(rng));
    data.wcs["cd2_1"]  = to_string(dist(rng));
    data.wcs["cd2_2"]  = to_string(dist(rng));
    data.wcs["lonpole"] = "180";

    return data;
}

void saveResultsToFile(const string& filename) {
    ofstream outFile(filename);
    outFile << "TimestampPackage;Data Label;Data Size;Data Size Cmp;Serialization Time (ms);Compress Time;Network Time (ms)\n";
    for (const auto& data : perfResults) {
        outFile << data.timeStamp << ";"
                << data.dataLabel << ";"
                << data.fileSize << ";"
                << data.compressSize << ";"
                << data.serializationT / 1000 << ";"
                << data.compressT / 1000 << ";"
                << data.networkTime / 1000 << "\n";
    }
    outFile.close();
    cout<< "Saved performance results to " << filename<< endl;
}

void sleepUntilSecondX(int targetSecond) {
    using namespace chrono;
    auto now = system_clock::now();
    time_t currentTime = system_clock::to_time_t(now);
    tm* timeinfo = localtime(&currentTime);

    // Calculate the target time
    timeinfo->tm_isdst = -1; // Let the library handle DST

    // If the target second is already passed, move to the next minute
    if (targetSecond <= timeinfo->tm_sec) {
        timeinfo->tm_min += 1;
    }
    timeinfo->tm_sec = targetSecond;

    auto targetTime = system_clock::from_time_t(mktime(timeinfo));
    std::cout << "Waiting to reach the " << timeinfo->tm_sec <<  " second, all the thread will start at the same time. " << std::endl;
    // Sleep until the target time
    this_thread::sleep_until(targetTime);
}

int main(int argc, char* argv[]) {
    if (argc > 9 || argc < 7) {
        cout<<"Argc " << argc << endl;
        cerr << "Usage (multiple detectors by count): " << argv[0]
             << " <num_messages> <number_of_detectors> <num_rows> <num_cols> <time_wait_seconds> <isCompressed> [alt_num_rows] [alt_num_cols]\n";
        cerr << "Usage (single detector by label): " << argv[0]
             << " <num_messages> <detector_label> <num_rows> <num_cols> <time_wait_seconds> <isCompressed>\n";
        cerr << "Notes:\n"
             << "  - time_wait_seconds: Number of seconds to wait before sending the nex message.\n"
             << "  - isCompressed: 0 = no compression, 1 = compress using ZSTD.\n"
             << "  - If alt_num_rows/alt_num_cols are provided, the sender will alternate\n"
             << "    between two frame resolutions when creating sample frames. This is\n"
             << "    useful to simulate e.g. NIR and VIS detectors with different\n"
             << "    resolutions: one frame uses <num_rows>x<num_cols>, the other uses\n"
             << "    <alt_num_rows>x<alt_num_cols>.\n";
         return 1;
     }
    
    int numMessages = -1;
    int numDetectos = 1;
    int numRows = -1;
    int numCols = -1;
    int numRows2 = -1;
    int numCols2 = -1;
    uint64_t timeWait = -1;
    string detLabel = "detH";
    int isCompressed = 0;

    try{
        numMessages = stoi(argv[1]);
        numDetectos = stoi(argv[2]);
        numRows2 = numRows = stoi(argv[3]);
        numCols2 = numCols = stoi(argv[4]);
        timeWait = stoi(argv[5]) * 1000000;
        isCompressed = stoi(argv[6]);
        if (argc > 7) {
            numRows2     = stoi(argv[7]);
            numCols2     = stoi(argv[8]);
        }
    } catch (...) {
        try {
            detLabel = argv[2];
            numRows2 = numRows = stoi(argv[3]);
            numCols2 = numCols = stoi(argv[4]);
            timeWait = stoi(argv[5]) * 1000000;
            isCompressed = stoi(argv[6]);
        }catch (...) {

            cerr << "The second to seventh parameters must be a number" << endl;
            cerr << "Usage2: " << argv[0] << " <num_messages> <detector label> <num data rows> <num data columns>" << endl;
            return 1;
        }
    }
    vector<ScorpioData> vectData;
    cout<< "Preparing ScorpioData structures. Number of Detectors: "<< numDetectos << endl;
    sleepUntilSecondX(45);
   
    for (int i=0; i < numDetectos; ++i) {
	string dLabel = (numDetectos > 1) ? detLabel+to_string(i) : detLabel;
        ScorpioData dataTmp = (i%2 == 0) ? 
                              generateRandomScorpioData(dLabel, numRows, numCols) :
                              generateRandomScorpioData(dLabel, numRows2, numCols2);
        dataTmp.dataSerialized = dataTmp.serialize();
        cout<<"Created the " << dataTmp.dataLabel << " detector "<< endl;
        vectData.push_back(dataTmp);
    }

    cout<< "Starting FITS transmission loop. Sending  "<< numMessages << " FITS files in sequence waiting " << timeWait << " seconds" << endl;

    for (int i = 0; i < numMessages; ++i) {
        int nDet = i % numDetectos;
        ScorpioData& selected = vectData[nDet];
        ostringstream oss;
        uint64_t tsStartSerialization = ScorpioData::getCurrentTimestamp();
        vector<unsigned char> dataSer = selected.serialize();
        uint64_t tsEndSerialization = ScorpioData::getCurrentTimestamp();
        vector<unsigned char> dataZSTD;
	    if (isCompressed)
	        dataZSTD = ScorpioData::compress_zstd(dataSer, true, 1);
	    else
	        dataZSTD = ScorpioData::compress_zstd(dataSer, false);
        uint64_t tsCmp = ScorpioData::getCurrentTimestamp();
        giapi::InstTransferData::sendImage(selected.dataLabel, dataZSTD, false);
        uint64_t tsEnd = ScorpioData::getCurrentTimestamp();
        perfResults.push_back({tsEnd, selected.dataLabel, dataSer.size(), dataZSTD.size(),  tsEndSerialization - tsStartSerialization, tsCmp - tsEndSerialization,tsEnd-tsCmp });
        cout<< "Msg: " << i << " dataLabel: " << selected.dataLabel << " noCompress: " << dataSer.size() << " bytes, compressed: " <<  dataZSTD.size()
                       << "Serialization took: " << (tsEndSerialization - tsStartSerialization) / 1000 << " ms "
		       << "Compress took: " << (tsCmp - tsEndSerialization)/1000 << " ms" << endl;
        uint64_t sleepUs = timeWait - (ScorpioData::getCurrentTimestamp() - tsStartSerialization);
	    cout<<"waiting: " << sleepUs <<  " timeWait " << timeWait << " - " <<  (ScorpioData::getCurrentTimestamp() - tsStartSerialization) << endl;
        this_thread::sleep_for(microseconds(sleepUs));	

    }

    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_tm;
    localtime_r(&now_time, &local_tm);
    ostringstream filename;
    auto micros = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch()).count();
    filename << "sender_performance_" << micros << ".csv";

    saveResultsToFile(filename.str());
    for (auto it : vectData)
        it.saveToAsciiFile("/tmp/"+it.dataLabel +"_sent.txt");
    
    cout<< "All FITS messages sent successfully."<<endl;
    return 0;
}
