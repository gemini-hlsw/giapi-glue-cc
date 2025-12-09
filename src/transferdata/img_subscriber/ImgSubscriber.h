#ifndef GIAPI_IMGSUBSCRIBER_H
#define GIAPI_IMGSUBSCRIBER_H

#include <string>
#include <tr1/memory>
#include <vector>

namespace giapi {
    namespace transferdata {
        namespace img_subscriber{
            /**
             * @class ImgSubscriber
             * @brief Abstract interface for image subscribers.
             *
             * Implementations receive image payloads (as byte vectors) from a transport
             * (e.g. JMS/ActiveMQ) and dispatch them to user-provided callbacks.
             * Methods throw on fatal errors (see noexcept(false) signatures).
             */
            class ImgSubscriber {
            public:
                /**
                 * @brief Start asynchronous reception of image bytes.
                 * @param detID Detector (topic) identifier to subscribe to receive images.
                 * @param callback Non-owning function pointer invoked with the received data bytes.
                 * @param ack If true, subscriber will emit acknowledgements as required by the transport.
                 * @return Status code (0 on success, non-zero on failure).
                 * @throws std::runtime_error on unrecoverable errors.
                 */
                virtual void receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&), const bool ack)=0;

                /**
                 * @brief Start asynchronous reception and provide a timestamp with the payload.
                 * @param detID Detector (topic) identifier to subscribe to receive images.
                 * @param callback Non-owning function pointer invoked with the received data bytes and a timestamp (microseconds).
                 * @param ack If true, subscriber will emit acknowledgements as required by the transport.
                 * @return Status code (0 on success, non-zero on failure).
                 * @throws std::runtime_error on unrecoverable errors.
                 */
                virtual void receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&, u_int64_t), const bool ack) =0;

                /**
                 * @brief Stop receiving messages and release any resources associated with reception.
                 *
                 * After calling stopReceive(), no further callbacks should be invoked.
                 */
                virtual void stopReceive() noexcept(false) = 0;
            };

            /**
             * @typedef pImgSubscriber
             * @brief Shared pointer type for ImgSubscriber implementations.
             */
        typedef std::tr1::shared_ptr<ImgSubscriber> pImgSubscriber;
        }
    }
} // namespace giapi

#endif // GIAPI_IMGSUBSCRIBER_H
