#ifndef JMSMESSAGECONSUMER_H_
#define JMSMESSAGECONSUMER_H_

#include <activemq/util/Config.h>
#include <decaf/util/concurrent/CountDownLatch.h>

#include <cms/MessageListener.h>
#include <cms/Session.h>

#include <giapi/giapi.h>
#include <giapi/giapiexcept.h>
#include <giapi/SequenceCommandHandler.h>
#include <giapi/HandlerResponse.h>
#include <util/JmsSmartPointers.h>
#include <gmp/ConnectionManager.h>

#include <log4cxx/logger.h>
#include <tr1/memory>

using namespace gmp;

namespace giapi {

using namespace cms;
using namespace decaf::util::concurrent;

/**
 * forward declaration
 */
class SequenceCommandConsumer;
/**
 * Definition of a smart pointer to sequence command consumers
 */
typedef std::tr1::shared_ptr<SequenceCommandConsumer> pSequenceCommandConsumer;

/**
 * The Sequence Command Consumer is a JMS Listener in charge
 * of receiving and processing sequence commands. Whenever
 * a sequence command is delivered through the GMP, an instance
 * of this class registered for that particular sequence command
 * will react, processing the sequence command and invoking
 * the appropriate SequenceCommandHandler specified through
 * the CommandUtil::subscribeSequenceCommand() method in the GIAPI
 *
 * @see CommandUtil::subscribeSequenceCommand()
 */
class SequenceCommandConsumer : public MessageListener {

public:
	/**
	 * Static factory to construct a sequence command consumer.
	 * The returned object is a smart pointer reference to a sequence command
	 * consumer.
	 *
	 * The arguments specify what sequence command and
	 * activities this consumer will take care of, and the client
	 * sequence command handler that needs to be invoked whenever
	 * the corresponding sequence command is received.
	 *
	 * @param id the command::SequenceCommand this consumer will process
	 * @param activities the set of Activity elements (represented by
	 * the enumerated type, command::ActivitySet) that this consumer
	 * will process.
	 * @param handler The pSequenceCommandHandler, a smart pointer containing
	 * the client implementation of a SequenceCommandHandler object that will
	 * be invoked when this consumer receives the selected SequenceCommand and
	 * Activity.
	 * @throw CommunicationException
	 * If there is an issue creating the consumer, such as:
	 * - Failure to initialize the communication channel.
	 * - Invalid command ID or activity set.
	 * - Connection failure with GMP.
         */
	static pSequenceCommandConsumer create(command::SequenceCommand id,
			command::ActivitySet activities,
			pSequenceCommandHandler handler) noexcept(false);
//Fixed for exception handling in the C++20 version.
	/**
	 * Static factory to construct a sequence command consumer for
	 * a given prefix (associated to an APPLY sequence command)
	 * The returned object is a smart pointer reference to a sequence command
	 * consumer.
	 *
	 * The arguments specify what prefix and
	 * activities this consumer will take care of, and the client
	 * sequence command handler that needs to be invoked whenever
	 * the corresponding prefix is received.
	 *
	 * @param prefix Configuration prefix this consumer will process
	 * @param activities the set of Activity elements (represented by
	 * the enumerated type, command::ActivitySet) that this consumer
	 * will process.
	 * @param handler The pSequenceCommandHandler, a smart pointer containing
	 * the client implementation of a SequenceCommandHandler object that will
	 * be invoked when this consumer receives the selected SequenceCommand and
	 * Activity.
	 *
	 * @throw CommunicationException
	 * If there is an issue creating the consumer, such as:
	 * - Failure to initialize the communication channel.
	 * - Invalid prefix or activity set.
	 * - Connection failure with GMP.
         */
	static pSequenceCommandConsumer create(const std::string & prefix,
			command::ActivitySet activities,
			pSequenceCommandHandler handler) noexcept(false);
//Fixed for exception handling in the C++20 version.
	/**
	 * Destructor. Cleans up all the resources instantiated by this consumer
	 * @note This function does not throw exceptions. However, if an exception oc>
         * the destructor **must not** propagate it, as destructors should be noexcep>
	 */
	virtual ~SequenceCommandConsumer() noexcept;
//Fixed for exception handling in the C++20 version.
	/**
	 * Invoked by the JMS whenever a new message is received
	 * @throw CommunicationException If an issue occurs while processing the message, such as:
	 * - Parsing failure in the received message.
	 * - Handler execution failure.
	 * - Issues with sending the response message.
         */
	virtual void onMessage(const Message* message) noexcept(false);
//Fixed for exception handling in the C++20 version.
private:
	/**
	 * Constructor. The arguments specify what sequence command and
	 * activities this consumer will take care of, and the client
	 * sequence command handler that needs to be invoked whenever
	 * the corresponding sequence command is received.
	 *
	 * @param id the command::SequenceCommand this consumer will process
	 * @param activities the set of Activity elements (represented by
	 * the enumerated type, command::ActivitySet) that this consumer
	 * will process.
	 * @param handler The pSequenceCommandHandler, a smart pointer containing
	 * the client implementation of a SequenceCommandHandler object that will
	 * be invoked when this consumer receives the selected SequenceCommand and
	 * Activity.
	 *
         * @throw CommunicationException
	 * If an issue occurs during initialization, such as:
	 * - Failure to create the session.
	 * - Invalid handler or activity set.
	 * - Problems establishing the consumer.
         */
	SequenceCommandConsumer(command::SequenceCommand id,
			command::ActivitySet activities,
			pSequenceCommandHandler handler) noexcept(false);
//Fixed for exception handling in the C++20 version.
	 /**
	 * Special constructor for the Apply Sequence Command Consumer.
	 * The arguments specify the prefix and the
	 * activities this consumer will take care of, and the client
	 * sequence command handler that needs to be invoked whenever
	 * the corresponding prefix is received.
	 *
	 * @param prefix The configuration prefix this consumer will process
	 * @param activities the set of Activity elements (represented by
	 * the enumerated type, command::ActivitySet) that this consumer
	 * will process.
	 * @param handler The pSequenceCommandHandler, a smart pointer containing
	 * the client implementation of a SequenceCommandHandler object that will
	 * be invoked when this consumer receives the selected SequenceCommand and
	 * Activity.
	 *
         * @throw CommunicationException
         * If an issue occurs during initialization, such as:
	 * - Failure to create the session.
	 * - Invalid handler or activity set.
	 * - Problems establishing the consumer.
         */
	SequenceCommandConsumer(const std::string & prefix,
			command::ActivitySet activities,
			pSequenceCommandHandler handler) noexcept(false);
//Fixed for exception handling in the C++20 version.
	/**
	 * Takes care of the initialization of a SequenceCommandConsumer
	 * using the given topic and set of activities. Associates the consumer
	 * with the given handler.
	 *
	 * This method is used by the constructor of this class.
	 * @throw CommunicationException
	 * If an error occurs during initialization, such as:
	 * - Connection failure to GMP.
	 * - Session or destination creation failure.
	 * - Consumer setup issues.
	 */
	void init(const std::string & topic, command::ActivitySet activities,
			pSequenceCommandHandler handler) noexcept(false);
//Fixed for exception handling in the C++20 version.
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

	/**
	 * The JMS Session associated to this consumer. Each consumer has its
	 * own session, since they need to operate in different execution
	 * threads
	 */
	pSession _session;

	/**
	 * The virtual channel from where this consumer will get the messages.
	 * The destination is obtained based on the sequence command we have
	 * interest on
	 */
	pDestination _destination;

	/**
	 * The consumer instance for this object. Each consumer runs on its
	 * own JMS Session
	 */
	pMessageConsumer _consumer;

	/**
	 * The handler to be invoked when a sequence command is received
	 */
	pSequenceCommandHandler _handler;

	/**
	 * The sequence command that is handled by this consumer
	 */
	command::SequenceCommand _sequenceCommand;
	
	/**
	 * The connection manager
	 */
	pConnectionManager _connectionManager;

	/**
	 * Close and destroy associated JMS resources used by this consumer
	 */
	void cleanup();

	/**
	 * Return an appropriate selector to be used by the message
	 * consumer based on the activity set
	 *
	 * @param activities The activity set that the returned selector will
	 *                   match
	 *
	 * @return a string representing the selector that matches the activity
	 *         set
	 */
	std::string buildSelector(command::ActivitySet activities);

};

}

#endif /*MESSAGECONSUMER_H_*/
