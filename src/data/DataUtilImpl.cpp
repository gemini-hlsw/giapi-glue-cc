#include "DataUtilImpl.h"

namespace giapi {

log4cxx::LoggerPtr DataUtilImpl::logger(log4cxx::Logger::getLogger("giapi.DataUtilImpl"));

pDataUtilImpl DataUtilImpl::INSTANCE(static_cast<DataUtilImpl *>(0));

DataUtilImpl::DataUtilImpl() noexcept(false) {
	pObsEventProducer = JmsObsEventProducer::create();
	pFileEventsProducer = JmsFileEventsProducer::create();
}

DataUtilImpl::~DataUtilImpl() {
	LOG4CXX_DEBUG(logger, "Destroying Data Util Service");
	pObsEventProducer.release(); //just to make sure the object is destroyed.
	pFileEventsProducer.release();
}

pDataUtilImpl DataUtilImpl::Instance() noexcept(false) {
	if (INSTANCE.get() == 0) {
		INSTANCE.reset(new DataUtilImpl());
	}
	return INSTANCE;
}

int DataUtilImpl::postObservationEvent(data::ObservationEvent event,
		const std::string & datalabel) noexcept(false) {

	return pObsEventProducer->postEvent(event, datalabel);

}

int DataUtilImpl::postAncillaryFileEvent(const std::string & filename,
		const std::string & datalabel) noexcept(false) {
//	LOG4CXX_INFO(logger, "postAncilliaryFileEvent: Filename " << filename
//			<< " datalabel " << datalabel);

	return pFileEventsProducer->postAncillaryFileEvent(filename, datalabel);
}

int DataUtilImpl::postIntermediateFileEvent(const std::string & filename,
		const std::string & datalabel, const std::string & hint) noexcept(false) {
//	LOG4CXX_INFO(logger, "postIntermediateFileEvent: Filename " << filename
//			<< " datalabel " << datalabel << " hint " << hint);
	return pFileEventsProducer->postIntermediateFileEvent(filename, datalabel, hint);
}

}
