#include "JmsImgSubscriber.h"
#include <gmp/GMPKeys.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace cms;
using namespace std;

namespace giapi {
    namespace transferdata {
        namespace img_subscriber {
            namespace jms {
                log4cxx::LoggerPtr JmsImgSubscriber::_logger(log4cxx::Logger::getLogger("giapi.JmsImgSubscriber"));

                /**
                 * @brief Constructor: Initializes the JMS consumer for the specified detector.
                 *
                 * Does not start receiving by itself; it configures the base JmsConsumer.
                 */
                JmsImgSubscriber::JmsImgSubscriber(const std::string& detID) : JmsConsumer(detID, 
                                                                                           cms::Session::AUTO_ACKNOWLEDGE,
                                                                                           true), 
                                                                               _callback(nullptr), 
                                                                               _callbackWithTimestamp(nullptr), 
                                                                               _isReceiving(false) {  
                        // Base class constructor initializes connection to ActiveMQ
                }

                /**
                 * @brief Destructor: ensure consumer is stopped and resources released.
                 */
                JmsImgSubscriber::~JmsImgSubscriber() {
                    if (_isReceiving) {
                        stopReceive();
                    }
                }

                /**
                 * @brief Simple factory: return a managed pointer to a new subscriber.
                 */
                pImgSubscriber JmsImgSubscriber::create(const std::string& detID) noexcept(false){
                    pImgSubscriber imgPub(new JmsImgSubscriber(detID));
                    return imgPub;
                }

                /**
                 * @brief Start the JMS consumer and configure ack behavior.
                 *
                 * Throws on CMS errors. Sets _isReceiving = true on success.
                 */
                void JmsImgSubscriber::_initConsumer(const std::string& detID, const bool ack) noexcept(false) {
                    try {
                        startConsumer();
                        _isReceiving = true;
                        _setAck(detID, ack);
                        LOG4CXX_INFO(_logger, "Starting receiving FITS files from " << _detID);
                    } catch (const CMSException& e) {
                        throw std::runtime_error("Error initializing the consuming FITS files (callback mode): " + e.getMessage());
                    }
                }

                /**
                * @brief Receives an image asynchronously via a callback (no timestamp).
                */
                void JmsImgSubscriber::receiveImage(const std::string& detID, 
                                                   void (*callback)(const std::vector<unsigned char>&), 
                                                   const bool ack) {
                    LOG4CXX_DEBUG(_logger, "Registering callback for receiving images from detector ID: " << detID);
                    try {
                        if (_checkReceiving(callback))
                            throw GiapiException("Client not ready to receave data");

                        _callback = callback;
                        _initConsumer(detID, ack);
                    } catch (...) {
                        LOG4CXX_ERROR(_logger, "Error creating the consumer for the detector ID: " << detID);
                        throw;
                    }
                }
    

                /**
                 * @brief Receives an image asynchronously via a callback that also receives a timestamp.
                 *
                 * Timestamp is delivered as microseconds delay computed from message property.
                 */
                void JmsImgSubscriber::receiveImage(const std::string& detID, 
                                                   void (*callback)(const std::vector<unsigned char>&, u_int64_t), 
                                                   const bool ack) {
                    try {
                      if (_checkReceiving(callback))
                         throw GiapiException("Client not ready to receave data");
                      _callbackWithTimestamp = callback;
                      _initConsumer(detID, ack);
                    }catch (...) {

                      throw;
                    }
                }

                /**
                 * @brief Stop receiving messages and clear callbacks.
                 */
                void JmsImgSubscriber::stopReceive() {
                    if (!_isReceiving) {
                        return;
                    }
                    try {
                        stopConsumer();
                        _isReceiving = false;
                        _callback = nullptr;
                        _callbackWithTimestamp = nullptr;
                        LOG4CXX_INFO(_logger, "Stopping receiving FITS files from detectorID, " << _detID);
                    } catch (const CommunicationException& e) {
                        LOG4CXX_ERROR(_logger, "Error while stopping FITS receiver of the " << _detID << "\n " << e.what());
                    }
                }

                /**
                 * @brief JMS MessageListener callback invoked by the JMS client.
                 *
                 * Extracts bytes from a BytesMessage and dispatches to the registered
                 * callback on a detached thread. Sends an ACK message if configured.
                 */
                void JmsImgSubscriber::onMessage(const Message* message) {
                    if (!_isReceiving || (!_callback) && (!_callbackWithTimestamp))  {
                        return;
                    }
    
                    try {
                        const BytesMessage* bytesMessage = dynamic_cast<const BytesMessage*>(message);
                        uint64_t receivedTimestamp = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count()); // nanoseconds
                        if (bytesMessage == nullptr) {
                            LOG4CXX_WARN(_logger, "Received a empty byte message from " << _detID);
                            return;
                        }
                        
                        // Get the message body size and data
                        const int size = bytesMessage->getBodyLength();
                        std::vector<unsigned char> buffer(size);
                        bytesMessage->readBytes(buffer.data(), size);    
                        uint64_t tsDelay = (_callbackWithTimestamp) ? (receivedTimestamp - bytesMessage->getLongProperty("timestamp")) / 1000 : 0; 
                        
                        // Dispatch processing to a detached thread to avoid blocking JMS thread.
                        std::thread([this, buffer, tsDelay]() {
                            if (_callback)                 
                                _callback(buffer);
                            else {
                                _callbackWithTimestamp(buffer, tsDelay); // tsDelay in microseconds
                            }
                        }).detach();   

                        // Optionally send a lightweight ACK message to a dedicated topic.
                        if (_ack && _ackPublisher) {
                            std::string ackMessage = "ACK:" + _detID;
                            std::unique_ptr<cms::TextMessage> msg(_session->createTextMessage(ackMessage));
                            _ackPublisher->send(msg.get());
                        }               
                    } catch (const CMSException& e) {
                        LOG4CXX_ERROR(_logger, "Error processing message received from " << _detID << "\n " << e.what());
                    } catch (const std::exception& e) {
                        LOG4CXX_ERROR(_logger, "Exception not analyzed in the onMessage function receiving messages from " << _detID << "\n " << e.what());
                    }
                }

                /**
                 * @brief The ack function is used to confirm that a message has been received.
                 */
                void JmsImgSubscriber::_setAck(const std::string& detID, const bool ack) {
                    _ack = ack;
                    _detID = detID;
                    Destination* ackTopic = _session->createTopic(detID + ":ack");
                    _ackPublisher.reset(_session->createProducer(ackTopic));
                    _ackPublisher->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
                }
            } // namespace jms
        } // namespace img_subscriber
    } // namespace transferdata
} // namespace giapi
