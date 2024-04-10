
#include <iostream>
#include <signal.h>

#include <decaf/util/concurrent/CountDownLatch.h>

#include <giapi/CommandUtil.h>
#include <giapi/SequenceCommandHandler.h>
#include <giapi/giapi.h>
#include "InstCmdHandler.h"
#include "DataResponse.h"
#include <mutex>

using namespace giapi;
using namespace std;

using namespace instDummy;

int actionId = -1;
std::mutex m1;

string getCommandName(short idSeqCmd) {
   switch (idSeqCmd) {
	   case giapi::command::TEST:
		   return "TEST";
	   case giapi::command::REBOOT:
		   return "REBOOT";
           case giapi::command::INIT:
		   return "INIT";
	   case giapi::command::DATUM:
		   return "DATUM";
           case giapi::command::PARK:
		   return "PARK";
           case giapi::command::VERIFY:
		   return "VERIFY";
           case giapi::command::END_VERIFY:
		   return "END_VERIFY";
           case giapi::command::GUIDE:
		   return "GUIDE";
           case giapi::command::END_GUIDE:
		   return "END_GUIDE";
           case giapi::command::APPLY:
		   return "APPLY";
           case giapi::command::OBSERVE:
		   return "OBSERVE";
           case giapi::command::END_OBSERVE:
		   return "END_OBSERVE";
           case giapi::command::PAUSE:
		   return "PAUSE";
           case giapi::command::CONTINUE:
		   return "CONTINUE";
           case giapi::command::STOP:
		   return "STOP";
           case giapi::command::STOP_CYCLE:
		   return "STOP_CYCLE";
	   case giapi::command::ABORT:
		   return "ABORT";
           default:
		  cout << " Error not sequecen command analyzed" << endl;
   }

}

DataResponse callback(giapi::command::ActionId actID, giapi::command::SequenceCommand seq, giapi::command::Activity act, giapi::pConfiguration conf)
{
    std::string str= "Command executed well";
    cout<< "the callback function has been called and executed the " << getCommandName(seq) << " command. " << endl;
    DataResponse data = DataResponse(HandlerResponse::STARTED, str);
    actionId = actID;
    m1.unlock();
    return data;
}



int main (int argc, char *argv[])
{
    try
    {

        /*
         *  Subscribe to all Sequence Commands (except Apply), using all
         *  of the same class (cAllSeqCmds):
         *      TEST, REBOOT, INIT, DATUM, PARK, VERIFY, END_VERIFY, GUIDE,
         *      END_GUIDE, OBSERVE, END_OBSERVE, PAUSE, CONTINUE, STOP,
         *      ABORT
         *  When a command comes in it executes the "handle" method.
         */
    	pSequenceCommandHandler seqHandler = instDummy::InstCmdHandler::create(callback);

        for (int cmd = giapi::command::TEST;
        		 cmd != giapi::command::ENGINEERING; cmd++)
        {
		cout << "subscribing to " << getCommandName(cmd) << " using callback function" << endl;
        	CommandUtil::subscribeSequenceCommand( (giapi::command::SequenceCommand) cmd, command::SET_PRESET_START, seqHandler );
        }
	cout << "Main process wait forever " <<  endl;
	while (1) {
	    cout << "Main process waiting " <<  endl;
	    m1.lock();
	    cout << "ActionId: " <<  actionId << endl;
	    CommandUtil::postCompletionInfo(actionId, HandlerResponse::create(HandlerResponse::COMPLETED));
            
	}
    }
    catch (GiapiException &s )
    {
        cout<< "GiapiException happened" << endl;
        return -1;
    }
    catch (...)
    {
        cout << "It is not a GiapiException " << endl;
        return -1;
    }


} /* end of main() */
