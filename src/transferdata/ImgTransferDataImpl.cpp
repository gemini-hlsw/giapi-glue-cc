#include "transferdata/ImgTransferDataImpl.h"

#include "transferdata/img_publisher/jms/JmsImgPublisher.h"
#include "transferdata/img_subscriber/jms/JmsImgSubscriber.h"

#include <iostream>

namespace giapi {

    // Define static members

    /**
     * The responsibility of this map is to keep track of active subscribers
     * reusing the subscriber instances for the same detector to avoid
     * creating duplicate JMS subscriptions and wasting resources.
     */
    std::map<std::string, transferdata::img_subscriber::pImgSubscriber> ImgTransferDataImpl::_subscribers;
    /**
     * The responsibility of this map is to keep track of active publishers
     * reusing the publisher instances for the same detector to avoid
     * creating duplicate JMS subscriptions and wasting resources.
     */
    std::map<std::string, transferdata::img_publisher::pImgPublisher> ImgTransferDataImpl::_publishers;
    log4cxx::LoggerPtr ImgTransferDataImpl::logger(log4cxx::Logger::getLogger("giapi.ImgTransferDataImpl"));
    ImgTransferDataImpl::ImgTransferDataImpl() noexcept(false) { }
    

/**
 * @brief Sends an image via JmsImgPublisher.
 */
int ImgTransferDataImpl::sendImage(const std::string& detID, const std::vector<unsigned char>& binaryData, bool blocking) noexcept(false) {
    try {
        auto it = _publishers.find(detID);
        transferdata::img_publisher::pImgPublisher publisher;
        if (it == _publishers.end()) {
            publisher = giapi::transferdata::img_publisher::jms::JmsImgPublisher::create(detID);
            _publishers.insert({detID, std::move(publisher)});
        } else
            publisher = it->second;

        return publisher->sendImage(binaryData, blocking);
    } catch (const std::exception& e) {
        LOG4CXX_ERROR(logger, "Error in sendImage: " << e.what());
        return -1;
    }
}

/**
 * @brief Receives an image asynchronously via ImgSubscriber.
 */
void ImgTransferDataImpl::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&), const bool ack){
    auto it = _subscribers.find(detID);
    LOG4CXX_DEBUG(logger, "Checking existing subscriptions for detector ID: " << detID);
    if (it != _subscribers.end()) {
        LOG4CXX_WARN(logger, "Subscription for " << detID << " already exists. Skipping duplicate subscription.");
        throw GiapiException("You are already subscribed to this");  // Avoid creating a duplicate subscriber
    }
    try {
        LOG4CXX_INFO(logger, "Creating new subscription for detector ID: " << detID);
        auto subscriber = transferdata::img_subscriber::jms::JmsImgSubscriber::create(detID);
        subscriber->receiveImage(detID, callback, ack);
        _subscribers.insert({detID, std::move(subscriber)});  // Store the subscriber

    } catch (const std::exception& e) {
        std::cerr << "Error in receiveImage (callback mode): " << e.what() << std::endl;
        throw e;
    }
}
void ImgTransferDataImpl::receiveImage(const std::string& detID, 
                                     void (*callback)(const std::vector<unsigned char>&, u_int64_t), 
                                     const bool ack) {
    auto it = _subscribers.find(detID);
    if (it != _subscribers.end()) {
        LOG4CXX_WARN(logger, "Subscription for " << detID << " already exists. Skipping duplicate subscription.");
	throw GiapiException("You are already subscribed to this");  // Avoid creating a duplicate subscriber
    }
    try {
        LOG4CXX_INFO(logger, "Creating new subscription for detector ID: " << detID);
        auto subscriber = transferdata::img_subscriber::jms::JmsImgSubscriber::create(detID);
        subscriber->receiveImage(detID, callback, ack);
        _subscribers.insert({detID, std::move(subscriber)});  // Store the subscriber

    } catch (const std::exception& e) {
        std::cerr << "Error in receiveImage (callback mode): " << e.what() << std::endl;
	throw e;
    }
}

void ImgTransferDataImpl::receiveImage(const std::string& detID,
                                      void (*callback)(const std::vector<unsigned char>&)) noexcept(false) {
    receiveImage(detID, callback, false);
}

void ImgTransferDataImpl::receiveImage(const std::string& detID,
                                      void (*callback)(const std::vector<unsigned char>&, u_int64_t)) noexcept(false) {
    receiveImage(detID, callback, false);
}

}
