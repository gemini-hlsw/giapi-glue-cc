#include "giapi/DataUtil.h"
#include "DataUtilImpl.h"
namespace giapi {

DataUtil::DataUtil() {
}

DataUtil::~DataUtil() {
}

int DataUtil::postObservationEvent(data::ObservationEvent event,
		const std::string & datalabel) noexcept(false) {
	return DataUtilImpl::Instance()->postObservationEvent(event, datalabel);
}

int DataUtil::postAncillaryFileEvent(const std::string & filename,
		const std::string & datalabel) noexcept(false) {
	return DataUtilImpl::Instance()->postAncillaryFileEvent(filename, datalabel);
}

int DataUtil::postIntermediateFileEvent(const std::string & filename,
		const std::string & datalabel, const std::string & hint) noexcept(false) {
	return DataUtilImpl::Instance()->postIntermediateFileEvent(filename, datalabel, hint);
}

}
