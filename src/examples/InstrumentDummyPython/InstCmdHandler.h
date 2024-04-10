#ifndef INSTCMDHANDLER_H_
#define INSTCMDHANDLER_H_

#include <iostream>
#include <giapi/giapi.h>
#include <giapi/SequenceCommandHandler.h>
#include <unistd.h>
#include "DataResponse.h"
unsigned int microsecond = 1000000;


using namespace giapi;
using namespace std;

namespace instDummy {

   // This is a pointer function to help implementing the python  function callback 
   // which will be called when a subscribed command is received. 
   typedef DataResponse (*callback_def)(giapi::command::ActionId, 
                                        giapi::command::SequenceCommand, 
                                        giapi::command::Activity, giapi::pConfiguration);

    
   /* 
    * This class facilitates subscribing to GMP commands (INIT, DATUM, OBSERVE, STOP, ABORT, ...) and
    * receiving the response to the callback function provided from Python.
    */
   class InstCmdHandler: public giapi::SequenceCommandHandler {
      
      private:
         /*Private attributes and methods */
         callback_def callback_f;
    
      public:
         /* This function is the responsible for receiving the commands (previously subscribed)
          * from the GMP and then makes the call to the callback function defined in python. 
          * The parameters of this funtions are the described in the Giapi C++ Language Glue API 
          * document, section 5.2. 
          * 
          * @params ActionId id : id the unique value that identifies the request or equivalently 
          *                       identifies the set of actions started by the request. 
          * @params SequenceCommand sequenceCommand : the sequence command that should be handled, 
          *                                           such as TEST or REBOOT.
          * @params Activity activity : Activity requested by the sender, like PRESET_START or CANCEL
          * @params pConfiguration config : A configuration associated to the command, as a smart pointer.
          */
        
         virtual giapi::pHandlerResponse handle(giapi::command::ActionId id,
                                                giapi::command::SequenceCommand sequenceCommand,
						giapi::command::Activity activity,
						giapi::pConfiguration config) {
      
               // Calling the python callback function. 
               DataResponse data = callback_f(id, sequenceCommand, activity, config);
               if (data.getResponse() ==  HandlerResponse::Response::ERROR)
                   return HandlerResponse::createError(data.getMsg());
               return HandlerResponse::create(data.getResponse());
         }
 
         /*
          * This function will create the InstCmdHandler object which will store the callback 
          * function reference. 
          */
         static giapi::pSequenceCommandHandler create(DataResponse (*callback)(giapi::command::ActionId,
				                                     giapi::command::SequenceCommand,
			                                             giapi::command::Activity,
	                                                             giapi::pConfiguration)) {
            pSequenceCommandHandler handler(new InstCmdHandler(callback));
            return handler;
	 }


         InstCmdHandler(DataResponse (*callback)(giapi::command::ActionId,
	                                         giapi::command::SequenceCommand,
						 giapi::command::Activity,
						 giapi::pConfiguration)) {
            callback_f = callback;
	 }

	 virtual ~InstCmdHandler() {}

   };

}

#endif /* INSTCMDHANDLEr*/
