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


// Logger for this example
static log4cxx::LoggerPtr exampleLogger(log4cxx::Logger::getLogger("giapi.examples.fits_sender"));

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " <fits_file_path>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        // Configure logging
        //log4cxx::BasicConfigurator::configure();

        // Read the FITS file
        std::string fitsPath = argv[1];
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

        uint64_t timestamp = ScorpioData::getCurrentTimestamp();
        // Create FitsData structure
        ScorpioData fitsData("ScorpioData", timestamp);
        fitsData.data = buffer;
        std::cout<< "sending the fitsData " << fitsPath<<std::endl;
        giapi::InstTransferData::sendImage("detH", fitsData.serialize(), false);

    } catch (const giapi::GiapiException& e) {
        LOG4CXX_INFO(exampleLogger, "Error: " << e.what());
        return 1;
    }

    return 0;
} 