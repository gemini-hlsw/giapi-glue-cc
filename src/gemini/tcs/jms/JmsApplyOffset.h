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

#include <stdexcept>

namespace giapi {

   namespace gemini {
   
      namespace tcs {
      
         namespace jms {
         
            class JmsApplyOffset: public ApplyOffset, util::jms::JmsProducer {
            public:

               /**
                * @throw CommunicationException
                * If there is an issue establishing the connection
                *        with the TCS system.
                */
            	static pTcsOffset create() noexcept(false);

               /**
                * @throw CommunicationException
                * If there is an issue sending the request to the TCS system.
                * @throw TimeoutException
                * If the response from the TCS system is not received within the timeout period.
                */
            	int sendOffset(const double p, const double q,
            				   const OffsetType offsetType, const long timeout) noexcept(false);
            

               /**
                * @throw CommunicationException
                * If there is an issue sending the request to the TCS system.
                * @throw TimeoutException
                * If the response from the TCS system is not received within the timeout period.
                */
            	int sendOffset(const double p, const double q,
            		           const OffsetType offsetType, const long timeout,
            		           void (*callbackOffset)(int, std::string)) noexcept(false);
            
            	static void callback(void (*callbackOffset)(int, std::string), MessageConsumer *tmpConsumer, TemporaryQueue * tmpQueue, int timeout);
            
            	virtual ~JmsApplyOffset();
            
            
            private:
               /**
                * @throw CommunicationException
                * If the initialization fails due to a connection error.
                */
            	JmsApplyOffset() noexcept(false);
            	std::string instName;
            
            };
         
         }
      }
   }
}

#endif /* SRC_GEMINI_TCS_JMS_JMSAPPLYOFFSET_H_ */


