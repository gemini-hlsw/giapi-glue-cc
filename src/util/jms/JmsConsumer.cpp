#include "JmsConsumer.h"
#include <log4cxx/logger.h>
#include <gmp/ConnectionManager.h>

namespace giapi {
   
namespace util {
namespace jms {

    log4cxx::LoggerPtr JmsConsumer::logger(log4cxx::Logger::getLogger("giapi.JmsConsumer"));

    JmsConsumer::JmsConsumer(const std::string & queueName) noexcept(false) : JmsConsumer(queueName, cms::Session::CLIENT_ACKNOWLEDGE) {}

    JmsConsumer::JmsConsumer(const std::string& destination, cms::Session::AcknowledgeMode acknowledgeMode) noexcept(false)
        : _destination(destination) {
        try {
            // Get the connection manager instance
            _connectionManager = ConnectionManager::Instance();
            
            // Create an auto-acknowledged session
            _session = _connectionManager->createSession(acknowledgeMode);
            
            // Create the destination (topic)
            _dest = pDestination(_session->createTopic(_destination));
            
            // Create the consumer
            _consumer = pMessageConsumer(_session->createConsumer(_dest.get()));
            
            // Set this object as the message listener
            _consumer->setMessageListener(this);
            
            LOG4CXX_INFO(logger, "Created JMS consumer for destination: " << _destination);
        } catch (const cms::CMSException& e) {
            LOG4CXX_ERROR(logger, "Error creating JMS consumer: " << e.what());
            cleanup();
            throw CommunicationException("Error creating JMS consumer: " + std::string(e.what()));
        }
    }

    void JmsConsumer::startConsumer() noexcept(false) {
        try {
            if (_consumer) {
                _consumer->start();
                LOG4CXX_INFO(logger, "Started JMS consumer for destination: " << _destination);
            }
        } catch (const cms::CMSException& e) {
            LOG4CXX_ERROR(logger, "Error starting JMS consumer: " << e.what());
            throw CommunicationException("Error starting JMS consumer: " + std::string(e.what()));
        }
    }

    void JmsConsumer::stopConsumer() {
        try {
            if (_consumer) {
                _consumer->stop();
                LOG4CXX_INFO(logger, "Stopped JMS consumer for destination: " << _destination);
            }
        } catch (const cms::CMSException& e) {
            LOG4CXX_ERROR(logger, "Error stopping JMS consumer: " << e.what());
        }
    }

    void JmsConsumer::cleanup() {
        try {
            if (_consumer.get() != 0) {
                _consumer->close();
            }
            if (_session.get() != 0) {
                _session->close();
            }
        } catch (const cms::CMSException& e) {
            LOG4CXX_ERROR(logger, "Error cleaning up JMS resources: " << e.what());
        }
    }

    JmsConsumer::~JmsConsumer() noexcept {
        LOG4CXX_DEBUG(logger, "Destroying JMS consumer for destination: " << _destination);
        cleanup();
    }

}
}
} 