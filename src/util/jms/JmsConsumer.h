#ifndef JMSCONSUMER_H_
#define JMSCONSUMER_H_

#include <string>
#include <tr1/memory>
#include <decaf/util/concurrent/CountDownLatch.h>  
#include <activemq/util/Config.h>
#include <cms/MessageListener.h>
#include <cms/Session.h>
#include <cms/Destination.h>
#include <cms/MessageConsumer.h>
#include <gmp/ConnectionManager.h>
#include <giapi/giapiexcept.h>
#include <util/JmsSmartPointers.h>
#include <log4cxx/logger.h>

using namespace gmp;

namespace giapi {
        
using namespace cms;
using namespace decaf::util::concurrent;

namespace util {
namespace jms {

    // Forward declaration
    class JmsConsumer;

    /**
     * Definition of a smart pointer to JMS consumers
     */
    typedef std::tr1::shared_ptr<JmsConsumer> pJmsConsumer;

    /**
     * Base class for JMS consumers. This class provides the basic functionality
     * to receive messages from a JMS destination.
     */
    class JmsConsumer : public MessageListener {
    public:
        /**
         * Destructor. Cleans up JMS resources.
         */
        virtual ~JmsConsumer() noexcept;

        /**
         * Logging facility
         */
        static log4cxx::LoggerPtr logger;

    protected:
        /**
         * Constructor. Creates a consumer for the specified destination.
         * 
         * @param destination The JMS destination to consume messages from
         * @throw CommunicationException if there is a problem establishing
         *        the connection with the JMS provider
         */
        JmsConsumer(const std::string& destination) noexcept(false);

        JmsConsumer(const std::string& queueName, cms::Session::AcknowledgeMode acknowledgeMode) noexcept(false);

        /**
         * Starts consuming messages from the destination.
         * 
         * @throw CommunicationException if there is a problem starting
         *        the consumer
         */
        virtual void startConsumer() noexcept(false);

        /**
         * Stops consuming messages from the destination.
         */
        virtual void stopConsumer();

        /**
         * Callback method that will be called when a message is received.
         * Subclasses must implement this method to process the received messages.
         * 
         * @param message The received JMS message
         */
        virtual void onMessage(const Message* message) = 0;

        /**
         * The JMS Session associated to this consumer.
         */
        pSession _session;

        /**
         * The message consumer in charge of receiving messages.
         * Runs on its own session.
         */
        pMessageConsumer _consumer;

        /**
         * The connection manager
         */
        pConnectionManager _connectionManager;

        

    private:
        /**
         * The destination name where this consumer will receive messages from
         */
        std::string _destination;

        /**
         * The virtual channel from where this consumer will receive messages
         */
        pDestination _dest;

        /**
         * Close open resources and destroy connections
         */
        void cleanup();

        // Prevent copy construction and assignment
        JmsConsumer(const JmsConsumer&);
        JmsConsumer& operator=(const JmsConsumer&);
    };

    }
    }
}

#endif /* JMSCONSUMER_H_ */ 