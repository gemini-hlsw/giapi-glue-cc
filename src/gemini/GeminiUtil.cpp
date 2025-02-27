#include <giapi/GeminiUtil.h>
#include "GeminiUtilImpl.h"

namespace giapi
{

GeminiUtil::GeminiUtil()
{
}

GeminiUtil::~GeminiUtil()
{
}


int GeminiUtil::subscribeEpicsStatus(const std::string &name,
		pEpicsStatusHandler handler) noexcept(false){
			//Fixed for exception handling in the C++20 version.
	return GeminiUtilImpl::Instance()->subscribeEpicsStatus(name, handler);
}

int GeminiUtil::unsubscribeEpicsStatus(const std::string &name) noexcept(false){
	//Fixed for exception handling in the C++20 version.
	return GeminiUtilImpl::Instance()->unsubscribeEpicsStatus(name);
}

int GeminiUtil::postPcsUpdate(double zernikes[],
		int size) noexcept(false) {
			//Fixed for exception handling in the C++20 version.
	return GeminiUtilImpl::Instance()->postPcsUpdate(zernikes, size);
}

int GeminiUtil::getTcsContext(TcsContext& ctx, long timeout) noexcept(false) {
	//Fixed for exception handling in the C++20 version.
	return GeminiUtilImpl::Instance()->getTcsContext(ctx, timeout);
}

int GeminiUtil::tcsApplyOffset(const double p, const double q, const OffsetType offsetType, const long timeout) noexcept(false) {
	//Fixed for exception handling in the C++20 version.
	return GeminiUtilImpl::Instance()->tcsApplyOffset(p, q, offsetType, timeout);
}

int GeminiUtil::tcsApplyOffset(const double p, const double q,
                              const OffsetType offsetType, const long timeout,
                              void (*callbackOffset)(int, std::string)) noexcept(false) {
								//Fixed for exception handling in the C++20 version.

	return GeminiUtilImpl::Instance()->tcsApplyOffset(p, q, offsetType, timeout, callbackOffset);

}

pEpicsStatusItem GeminiUtil::getChannel(const std::string &name, long timeout) noexcept(false)  {
	//Fixed for exception handling in the C++20 version.
  return GeminiUtilImpl::Instance()->getChannel(name, timeout);
}

}


