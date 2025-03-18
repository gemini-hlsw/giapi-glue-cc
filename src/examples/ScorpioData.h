
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <chrono>
#include <sstream>

using namespace std::chrono;

class ScorpioData {
public:

    ScorpioData (const std::vector<unsigned char> &binaryData) {
        deserialize(binaryData);
    }
    
    ScorpioData (const std::string &label, uint64_t ts) : dataLabel(label), timestamp(ts) {}

    std::string dataLabel;

    uint64_t timestamp;

    std::vector<unsigned char> data;

    std::vector<unsigned char> serialize() const {
        std::ostringstream stream;

        uint32_t labelSize = dataLabel.size();
        uint32_t dataSize = data.size();

        stream.write(reinterpret_cast<const char *>(&labelSize), sizeof(labelSize));
        stream.write(dataLabel.c_str(), labelSize);
        stream.write(reinterpret_cast<const char *>(&timestamp), sizeof(timestamp));
        stream.write(reinterpret_cast<const char *>(&dataSize), sizeof(dataSize));
        stream.write(reinterpret_cast<const char *>(data.data()), dataSize);

        std::string binaryString = stream.str();
        return std::vector<unsigned char>(binaryString.begin(), binaryString.end());
    }
    
    void deserialize(const std::vector<unsigned char> &binaryData) {
        std::istringstream stream(std::string(binaryData.begin(), binaryData.end()));

        uint32_t labelSize, dataSize;
        stream.read(reinterpret_cast<char *>(&labelSize), sizeof(labelSize));

        dataLabel.resize(labelSize);
        stream.read(&dataLabel[0], labelSize);

        stream.read(reinterpret_cast<char *>(&timestamp), sizeof(timestamp));

        stream.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        data.resize(dataSize);
        stream.read(reinterpret_cast<char *>(data.data()), dataSize);
    }

    static uint64_t getCurrentTimestamp() {
        
        return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    }
};


