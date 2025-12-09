/**
 * @class JmsConsumer
 *
 * @brief A JMS (Java Message Service) consumer class for receiving asynchronous messages from a topic.
 *
 * The JmsConsumer encapsulates the logic for creating a JMS topic consumer using the ActiveMQ CMS API.
 * It supports both **shared** and **dedicated** connection/session modes, allowing flexible resource
 * usage depending on throughput and isolation needs.
 *
* ## Key Features:
 * - Subscribes to a JMS topic using a CMS consumer.
 * - Can reuse a shared CMS connection/session or create a fully **dedicated connection and session**.
 * - Registers itself as the message listener for asynchronous message reception.
 * - Handles exceptions during setup and ensures proper resource cleanup.
 *
 * ## Session/Connection Handling:
 * - **Shared Session**: Obtained from `ConnectionManager::createSession()` and uses a common JMS connection.
 *   Useful for lightweight consumers or when resource sharing is acceptable.
 * - **Dedicated Session**: Creates an independent connection via `createDedicatedConnection()`, starts it,
 *   and builds a private session for complete consumer isolation. Suitable for high-throughput or critical
 *   message pipelines.
 *
 */

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
         * @param destination  The name of the JMS topic to subscribe to.
         * @throw CommunicationException if there is a problem establishing
         *        the connection with the JMS provider
         */
        JmsConsumer(const std::string& destination) noexcept(false);

        /**
         * Constructor. Creates a consumer for the specified destination.
         * 
         * @param destination  The name of the JMS topic to subscribe to.
         * @param acknowledgeMode (cms::Session::AcknowledgeMode): CMS acknowledgment mode (AUTO_ACKNOWLEDGE)
         * @throw CommunicationException if there is a problem establishing
         *        the connection with the JMS provider
         */
        JmsConsumer(const std::string& queueName, cms::Session::AcknowledgeMode acknowledgeMode) noexcept(false);
        
	    /**
         * Constructor. Creates a consumer for the specified destination.
         * 
         * @param destination (string) The name of the JMS topic to subscribe to.
         * @param acknowledgeMode (cms::Session::AcknowledgeMode): CMS acknowledgment mode (AUTO_ACKNOWLEDGE)
         * @param useDedicatedSession (bool): 
         *     - If `false` (default behavior), uses the shared connection and session managed by ConnectionManager.
         *     - If `true`, creates a new CMS connection and session **dedicated** to this consumer.
         * @throw CommunicationException if there is a problem establishing
         *        the connection with the JMS provider
         */        
        
        JmsConsumer(const std::string& queueName, cms::Session::AcknowledgeMode acknowledgeMode, bool useDedicatedSession) noexcept(false);

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
         * CMS connection pointer. 
         */

	    pConnection _connection;

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
