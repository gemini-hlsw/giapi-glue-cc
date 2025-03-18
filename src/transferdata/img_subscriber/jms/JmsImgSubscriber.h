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
                     * @brief Constructor for JmsImgSubscriber.
                     * @param detID Detector ID used for topic creation.
                     */
                    explicit JmsImgSubscriber(const std::string& detID);

                    /**
                     * @brief Destructor.
                     */
                    virtual ~JmsImgSubscriber();

                    static pImgSubscriber create(const std::string& detID) noexcept(false);

                   /**
                     * @brief Receives an image asynchronously using a callback.
                     * @param detID Detector ID.
                     * @param callback Function to handle received data.
                     * @return Status code (0 for success, nonzero for failure).
                     */

                    int receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false);

                    /**
                        * Stops listening for FITS files
                        */
                    virtual void stopReceive();

                    /**
                        * Callback function that will be called when a message is received
                        * @param message The received JMS message
                        */
                    virtual void onMessage(const Message* message);

                private:
                    void (*_callback)(const std::vector<unsigned char>&);
                    bool _isReceiving;
                };

            } // namespace jms
        } // namespace img_subscriber
    } // namespace transferdata
} // namespace giapi

#endif // GIAPI_JMSIMGSUBSCRIBER_H
