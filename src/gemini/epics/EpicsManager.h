
#ifndef EPICSCHANNELSMANAGER_H_
#define EPICSCHANNELSMANAGER_H_

#include <tr1/memory>
#include <giapi/giapiexcept.h>
#include <giapi/EpicsStatusHandler.h>

#include <stdexcept>
//Required for exception handling

namespace giapi {

/**
 * This is an interface for implementations that will
 * provide support to handle subscriptions to epics status
 * items
 */
class EpicsManager {

public:

	/**
 	* @throw GiapiException
	* Thrown if there is an error while subscribing to the EPICS channel,
        * such as an invalid channel name, communication failure, or if the
        * handler is not valid.
 	*/
	virtual int subscribeEpicsStatus(const std::string &name,
			pEpicsStatusHandler handler) noexcept(false)=0;

	/**
 	* @throw GiapiException
 	* Thrown if there is an error while unsubscribing from the EPICS channel,
	* such as if the channel is not found or there is no active subscription.
 	*/
	virtual int unsubscribeEpicsStatus(const std::string &name)
			noexcept(false)=0;

	EpicsManager() {}
	virtual ~EpicsManager() {}
};

typedef std::auto_ptr<EpicsManager> pEpicsManager;

}


#endif /* EPICSCHANNELSMANAGER_H_ */
