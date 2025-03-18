#include "ScorpioData.h"
#include <giapi/InstTransferData.h>
#include <giapi/giapiexcept.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
#include <csignal>
#include <atomic>

// Structure to hold timing data
struct PerfData {
    uint64_t networkTime;
    uint64_t deserializeTime;
};

// Atomic flag to detect Ctrl+C
std::atomic<bool> running(true);

// Store results
std::vector<PerfData> perfResults;

// Logger for this example
static log4cxx::LoggerPtr exampleLogger(log4cxx::Logger::getLogger("giapi.examples.fits_receiver"));

std::string convertTimestampToHuman(uint64_t timestamp) {
    using namespace std::chrono;

    // Convert uint64_t (microseconds) to std::chrono::system_clock::time_point
    microseconds usTimestamp(timestamp);
    system_clock::time_point tp(usTimestamp);

    // Convert to time_t (seconds)
    std::time_t timeT = system_clock::to_time_t(tp);
    
    // Convert to local time structure
    std::tm localTm;
    localtime_r(&timeT, &localTm);

    // Format into human-readable string
    std::ostringstream oss;
    oss << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S") << "."  << (timestamp % 1000000);  // Append microseconds

    return oss.str();
}

// Callback function to handle received FITS data
void handleFitsData(const std::vector<unsigned char>& binaryData) {
    try {
        std::cout << "Arrived to the binary data, the length is " << binaryData.size() << std::endl;
        uint64_t tsReceived = ScorpioData::getCurrentTimestamp();
        ScorpioData data(binaryData);
        // Check if this is ScorpioData based on the dataLabel
        if (data.dataLabel != "ScorpioData") {
            throw std::runtime_error("Error: Received data is not a ScorpioData instance. Label: " + data.dataLabel);
        }
        
        //std::cout << "Processing ScorpioDatam, ts: " << data.timestamp << std::endl;
        
        uint64_t ts2 = ScorpioData::getCurrentTimestamp();
        uint64_t tsDeserialization = ts2 - tsReceived;
        LOG4CXX_DEBUG(exampleLogger, "Time spent in Serialize, and network: " << (tsReceived - data.timestamp)/1000 << " ms");
        LOG4CXX_DEBUG(exampleLogger, "Time spent in Deserialize: " << tsDeserialization / 1000   << " ms");
        
        std::string filename = "received_" + data.dataLabel + ".fits";
        
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile.is_open()) {
            LOG4CXX_DEBUG(exampleLogger, "Failed to create file: " << filename);
            return;
        }


        LOG4CXX_DEBUG(exampleLogger, "Attempting to write " << data.data.size() << " bytes to file");
        
        // Try writing in smaller chunks to identify where it fails
        const size_t chunkSize = 1024 * 1024; // 1MB chunks
        size_t totalWritten = 0;
        
        while (totalWritten < data.data.size()) {
            size_t remaining = data.data.size() - totalWritten;
            size_t currentChunk = std::min(chunkSize, remaining);
            
            if (!outFile.write(reinterpret_cast<const char*>(data.data.data() + totalWritten), currentChunk)) {
                LOG4CXX_ERROR(exampleLogger, "Failed to write chunk at offset " << totalWritten << ". Error: " << strerror(errno));
                outFile.close();
                return;
            }
            totalWritten += currentChunk;
        }
        
        outFile.close();
        uint64_t ts3 = ScorpioData::getCurrentTimestamp();
        ts3 = ts3 - ts2;
        LOG4CXX_DEBUG(exampleLogger, "Successfully saved FITS file, Spent in saving the fits file: " << ts3 / 1000 << " ms");
        // Store results
        perfResults.push_back({tsReceived - data.timestamp, tsDeserialization});

    // Log performance
        
    } catch (const std::exception& e) {
        LOG4CXX_ERROR(exampleLogger, "Error processing FITS data: " << e.what());
    }
}

// Signal handler for Ctrl+C
void signalHandler(int signal) {
    if (signal == SIGINT) {
        LOG4CXX_INFO(exampleLogger, "Ctrl+C detected! Saving results and exiting...");
        running = false; // Set flag to exit loop
    }
}

// Function to save results to a CSV file
void saveResultsToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    outFile << "Network Time (ms),Deserialize Time (ms)\n";
    for (const auto& data : perfResults) {
        outFile << (data.networkTime)/1000 << ","
                << (data.deserializeTime) / 1000 << "\n";
    }
    outFile.close();
    LOG4CXX_INFO(exampleLogger, "Saved performance results to " << filename);
}

// Thread function to subscribe to a detector
void subscribeToDetector(const std::string& detID) {
    try {
        LOG4CXX_INFO(exampleLogger, "Starting subscription for: " << detID);
        giapi::InstTransferData::receiveImage(detID, handleFitsData);
    } catch (const std::exception& e) {
        LOG4CXX_ERROR(exampleLogger, "Error in subscription for " << detID << ": " << e.what());
    }
}

int main(int argc, char* argv[]) {
    try {
        // Configure logging
        //log4cxx::BasicConfigurator::configure();
        
        LOG4CXX_INFO(exampleLogger, "Starting FITS receiver...");
        LOG4CXX_INFO(exampleLogger, "Waiting for FITS files from scorpio...");
        
        // Subscribe to receive FITS data from scorpio
        std::thread thread1(subscribeToDetector, "detH1");
        std::cout<<"Sleeping" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::thread thread2(subscribeToDetector, "detH2");
        
        // Keep the program running
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        // Save performance results before exiting
        saveResultsToFile("receiver_performance.csv");
        
    } catch (const giapi::GiapiException& e) {
        LOG4CXX_ERROR(exampleLogger, "GiapiException: " << e.what());
        return 1;
    } catch (const std::exception& e) {
        LOG4CXX_ERROR(exampleLogger, "Exception: " << e.what());
        return 1;
    }
    
    return 0;
} 