/*
 * JmsApplyOffset.h
 *
 *  Created on: Aug 17, 2023
 *      Author: framos
 */

#ifndef JMSAPPLYOFFSET_H_
#define JMSAPPLYOFFSET_H_

#include <giapi/giapiexcept.h>
#include <util/jms/JmsProducer.h>
#include "../ApplyOffset.h"
#include <gemini/tcs/ApplyOffset.h>

namespace giapi {

namespace gemini {

namespace tcs {

namespace jms {

class JmsApplyOffset: public ApplyOffset, util::jms::JmsProducer {
public:

	static pTcsOffset create() throw (CommunicationException);

	int sendOffset(const double p, const double q,
				   const OffsetType offsetType, const long timeout) throw (CommunicationException, TimeoutException);

	int sendOffset(const double p, const double q,
		           const OffsetType offsetType, const long timeout,
		           void (*callbackOffset)(int, std::string)) throw (CommunicationException, TimeoutException);

	static void callback(void (*callbackOffset)(int, std::string), MessageConsumer *tmpConsumer, TemporaryQueue * tmpQueue, int timeout);

	virtual ~JmsApplyOffset();


private:
	JmsApplyOffset() throw (CommunicationException);
	std::string instName;

};

}
}
}
}

#endif /* SRC_GEMINI_TCS_JMS_JMSAPPLYOFFSET_H_ */

