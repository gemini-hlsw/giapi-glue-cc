#include "JmsImgPublisher.h"
#include <gmp/GMPKeys.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace cms;
using namespace std::chrono_literals;


namespace giapi {
    namespace transferdata {
        namespace img_publisher {
            namespace jms {
                log4cxx::LoggerPtr JmsImgPublisher::_logger(log4cxx::Logger::getLogger("giapi.JmsImgPublisher"));
                /**
                 * @brief Constructor: Initializes the JMS producer for the specified detector.
                 */
                JmsImgPublisher::JmsImgPublisher(const std::string& detID) 
                    : JmsProducer(detID, cms::Session::AUTO_ACKNOWLEDGE), _detID(detID)  {  // 🔹 Now using detID dynamically
                    // Base class constructor initializes connection to ActiveMQ
                }

                pImgPublisher JmsImgPublisher::create(const std::string& detID) noexcept(false) {
                    pImgPublisher imgPub(new JmsImgPublisher(detID));
                    return imgPub;
                }

                

                /**
                 * @brief Sends an image via ActiveMQ with blocking or non-blocking behavior.
                 */
                int JmsImgPublisher::sendImage(const std::vector<unsigned char>& binaryData, const bool blocking) noexcept(false) {
                    try {
                        LOG4CXX_DEBUG(_logger, "Sending image to detector: " << _detID);
                        // Create message
                        auto message = std::unique_ptr<BytesMessage>(_session->createBytesMessage());

                        uint64_t t1 = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
                        message->writeBytes(binaryData);
                        message->setLongProperty("timestamp", static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()));

                        _producer->send(message.get());
                        uint64_t t2 = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count() - t1) / 1000000;
                        
                        // Blocking behavior: Wait for acknowledgment from subscriber
                        if (blocking) {
                            std::string ackQueueName = "ImageAck." + _detID;
                            LOG4CXX_DEBUG(_logger, "Waiting for acknowledgment from subscriber: " << _detID);
                            std::unique_ptr<Queue> ackQueue(_session->createQueue(ackQueueName));
                            std::unique_ptr<MessageConsumer> ackConsumer(_session->createConsumer(ackQueue.get()));

                            std::unique_ptr<Message> ackMessage(ackConsumer->receive(TIMEOUT)); // Timeout after 5 seconds
                            if (ackMessage) {
                                LOG4CXX_DEBUG(_logger, "Acknowledgment received for detector: " << _detID);
                            } else {
                                LOG4CXX_WARN(_logger, "No acknowledgment received within timeout for the detector: " << _detID);
                            }
                            ackConsumer->close();
                        }

                        return 0;  // Success
                    } catch (const CMSException& e) {
                        throw std::runtime_error("Error sending image: " + e.getMessage());
                    }
                }
            }
        }
    }
} 
