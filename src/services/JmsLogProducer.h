/**
 * A JMS Producer that sends logging information to the GMP
 */

#ifndef JMSLOGPRODUCER_H_
#define JMSLOGPRODUCER_H_

#include <cstdarg>
#include <giapi/giapi.h>
#include <giapi/giapiexcept.h>
#include <util/jms/JmsProducer.h>

#include <stdexcept>


namespace giapi {
/**
 * Forward declaration of JMS Class
 */
class JmsLogProducer;

/**
 * Smart pointer definition for this class
 */
typedef std::auto_ptr<JmsLogProducer> pJmsLogProducer;

/**
 * A JMS Producer in charge of sending Logs to the GMP
 */
class JmsLogProducer :  public util::jms::JmsProducer  {
public:
	/**
	 * Factory method to create a new JmsLogProducer.
	 * This factory returns a smart pointer object to
	 * simplify the management.
	 * @throw CommunicationException
	 * If the producer cannot be created due to
         * connection issues or session initialization failure.
	 */
	static pJmsLogProducer create() noexcept(false);

	/**
	 * Sends the system logging information to the GMP.
	 * @throw CommunicationException
	 * If there is an issue sending the log
	 * message, such as connection failure or session errors.
	 */
	void postLog(log::Level level, const std::string &msg)
		noexcept(false);

	virtual ~JmsLogProducer();
private:
	/**
	 * Private constructor.
	 * @throw CommunicationException
	 * If the producer fails to initialize due to
	 * connection issues or session creation failure.
	 */
	JmsLogProducer() noexcept(false);
};

}

#endif /* JMSLOGPRODUCER_H_ */
