#include "JmsConsumer.h"
#include <log4cxx/logger.h>
#include <gmp/ConnectionManager.h>
#include <iostream>
#include <exception>

using namespace std;
namespace giapi {
   
namespace util {
namespace jms {

    log4cxx::LoggerPtr JmsConsumer::logger(log4cxx::Logger::getLogger("giapi.JmsConsumer"));

    JmsConsumer::JmsConsumer(const std::string & queueName) noexcept(false) : JmsConsumer(queueName, cms::Session::CLIENT_ACKNOWLEDGE) {}

    JmsConsumer::JmsConsumer(const std::string& destination, cms::Session::AcknowledgeMode acknowledgeMode) noexcept(false) : JmsConsumer(destination,acknowledgeMode,false) {}

    JmsConsumer::JmsConsumer(const std::string& destination,
                         cms::Session::AcknowledgeMode acknowledgeMode,
                         bool useDedicatedSession) noexcept(false) : _destination(destination) {
       try {
           // Get the connection manager instance
           _connectionManager = ConnectionManager::Instance();
   
           if (!useDedicatedSession) {
               // Shared connection and session from ConnectionManager
               _session = _connectionManager->createSession(acknowledgeMode);
           } else {
               LOG4CXX_DEBUG(logger, "Starting to create a connection, topic: " << _destination);
               // Create a new connection and session for this consumer
               _connection = _connectionManager->createDedicatedConnection();
               LOG4CXX_DEBUG(logger, "Connection created" );
               _connection->start();
               LOG4CXX_DEBUG(logger, "Connection started and creating a session" );
               _session.reset(_connection->createSession(acknowledgeMode));
               LOG4CXX_INFO(logger, "Session created. Consumer initialized properly" );
           }
   
           // Create the destination (topic)
           _dest = pDestination(_session->createTopic(_destination));
           
           // Create the consumer
           _consumer = pMessageConsumer(_session->createConsumer(_dest.get()));
   
           // Set this object as the message listener
           _consumer->setMessageListener(this);
   
           LOG4CXX_INFO(logger, "Created JMS consumer for destination: " << _destination
                           << (useDedicatedSession ? " [dedicated session]" : " [shared session]"));
       } catch (const cms::CMSException& e) {
           LOG4CXX_ERROR(logger, "Error creating JMS consumer: " << e.what());
           cleanup();
           throw CommunicationException("Error creating JMS consumer: " + std::string(e.what()));
       }
       catch (const std::exception& e) {
           // Catch standard exceptions (runtime_error, system_error, etc.)
           LOG4CXX_ERROR(logger, "Standard exception creating JMS consumer: " << e.what());
           cleanup();
           throw CommunicationException("Error creating JMS consumer: " + std::string(e.what()));
       }
       catch (...) {
           // Last-resort handler for non-standard exceptions
           LOG4CXX_ERROR(logger, "Unknown non-standard exception during JmsConsumer constructor, detector: "<< _destination);
           cleanup();
           throw CommunicationException("Unknown exception creating JMS consumer");
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
                _consumer.reset();
            }
            if (_session.get() != 0) {
                _session->close();
                _session.reset();
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
