#ifndef GIAPI_IMGPUBLISHER_H
#define GIAPI_IMGPUBLISHER_H

#include <string>
#include <tr1/memory>
#include <vector>

namespace giapi {
    namespace transferdata {
        namespace img_publisher{

            /**
             * @interface ImgPublisher
             * @brief Abstract interface for sending binary image payloads through GIAPI.
             *
             * The implementations of this class is responsible for transmitting a binary
             * data buffer (for example FITS image bytes) over the chosen transport
             * (e.g. JMS/ActiveMQ). It keeps the caller decoupled from transport details.
             */
            class ImgPublisher {
            public:
                /**
                 * @brief Send binary image data.
                 *
                 * @param binaryData Byte vector containing the image payload to send.
                 * @param blocking If true, block until the message has been delivered/acknowledged
                 *                 according to the transport semantics. If false, send asynchronously.
                 * @return Status code (0 on success, non-zero on failure).
                 * @throws std::runtime_error on unrecoverable errors.
                 */
                virtual int sendImage(const std::vector<unsigned char>& binaryData, const bool blocking) noexcept(false) = 0;
                virtual ~ImgPublisher() {}
            };

            /**
             * @typedef pImgPublisher
             * @brief Shared pointer type for ImgPublisher implementations.
             */
        typedef std::tr1::shared_ptr<ImgPublisher> pImgPublisher;
        }
    }
} // namespace giapi

#endif // GIAPI_IMGPUBLISHER_H
