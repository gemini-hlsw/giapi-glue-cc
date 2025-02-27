#ifndef SERVICESUTILIMPL_H_
#define SERVICESUTILIMPL_H_

#include <cstdarg>

#include <tr1/memory>
#include <log4cxx/logger.h>

#include <giapi/giapi.h>
#include <giapi/giapiexcept.h>

#include <services/RequestProducer.h>
#include <services/JmsLogProducer.h>

#include <stdexcept>
//Required for exception handling

namespace giapi {

class ServicesUtilImpl;

typedef std::tr1::shared_ptr<ServicesUtilImpl> pServicesUtilImpl;

class ServicesUtilImpl {
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

public:

	/**
	 * Get the singleton instance of the ServiceUtil implementation
	 * @throw CommunicationException
	 * If an error occurs while initializing
	 * the instance, such as failure in connecting to GMP services.
	 */
	static pServicesUtilImpl Instance() noexcept(false);

	/**
	 * Sends the logging information to the GMP
	 * @throw CommunicationException
	 * If there is an issue sending the log
	 * message, such as connection failure or session errors.
	 */
	void systemLog(log::Level level, const std::string &msg)
		noexcept(false);

	long64 getObservatoryTime();

	/**
	 * Returns the property value for the given key. If there
	 * are no value associated to that key, an empty string
	 * is returned.
	 * @throw CommunicationException
	 * If an error occurs while sending the request
	 * or receiving the response.
	 * @throw TimeoutException
	 * If the request times out before receiving a response.
	 */
	const std::string getProperty(const std::string &key, long timeout)
			noexcept(false);

	/**
	 * Destructor
	 */
	virtual ~ServicesUtilImpl();

private:
	static pServicesUtilImpl INSTANCE;

	/**
	 * Private constructor
	 * @throw CommunicationException
	 * If an error occurs during initialization,
	 * such as failure in establishing connections.
	 */
	ServicesUtilImpl() noexcept(false);

	/**
	 * Smart pointer to the RequestProducer object
	 */
	pRequestProducer _producer;

	/**
	 * Smart pointer to the Log Producer object
	 */
	pJmsLogProducer _logProducer;


};

}

#endif /*SERVICESUTILIMPL_H_*/
