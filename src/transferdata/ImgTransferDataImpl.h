#ifndef GIAPI_IMGTRANSFERDATAIMPL_H
#define GIAPI_IMGTRANSFERDATAIMPL_H

#include <string>
#include <functional>
#include <memory>
#include <log4cxx/logger.h>
#include "img_subscriber/ImgSubscriber.h"
#include "img_publisher/ImgPublisher.h"


namespace giapi {
    /**
     * @class ImgTransferDataImpl
     * @brief Private implementation of InstTransferData for image transfer operations.
     */
    class ImgTransferDataImpl {
    public:

        ImgTransferDataImpl() noexcept(false);
        /**
         * @brief Receives an image asynchronously using a callback.
         * @param detID Detector ID.
         * @param callback Function to handle received data.
         * @return Status code (0 for success, nonzero for failure).
         */
        static int receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false);

        /**
         * @brief Sends an image to the specified detector.
         * @param detID Detector ID.
         * @param binaryData the  binary data as `std::vector<unsigned char>` to be sent.
         * @param blocking If true, blocks until data is received by the client.
         * @return Status code (0 for success, nonzero for failure).
         */
        static int sendImage(const std::string& detID, const std::vector<unsigned char>& binaryData, bool blocking) noexcept(false);
    private:
        //transferdata::img_subscriber::pImgSubscriber _imgSubscriber;
        static transferdata::img_publisher::pImgPublisher _imgPublisher;
        static transferdata::img_subscriber::pImgSubscriber _imgSubscriber;

        /**
         * Logging facility
         */
        static log4cxx::LoggerPtr logger;
    };

} // namespace giapi

#endif // GIAPI_IMGTRANSFERDATAIMPL_H
