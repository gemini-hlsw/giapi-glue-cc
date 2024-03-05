/*
 * JmsApplyOffset.cpp
 *
 *  Created on: Aug 17, 2023
 *      Author: framos
 */

#include "JmsApplyOffset.h"
#include <gmp/GMPKeys.h>
#include <src/util/PropertiesUtil.h>
#include <src/util/StringUtil.h>
#include <stdlib.h>
#include <thread>
#include<unistd.h>

using namespace gmp;

namespace giapi {

namespace gemini {

namespace tcs {

namespace jms {

JmsApplyOffset::JmsApplyOffset() throw (CommunicationException) :
                                 JmsProducer(GMPKeys::GMP_TCS_OFFSET_DESTINATION) {
	instName = giapi::util::PropertiesUtil::Instance().getProperty("gmp.instrument");
	if(giapi::util::StringUtil::isEmpty(instName)) {
		LOG4CXX_WARN(logger, "Not instrument set in the gmp.properties file. The dummyInst name is used by default" << instName);
		instName = "dummyInst";
	}
}

JmsApplyOffset::~JmsApplyOffset() {
}


void JmsApplyOffset::callback(void (*callbackOffset)(int, std::string),
		                      MessageConsumer *tmpConsumer, TemporaryQueue * tmpQueue, int timeout) {

	//and wait for the response, timing out if necessary.
	Message *reply = (timeout > 0) ? tmpConsumer->receive(timeout)
					: tmpConsumer->receive();

	// Free the resources
	tmpConsumer->close();
	delete tmpConsumer;
	tmpQueue->destroy();
	delete tmpQueue;

	int status = status::ERROR;
	std::string msg = "";
	if (reply != NULL) {
		const TextMessage* textMessage = dynamic_cast<const TextMessage*> (reply);
		if (textMessage == NULL)
		msg = "Error. Empty message was received ";
		std::string text = textMessage->getText();
		std::string::size_type posResult = text.find('|');
		if (posResult != std::string::npos) {
		   status = std::stoi(text.substr(0, posResult));
		   msg = text.substr(posResult+1, text.length());
		}

	} else { //timeout .Throw an exception
		msg = "Time out while waiting for TCS Offset executiong";
	}
	callbackOffset(status, msg);

}


pTcsOffset JmsApplyOffset::create() throw (CommunicationException) {
	pTcsOffset tcsOffset(new JmsApplyOffset());
	return tcsOffset;
}

int JmsApplyOffset::sendOffset(const double p, const double q,
		           const OffsetType offsetType, const long timeout,
		           void (*callbackOffset)(int, std::string)) throw (CommunicationException, TimeoutException) {

	BytesMessage * rMsg = NULL;
	int wasOffsetApplied = 0;
	try {
		//Create a message to do the request.
		rMsg = _session->createBytesMessage();
		//create temporary objects to get the answer
		TemporaryQueue * tmpQueue = _session->createTemporaryQueue();
		MessageConsumer * tmpConsumer = _session->createConsumer(tmpQueue);

		//define the destination for the service to provide an answer
		rMsg->setCMSReplyTo(tmpQueue);
		//send the request
		rMsg->writeDouble(p);
		rMsg->writeDouble(q);
		rMsg->writeInt(offsetType);
		rMsg->setStringProperty("instName", instName);
		_producer->send(rMsg);
		if (callbackOffset == NULL) {
			//delete the request, not needed anymore
			delete rMsg;

			//and wait for the response, timing out if necessary.
			Message *reply = (timeout > 0) ? tmpConsumer->receive(timeout)
							: tmpConsumer->receive();

			tmpConsumer->close();
			delete tmpConsumer;

			tmpQueue->destroy();
			delete tmpQueue;

			if (reply != NULL) {
				const TextMessage* textMessage = dynamic_cast<const TextMessage*> (reply);
				if (textMessage == NULL)
					return status::ERROR;

				//it gets if the offset was applied
			    	std::string text = textMessage->getText();

				std::string::size_type posResult = text.find('|');
				if (posResult != std::string::npos)
				   wasOffsetApplied = std::stoi(text.substr(0, posResult));
				if (wasOffsetApplied == 0)
				   std::cerr << "The offset was not applied due to " << text.substr(posResult+1, text.length()) <<std::endl;

			} else { //timeout .Throw an exception
					throw TimeoutException("Time out while waiting for TCS Offset executiong");
			}

		} else {
			wasOffsetApplied = 1;
			std::thread t1 (callback, callbackOffset, tmpConsumer, tmpQueue, timeout);
			t1.detach();
		}
	} catch (CMSException &e) {
		if (rMsg != NULL) {
			delete rMsg;
		}
		throw CommunicationException("Problem applying the Offset in the TCS.  " + e.getMessage());
	}
	return wasOffsetApplied;

}

int JmsApplyOffset::sendOffset(const double p, const double q,
				   const OffsetType offsetType, const long timeout) throw (CommunicationException, TimeoutException) {

	return sendOffset(p, q, offsetType, timeout, NULL);
}


} // jms    namespace
} // tcs    namespace
} // gemini namespace
} // giapi  namespace
