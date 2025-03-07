#include <giapi/ServicesUtil.h>
#include "ServicesUtilImpl.h"
namespace giapi {

ServicesUtil::ServicesUtil() {
}

ServicesUtil::~ServicesUtil() {
}

void ServicesUtil::systemLog(log::Level level,
		const std::string &msg) noexcept(false){
	ServicesUtilImpl::Instance()->systemLog(level, msg);
}

long64 ServicesUtil::getObservatoryTime() noexcept(false){
	return ServicesUtilImpl::Instance()->getObservatoryTime();
}

const std::string ServicesUtil::getProperty(const std::string &key, long timeout) noexcept(false){
	return ServicesUtilImpl::Instance()->getProperty(key, timeout);
}

}
