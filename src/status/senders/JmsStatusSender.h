#ifndef JMSSTATUSSENDER_H_
#define JMSSTATUSSENDER_H_

#include <log4cxx/logger.h>

#include <status/senders/AbstractStatusSender.h>
#include <giapi/giapiexcept.h>
#include <status/StatusItem.h>
#include <status/AlarmStatusItem.h>
#include <status/HealthStatusItem.h>

#include <cms/Session.h>
#include <cms/Destination.h>
#include <cms/MessageProducer.h>

#include <util/JmsSmartPointers.h>
#include <gmp/ConnectionManager.h>

#include <stdexcept>
//Required for exception handling

using namespace gmp;

namespace giapi {
/**
 * A Status Sender that uses JMS as the underlying communication
 * mechanism
 */
class JmsStatusSender: public AbstractStatusSender {
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;
	enum Offsets {
		BASIC_OFFSET = 0,
		ALARM_OFFSET = 10,
		HEALTH_OFFSET = 20
	};


public:
	/**
	 * @throw CommunicationException
	 * If the sender fails to establish a connection
	 * or initialize the message producer.
	 */
	JmsStatusSender() noexcept(false);
	virtual ~JmsStatusSender();
protected:
	/**
	 * @throw PostException
	 * If an error occurs while sending the message.
	 */
	virtual int postStatus(pStatusItem item) const noexcept(false);

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
	 * The message producer in charge of sending requests down to
	 * the GMP. Runs on its own session
	 */
	pMessageProducer _producer;
	
	/**
	 * The connection manager associated to this sender
	 */
	pConnectionManager _connectionManager;

	/**
	 * Close open resources and destroy connections
	 */
	void cleanup();

};

}

#endif /*JMSSTATUSSENDER_H_*/
