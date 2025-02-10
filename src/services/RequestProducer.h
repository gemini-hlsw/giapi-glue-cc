/*
 * RequestProducer.h
 *
 *  Created on: Dec 10, 2008
 *      Author: anunez
 */

#ifndef REQUESTPRODUCER_H_
#define REQUESTPRODUCER_H_

#include <cstdarg>

#include <giapi/giapiexcept.h>

#include <util/JmsSmartPointers.h>


#include <cms/Session.h>
#include <cms/Destination.h>
#include <cms/MessageProducer.h>
#include <gmp/ConnectionManager.h>

#include <log4cxx/logger.h>

#include <stdexcept>
//Required for exception handling

using namespace cms;
using namespace gmp;

namespace giapi {

class RequestProducer;

typedef std::auto_ptr<RequestProducer> pRequestProducer;

/**
 * This class is in charge of producing JMS Messages to
 * send service requests to the GMP
 * @throw CommunicationException
 * If there is an issue establishing
 * a connection with the GMP, creating a session, or initializing
 * the message producer.
 *
 * @throw TimeoutException
 * If a request sent to the GMP does not
 * receive a response within the specified timeout period.
 *
 */
class RequestProducer {
public:

	virtual ~RequestProducer();

	/**
	 * @throw CommunicationException If the producer fails to initialize
	 * due to connection issues or session creation failure.
	 */
	static pRequestProducer create() noexcept(false);


	/**
	 * @throw TimeoutException If the request times out before receiving a response.
	 *
	 * @throw PostException If there is an issue sending the utility request.
	 */
	std::string getProperty(const std::string &key, long timeout = 0)
		noexcept(false);

private:

	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

	/**
	 * The JMS Session associated to this producer.
	 */
	pSession _session;

	/**
	 * The virtual channel to where this producer will send messages to
	 */
	pDestination _destination;

	/**
	 * The message producer in charge of sending requests down to
	 * the GMP. Runs on its own session
	 */
	pMessageProducer _producer;

	
	/**
	 * The connection manager
	 */
	pConnectionManager _connectionManager;

	/**
	 * Close open resources and destroy connections
	 */
	void cleanup();

	/**
	 * Constructor
	 * @throw CommunicationException
	 * If the producer fails to
	 * initialize due to connection issues or session creation failure.
	 */
	RequestProducer() noexcept(false);

};



}

#endif /* REQUESTPRODUCER_H_ */
