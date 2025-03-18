#include "JmsImgSubscriber.h"
#include <gmp/GMPKeys.h>
#include <iostream>


using namespace cms;

namespace giapi {
    namespace transferdata {
        namespace img_subscriber {
            namespace jms {
                /**
                 * @brief Constructor: Initializes the JMS consumer for the specified detector.
                 */
                JmsImgSubscriber::JmsImgSubscriber(const std::string& detID) : JmsConsumer(detID, cms::Session::AUTO_ACKNOWLEDGE), 
                                                                               _callback(nullptr), _isReceiving(false) {  
                // Base class constructor initializes connection to ActiveMQ
                }

                JmsImgSubscriber::~JmsImgSubscriber() {
                    if (_isReceiving) {
                        stopReceive();
                    }
                }

                pImgSubscriber JmsImgSubscriber::create(const std::string& detID) noexcept(false){
                    pImgSubscriber imgPub(new JmsImgSubscriber(detID));
                    return imgPub;
                }

                /**
                * @brief Receives an image asynchronously via a callback.
                */
                int JmsImgSubscriber::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false) {
                    if (_isReceiving) {
                        std::cout << "Already receiving FITS files"<< std::endl;
                        return status::ERROR;
                    }
    
                    if (!callback) {
                        std::cout << "Callback function cannot be null" << std::endl;
                        return status::ERROR;
                    }
    
                    try {
                        _callback = callback;
                        startConsumer();
                        _isReceiving = true;
                        std::cout<<"Started receiving FITS files"<<std::endl;
                        return 0;  // Success
                    } catch (const CMSException& e) {
                        throw std::runtime_error("Error receiving image (callback mode): " + e.getMessage());
                    }
                }

                void JmsImgSubscriber::stopReceive() {
                    if (!_isReceiving) {
                        return;
                    }
    
                    try {
                        stopConsumer();
                        _isReceiving = false;
                        _callback = nullptr;
                        std::cout<< "Stopped receiving FITS files" << std::endl;
                    } catch (const CommunicationException& e) {
                        std::cout<< "Error while stopping FITS receiver: " << e.what() << std::endl;
                    }
                }

                void JmsImgSubscriber::onMessage(const Message* message) {
                    if (!_isReceiving || !_callback) {
                        return;
                    }
    
                    try {
                        const BytesMessage* bytesMessage = dynamic_cast<const BytesMessage*>(message);
                        if (bytesMessage == nullptr) {
                            std::cout<<"Received message is not a BytesMessage"<<std::endl;
                            return;
                        }
                        std::cout<< "Received message body length: " << bytesMessage->getBodyLength() << " bytes" << std::endl;
                        // Get the message body size and data
                        const int size = bytesMessage->getBodyLength();
                        std::vector<unsigned char> buffer(size);
                        bytesMessage->readBytes(buffer.data(), size);                        
                        _callback(buffer);
                       std::cout<< "Successfully processed FITS file" << std::endl;
    
                    } catch (const CMSException& e) {
                        std::cout<< "Error processing received FITS message: " << e.what() << std::endl;
                    } catch (const std::exception& e) {
                        std::cout<< "Error processing received FITS file: " << e.what()<< std::endl;
                    }
                }

            } // namespace jms
        } // namespace img_subscriber
    } // namespace transferdata
} // namespace giapi
