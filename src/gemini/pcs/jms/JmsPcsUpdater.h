
#ifndef JMSPCSUPDATER_H_
#define JMSPCSUPDATER_H_

#include <giapi/giapiexcept.h>
#include <gemini/pcs/PcsUpdater.h>
#include <util/jms/JmsProducer.h>

#include <stdexcept>
//Required for exception handling

namespace giapi {
namespace gemini {
namespace pcs {
namespace jms {


/**
 * An implementation of the PcsUpdater interface using JMS
 * as the connection mechanism.
 */
class JmsPcsUpdater: public PcsUpdater,  util::jms::JmsProducer {
public:
	virtual ~JmsPcsUpdater();

	/**
 	* @throw GiapiException
	* If the update fails due to internal errors.
 	*/
	int postPcsUpdate(double zernikes[], int size) noexcept(false);

	/**
	 * Static factory method to instantiate a new JmsPcsUpdater object
	 * and obtain a smart pointer to access it.
	 * @throw CommunicationException
	 * If the creation of the updater fails due
	 *        to an issue with the GMP communication setup.
	 */
	static pPcsUpdater create() noexcept(false);
private:
	/**
	 * Private Constructor.
	 * @throw CommunicationException
	 * If the initialization fails due to
	 *        a communication error with the GMP.
	 */
	JmsPcsUpdater() noexcept(false);
};

}
}
}
}

#endif /* JMSPCSUPDATER_H_ */
