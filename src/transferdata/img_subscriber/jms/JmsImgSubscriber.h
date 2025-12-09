#ifndef GIAPI_JMSIMGSUBSCRIBER_H
#define GIAPI_JMSIMGSUBSCRIBER_H

#include "transferdata/img_subscriber/ImgSubscriber.h"
#include "util/jms/JmsConsumer.h"

namespace giapi {
    namespace transferdata {
        namespace img_subscriber {
            namespace jms {
                const short TIMEOUT = 2000;
                /**
                 * @class JmsImgSubscriber
                 * @brief ActiveMQ-based image Subscriber using JMS.
                 */
                class JmsImgSubscriber : public ImgSubscriber, public util::jms::JmsConsumer {
                public:
                    /**
                     * @brief Construct a JmsImgSubscriber for a specific detector.
                     * @param detID Detector identifier used to derive the JMS topic name.
                     */
                    explicit JmsImgSubscriber(const std::string& detID);

                    /**
                     * @brief Destructor, releases any resources owned by the subscriber.
                     */
                    virtual ~JmsImgSubscriber();

                    /**
                     * @brief Factory method to create a shared pointer to a subscriber.
                     * @param detID Detector identifier for the created subscriber.
                     * @return A pImgSubscriber instance or throws on error.
                     */
                    static pImgSubscriber create(const std::string& detID) noexcept(false);

                   /**
                     * @brief Register a callback to receive image bytes asynchronously.
                     * @param detID Detector identifier to subscribe to.
                     * @param callback Function invoked when image data arrives (byte vector).
                     * @param ack If true, messages will be acknowledged according to JMS semantics.
                     * @return Status code (0 on success, non-zero on failure).
                     *
                     * This starts asynchronous reception; the callback runs on the JMS message thread.
                     */
                    void receiveImage(const std::string& detID, 
                                    void (*callback)(const std::vector<unsigned char>&), 
                                    const bool ack);

                    /**
                     * @brief Register a callback to receive image bytes and a timestamp.
                     * @param detID Detector identifier to subscribe to.
                     * @param callback Function invoked when image data arrives (bytes and timestamp when the message arrived).
                     * @param ack If true, messages will be acknowledged according to JMS semantics.
                     * @return Status code (0 on success, non-zero on failure).
                     *
                     * The timestamp argument is a u_int64_t representing the image time/metadata.
                     */
                    void receiveImage(const std::string& detID, 
                                     void (*callback)(const std::vector<unsigned char>&, u_int64_t), 
                                     const bool ack);                   
                    /**
                        * @brief Stops listening for messages and shuts down the consumer.
                        *
                        * After calling stopReceive(), no further callbacks will be invoked.
                    */
                    virtual void stopReceive();

                    /**
                        * @brief JMS message handler invoked when a message is delivered.
                        * @param message The received JMS Message pointer (read-only).
                        *
                        * This method is called by the underlying JMS client when a message
                        * for this subscriber is delivered. It is responsible for extracting
                        * payload and invoking the user-provided callback.
                    */
                    virtual void onMessage(const Message* message);

                private:
                    /**
                     * Callback invoked with the raw image bytes when a message arrives.
                     * Non-owning: function pointer provided by the caller.
                     */
                    void (*_callback)(const std::vector<unsigned char>&);

                    /**
                     * Callback invoked with the raw image bytes and a timestamp (microseconds).
                     * Timestamp semantics: microseconds representing the message/image delay.
                     * Non-owning: function pointer provided by the caller.
                     */
                    void (*_callbackWithTimestamp)(const std::vector<unsigned char>&, u_int64_t);

                    /**
                     * @brief Internal helper to set up ack-related state and publisher if needed.
                     * @param detID Detector identifier.
                     * @param ack True to enable acknowledgements, false to disable.
                     */
                    void _setAck(const std::string& detID, const bool ack);

                    /**
                     * @brief Initialize the JMS consumer for the given detector. 
                     * @param detID Detector identifier.
                     * @param ack Whether to acknowledge messages.
                     * @throws on failure to create or start the consumer.
                     */
                    void _initConsumer(const std::string& detID, const bool ack) noexcept(false);

                    /**
                     * @brief Internal helper to validate callback state before starting reception.
                     * @tparam CallbackType Type of callback function.
                     * @param callback The callback to validate/store.
                     * @return 0 on success, non-zero on validation failure.
                     *
                     * Ensures only one callback is registered and that the subscriber is
                     * not already receiving.
                     */
                    template<typename CallbackType>
                    int  _checkReceiving (CallbackType callback) noexcept(false);

                    bool _isReceiving;
                    bool _ack;
                    std::string _detID;
                    std::unique_ptr<cms::MessageProducer> _ackPublisher;

                    static log4cxx::LoggerPtr _logger;
                };

                #include "JmsImgSubscriber.i"

            } // namespace jms
        } // namespace img_subscriber
    } // namespace transferdata
} // namespace giapi

#endif // GIAPI_JMSIMGSUBSCRIBER_H
