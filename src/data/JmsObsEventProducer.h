#ifndef JMSOBSEVENTPRODUCER_H_
#define JMSOBSEVENTPRODUCER_H_

#include <giapi/giapi.h>

#include <util/jms/JmsProducer.h>

#include <stdexcept>
//Required for exception handling

namespace giapi {

class JmsObsEventProducer;

typedef std::auto_ptr<JmsObsEventProducer> pJmsObsEventProducer;

/**
 * This class is in charge of sending JMS messages to the GMP
 * containing the GIAPI Observation Events from the instrument
 */
class JmsObsEventProducer: public util::jms::JmsProducer {
public:

	/**
	 * Factory method to create a producer of Observation Events.
	 * @return a smart pointer to the newly created observation
	 *         event producer
	 * @throws CommunicationException If there is an error while initializing
	 *        the connection to the GMP for observation event messaging.
	 */
	static pJmsObsEventProducer create() noexcept(false);

	/**
	 * Destructor
	 */
	virtual ~JmsObsEventProducer();

	/**
	 * Sends the observation event to the GMP via JMS
	 * @param event the Observation event to send
	 * @param dataLabel the fits filename associated to this observation event
	 * @return  status::ERROR if the event is invalid or the dataLabel is empty.
	 *          Otherwise it returns status::OK
	 * @throws CommunicationException If there is a failure in sending
	 *        the observation event to the GMP.
	 *
	 */
	int postEvent(data::ObservationEvent event, const std::string &dataLabel)
			noexcept(false);

private:
	/**
	 * @throws CommunicationException If the producer cannot be initialized
	 *        properly due to connection failures or GMP communication issues.
	 *
	 */
	JmsObsEventProducer() noexcept(false);
};

}

#endif /* JMSOBSEVENTPRODUCER_H_ */
