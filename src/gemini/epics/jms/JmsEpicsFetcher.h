#ifndef JMSEPICSFETCHER_H_
#define JMSEPICSFETCHER_H_

#include <gemini/epics/EpicsFetcher.h>
#include <util/jms/JmsProducer.h>

#include <stdexcept>
//Required for exception handling


namespace giapi {
namespace gemini {
namespace epics {

class JmsEpicsFetcher: public EpicsFetcher, util::jms::JmsProducer {

public:

  /**
   * Static factory method to instantiate a new JmsEpicsFetcher object
   * and obtain a smart pointer to access it.
   * @throw CommunicationException
   * If there is an issue establishing communication
   *        with the GMP when creating the fetcher instance.
   */
  static pEpicsFetcher create() noexcept(false);

  /**
   * @throw GiapiException
   * If the response received from the GMP is not valid
   *        or does not contain the expected data.
   *
   */
  virtual pEpicsStatusItem getChannel(const std::string &name, long timeout) noexcept(false);

private:

  /**
   * Private Constructor
   * @throw CommunicationException
   * If the initialization of the EPICS fetcher
   *        fails due to communication issues.
   *
   */
  JmsEpicsFetcher() noexcept(false);

};

}
}
}

#endif /* EPICSFETCHER_H_ */
