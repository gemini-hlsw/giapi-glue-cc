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
                        // Ensure ActiveMQ connection is initialized
                        //initialize();

                        // Serialize DetectorData
                        //std::vector<unsigned char> binaryData = imgData.serialize();

                        // Create message
                        auto message = std::unique_ptr<BytesMessage>(_session->createBytesMessage());

                        //message->writeBytes(binaryData.data(), binaryData.size());
                        message->writeBytes(binaryData);

                        _producer->send(message.get());

                        std::cout << "Image sent to detector: " << _detID << std::endl;

                        // Blocking behavior: Wait for acknowledgment from subscriber
                        if (blocking) {
                            std::string ackQueueName = "ImageAck." + _detID;
                            std::cout << "Waiting for acknowledgment from subscriber  " << _detID << std::endl;
                            std::unique_ptr<Queue> ackQueue(_session->createQueue(ackQueueName));
                            std::unique_ptr<MessageConsumer> ackConsumer(_session->createConsumer(ackQueue.get()));

                            std::unique_ptr<Message> ackMessage(ackConsumer->receive(TIMEOUT)); // Timeout after 5 seconds
                            if (ackMessage) {
                                std::cout << "Acknowledgment received for detector: " << _detID << std::endl;
                            } else {
                                std::cerr << "Warning: No acknowledgment received within timeout." << std::endl;
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
