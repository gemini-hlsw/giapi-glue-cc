#ifndef JMSEPICSCONFIGURATION_H_
#define JMSEPICSCONFIGURATION_H_

#include <giapi/giapiexcept.h>
#include <util/giapiMaps.h>
#include <util/JmsSmartPointers.h>
#include <gemini/epics/EpicsConfiguration.h>

#include <log4cxx/logger.h>

#include <set>
#include <string>

#include <stdexcept>

namespace giapi {


/**
 * A JMS based Epics Configuration
 */

class JmsEpicsConfiguration: public EpicsConfiguration {
public:

	/*
	 * Factory constructor, using smart pointers
	 */
	static pEpicsConfiguration create(pSession session);

	/**
	 * Initialize the configuration. This requires access to the
	 * GMP to get the values.
	 * @throw CommunicationException
	 * If there is an issue establishing communication with the GMP.
	 *
	 * @throw TimeoutException
	 * If the request to fetch the EPICS channels exceeds the
	 * allowed timeout.
	 *
	 */
	void init() noexcept(false);


	/**
	 * Return true if this epics configuration was correctly initialized
	 * and contains information from the valid epics channels from the GMP
	 */
	bool isInitialized() const;


	/**
	 * Return true if the given channel name is an available channel
	 * in the configuration
	 */
	bool hasChannel(const std::string &channel);

	/**
	 * Constructor. Takes as an argument an already initialized session.
	 */
	JmsEpicsConfiguration(pSession session);

	virtual ~JmsEpicsConfiguration();

private:

	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

	/**
	 * Internal storage of valid epics channels
	 */
	std::set<std::string> _epicsChannels;

	/**
	 * Request the valid epics channels to the GMP, and stores
	 * them internally for later reference
	 * @throw CommunicationException
	 * If there is an error in the communication with the GMP.
	 *
	 * @throw TimeoutException
	 * If the request to retrieve the EPICS channels times out.
	 *
	 */
	void requestChannels(long timeout) noexcept(false);

	/**
	 * The JMS Session associated to this producer.
	 */
	pSession _session;

};

}

#endif /*JMSEPICSCONFIGURATION_H_*/

