#ifndef COMPLETIONINFOPRODUCER_H_
#define COMPLETIONINFOPRODUCER_H_

#include <cstdarg>

#include <giapi/giapi.h>
#include <giapi/HandlerResponse.h>
#include <giapi/giapiexcept.h>
#include <util/JmsSmartPointers.h>

#include <cms/Session.h>
#include <cms/Destination.h>
#include <cms/MessageProducer.h>

#include <log4cxx/logger.h>
#include <tr1/memory>

#include <gmp/ConnectionManager.h>

#include <stdexcept>

using namespace giapi;
using namespace cms;

namespace gmp {

class CompletionInfoProducer;

typedef std::auto_ptr<CompletionInfoProducer> pCompletionInfoProducer;

/**
 * Produces completion information messages back to the GMP.
 */

class CompletionInfoProducer {

public:
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

	virtual ~CompletionInfoProducer();

	/**
	 * Send the completion information contained in the response back to the
	 * GMP.
	 *
	 * @param id the original ActionId associated to the actions for which
	 * completion info is being reported
	 * @param response contains the completion state associated to the action
	 * id.
	 * @return giapi::status::OK if the post succeeds. Otherwise, it
	 *         returns giapi::status::ERROR.
	 * @throw PostException
	 * If there is a problem posting the completion information to the GMP.
	 */
	int postCompletionInfo(command::ActionId id,
			pHandlerResponse response) noexcept(false);


	/**
	 * Static factory to instantiate producers referenced via smart pointers
	 * @throw CommunicationException
	 * If the producer cannot be created due to connection issues.
	 */
	static pCompletionInfoProducer create() noexcept(false);

private:

	/**
	 * The JMS Session associated to this producer.
	 */
	pSession _session;

	/**
	 * The virtual channel to where this producer will send messages to
	 */
	pDestination _destination;

	/**
	 * The message producer in charge of sending completion information
	 * messages. Runs on its own session
	 */
	pMessageProducer _producer;
	
	/**
	 * The connection Manager
	 */
	pConnectionManager _connectionManager;

	/**
	 * Destroy any allocated resources and closes communication channels
	 */
	void cleanup();

	/**
	 * Private Constructor
	 * @throw CommunicationException
	 * If there is an issue initializing the producer.
	 */
	CompletionInfoProducer() noexcept(false);

};

}

#endif /*COMPLETIONINFOPRODUCER_H_*/
