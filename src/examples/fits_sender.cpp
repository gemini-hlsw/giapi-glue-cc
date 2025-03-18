#include "ScorpioData.h"
#include <giapi/InstTransferData.h>
#include <giapi/giapiexcept.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#define NUM_ITERATIONS 500

// Structure to hold timing data
struct PerfData {
    int iteration;
    uint64_t fileSize;
    uint64_t serializeTime;
    uint64_t sendTime;
};

// Store results
std::vector<PerfData> perfResults;

// Logger for this example
static log4cxx::LoggerPtr exampleLogger(log4cxx::Logger::getLogger("giapi.examples.fits_sender"));

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " <fits_file_path>" << std::endl;
}

int sendFitsFile(const std::string& fitsPath, const std::string& detID, int it)
{
    try {

        std::ifstream file(fitsPath, std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
           LOG4CXX_ERROR(exampleLogger, "ERROR: Unable to open FITS file: " << fitsPath);
           perror("File open error");  // System-level error
           return 1;
        }

        std::streamsize size = file.tellg();
        if (size <= 0) {
            LOG4CXX_ERROR(exampleLogger, "ERROR: FITS file appears empty or unreadable.");
            return 1;
        }

        file.seekg(0, std::ios::beg);
        std::vector<unsigned char> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
           LOG4CXX_ERROR(exampleLogger, "ERROR: Failed to read FITS file data.");
           std::cerr << "ifstream state: fail=" << file.fail() << ", bad=" << file.bad() << ", eof=" << file.eof() << std::endl;
           return 1;
        }
        LOG4CXX_INFO(exampleLogger, "Successfully read FITS file: " << fitsPath);
        uint64_t ts_startSer = ScorpioData::getCurrentTimestamp();

        // Create FitsData structure
        ScorpioData fitsData(detID, ts_startSer);
        fitsData.data = buffer;
        std::vector<unsigned char> serializedData = fitsData.serialize();
        uint64_t ts_endSer = ScorpioData::getCurrentTimestamp();
        giapi::InstTransferData::sendImage(detID, serializedData, false);
        uint64_t ts_endTransfer = ScorpioData::getCurrentTimestamp();
        // Store results
        perfResults.push_back({it, static_cast<uint64_t>(size), ts_startSer - ts_endSer, ts_endSer - ts_endTransfer});

    } catch (const giapi::GiapiException& e) {
        LOG4CXX_INFO(exampleLogger, "Error: " << e.what());
        return 1;
    }
    return 0;
}

void saveResultsToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    outFile << "Iteration,File Size (bytes),Serialize Time (ms),Send Latency (ms)\n";
    for (const auto& data : perfResults) {
        outFile << data.iteration << "," 
                << data.fileSize << ","
                << data.serializeTime / 1000 << ","
                << data.sendTime / 1000 << "\n";
    }
    outFile.close();
    LOG4CXX_INFO(exampleLogger, "Saved performance results to " << filename);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <fits_file_1> <fits_file_2>" << std::endl;
        return 1;
    }

    // Send both FITS files multiple times
    for (int i =0; i < NUM_ITERATIONS; i++) {
        sendFitsFile(argv[1], "detH1", i);
        sendFitsFile(argv[2], "detH2", i);
    }

    // Save results to a CSV file
    saveResultsToFile("sender_performance.csv");

    return 0;
} 