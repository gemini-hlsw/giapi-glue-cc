#ifndef DATAUTILIMPL_H_
#define DATAUTILIMPL_H_

#include <tr1/memory>
#include <log4cxx/logger.h>
#include <giapi/giapi.h>

#include <data/JmsObsEventProducer.h>
#include <data/JmsFileEventsProducer.h>

#include <stdexcept>
//Required for exception handling

namespace giapi {

class DataUtilImpl;
typedef std::tr1::shared_ptr<DataUtilImpl> pDataUtilImpl;

class DataUtilImpl {
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;
public:
	/**
	 * @throws CommunicationException If there is an error initializing
	 *         the internal producers for observation and file events.
	 */
	static pDataUtilImpl Instance() noexcept(false);

	/**
	 * @throws CommunicationException If there is an error sending the event
	 *         to the GMP via JMS.
	 */
	int postObservationEvent(data::ObservationEvent event, const std::string & datalabel) noexcept(false);

	/**
	 * @throws CommunicationException If there is an error sending the event
	 *         to the GMP via JMS.
	 */
	int postAncillaryFileEvent(const std::string & filename, const std::string & datalabel) noexcept(false);

	/**
	 * @throws CommunicationException If there is an error sending the event
	 *         to the GMP via JMS.
	 */
	int postIntermediateFileEvent(const std::string & filename,
					const std::string & datalabel, const std::string & hint) noexcept(false);

	virtual ~DataUtilImpl();

private:
	static pDataUtilImpl INSTANCE;

	pJmsObsEventProducer pObsEventProducer;

	pJmsFileEventsProducer pFileEventsProducer;

	/**
	 * @throws CommunicationException If there is an error initializing
	 *         the observation event or file event producers.
	 */
	DataUtilImpl() noexcept(false);
};

}

#endif /*DATAUTILIMPL_H_*/
