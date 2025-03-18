#include "transferdata/ImgTransferDataImpl.h"

#include "transferdata/img_publisher/jms/JmsImgPublisher.h"
#include "transferdata/img_subscriber/jms/JmsImgSubscriber.h"

#include <iostream>

namespace giapi {

    // Define static members
    transferdata::img_publisher::pImgPublisher ImgTransferDataImpl::_imgPublisher;
    transferdata::img_subscriber::pImgSubscriber ImgTransferDataImpl::_imgSubscriber;

    log4cxx::LoggerPtr ImgTransferDataImpl::logger(log4cxx::Logger::getLogger("giapi.ImgTransferDataImpl"));

    //pImgTransferImpl ImgTransferDataImpl::INSTANCE(static_cast<ImgTransferDataImpl *>(0));
    
    ImgTransferDataImpl::ImgTransferDataImpl() noexcept(false) { }


    
    

/**
 * @brief Sends an image via JmsImgPublisher.
 */
int ImgTransferDataImpl::sendImage(const std::string& detID, const std::vector<unsigned char>& binaryData, bool blocking) noexcept(false) {
    try {
        if (!_imgPublisher ) 
            _imgPublisher = giapi::transferdata::img_publisher::jms::JmsImgPublisher::create(detID);
        return _imgPublisher->sendImage(binaryData, blocking);
    } catch (const std::exception& e) {
        std::cerr << "Error in sendImage: " << e.what() << std::endl;
        return -1;
    }
}

/**
 * @brief Receives an image asynchronously via ImgSubscriber.
 */
int ImgTransferDataImpl::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false) {
    try {
        // Create an instance of the image subscriber
        if (!_imgSubscriber)
            _imgSubscriber = giapi::transferdata::img_subscriber::jms::JmsImgSubscriber::create(detID);
        return _imgSubscriber->receiveImage(detID, callback);
    } catch (const std::exception& e) {
        std::cerr << "Error in receiveImage (callback mode): " << e.what() << std::endl;
        return -1;
    }
}

}