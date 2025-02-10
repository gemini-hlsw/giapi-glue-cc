/*
 * AbstractStatusSender.cpp
 *
 *  Created on: Dec 18, 2008
 *      Author: anunez
 */

#include <status/senders/AbstractStatusSender.h>
#include <status/StatusDatabase.h>

namespace giapi {

log4cxx::LoggerPtr AbstractStatusSender::logger(log4cxx::Logger::getLogger(
		"giapi.AbstractStatusSender"));

AbstractStatusSender::AbstractStatusSender() {
}

AbstractStatusSender::~AbstractStatusSender() {
}

int AbstractStatusSender::postStatus(const std::string &name) const
		noexcept(false) {
		//Fixed for exception handling in the C++20 version.

	pStatusItem statusItem = StatusDatabase::Instance()->getStatusItem(name);

	if (statusItem.get() == 0) {
		LOG4CXX_WARN(logger, "No status item found for " << name << ". Not posting");
		return status::ERROR;
	}

	return doPost(statusItem);
}

/*
 * TODO. This is necessary to stdc++20 and we have to put the logic
 * in this rutime. @std_work
 */

int AbstractStatusSender::postStatus(pStatusItem item) const noexcept(false) {
    return 0;
}

int AbstractStatusSender::postStatus() const noexcept(false) {
	//Fixed for exception handling in the C++20 version.
	//get the status items
	const std::vector<pStatusItem> items =
			StatusDatabase::Instance()->getStatusItems();
	//and post the ones that haven't changed. Clear their status
	for (std::vector<pStatusItem>::const_iterator it = items.begin(); it
			!= items.end(); ++it) {
		pStatusItem item = *it;
		doPost(item);
	}
	return status::OK;
}


int AbstractStatusSender::doPost(pStatusItem statusItem) const noexcept(false) {
	//Fixed for exception handling in the C++20 version.
	if (statusItem.get() == 0)
		return giapi::status::ERROR;

	//value hasn't changed since last post, return immediately.
	if (!statusItem->isChanged()) {
		return status::ERROR;
	}

	statusItem->clearChanged(); //mark clean, so it can be posted again
	//Post It. Invoke an specific post mechanism delegated to implementors
	return postStatus(statusItem);
}

}
