/**
 * Example program to subscribe and process a sequence command
 */

#include <iostream>
#include <signal.h>

#include <decaf/util/concurrent/CountDownLatch.h>

#include <giapi/CommandUtil.h>
#include <giapi/SequenceCommandHandler.h>
#include <giapi/giapi.h>
#include "ApplyHandler.h"


using namespace giapi;

decaf::util::concurrent::CountDownLatch endLock(1);

void terminate(int signal) {
    std::cout << "Exiting... " << std::endl;
    endLock.countDown();
    //exit(0);
}

/**
 * This example emulates the behavior of GHOST commands
 * where a sub command can complete before the other subcommand starts
 */
class GhostApplyHandler: public giapi::SequenceCommandHandler {

private:
  WorkerThread * worker;
  Thread* thread;

public:

  virtual giapi::pHandlerResponse handle(giapi::command::ActionId id,
      giapi::command::SequenceCommand sequenceCommand,
      giapi::command::Activity activity, giapi::pConfiguration config) {

    if (config != NULL) {
      std::vector<std::string> keys = config->getKeys();
      std::vector<std::string>::iterator it = keys.begin();
      printf("Configuration\n");
      for (; it < keys.end(); it++) {
        std::cout << "{" << *it << " : " << config->getValue(*it)
            << "}" << std::endl;
      }
    }

    printf("Starting worker thread on id %i\n", id);

    if (thread != NULL) {
      thread->join();// this is where  code must be smarter than
      //this, or else the invoker won't receive answers while
      //this thread is processing.
      delete thread;
    }
    thread = new Thread( worker );
    worker->setId(id);
    thread->start();
                printf("Sending started\n");
    return HandlerResponse::create(HandlerResponse::STARTED);
  }

  static giapi::pSequenceCommandHandler create(int totalMessages) {
    pSequenceCommandHandler handler(new GhostApplyHandler(totalMessages));
    return handler;
  }

  virtual ~GhostApplyHandler() {
    delete worker;
    if (thread != NULL) {
      thread->join();
      delete thread;
    }
  }

private:
  GhostApplyHandler(bool quick) {
                if (quick) {
      worker = new WorkerThread(1, 5);
    } else {
      worker = new WorkerThread(10, 500);
    }
    thread = NULL;
  }
};

int main(int argc, char **argv) {
  signal(SIGABRT, terminate);
  signal(SIGTERM, terminate);
  signal(SIGINT, terminate);

  try {

    std::cout << "Starting Apply Sequence Command Example" << std::endl;

    pSequenceCommandHandler handler1 = GhostApplyHandler::create(true);
    pSequenceCommandHandler handler2 = GhostApplyHandler::create(false);

    CommandUtil::subscribeApply("ghost:blue", command::SET_PRESET_START, handler1);
    CommandUtil::subscribeApply("ghost:red", command::SET_PRESET_START, handler2);

    //Wait until is killed
    endLock.await();
  } catch (GmpException &e) {
    std::cerr << "Is the GMP up?... Exiting" << std::endl;

  }
  return 0;
}

