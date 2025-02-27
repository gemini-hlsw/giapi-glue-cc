#ifndef DATAUTIL_H_
#define DATAUTIL_H_

#include <string>

#include <giapi/giapi.h>
#include <giapi/giapiexcept.h>

#include <stdexcept>
//Required for exception handling

namespace giapi {

/**
 * This class provides the mechanisms to update Gemini with information
 * required to integrate the data acquisition process inside the
 * instrument with the OCS.
 */
class DataUtil {
public:

	/**
	 * Send an observation event associated to the given datalabel to the
	 * GMP.
	 * This method should be called by the instrument at each phase of the
	 * execution of an observe sequence command.
	 *
	 * @param event The observation event to be sent, such as
	 *              data::OBS_START_ACQ
	 * @param datalabel the datalabel that is being generated by the
	 *                  observe command.
	 *
	 * @return status::OK if the event was dispatched properly or
	 *         status::ERROR if there is a problem.
	 *
	 * @throws GiapiException if there is an error accessing the GMP to post
	 *         the given Observation Event
	 */
	static int postObservationEvent(data::ObservationEvent event,
			const std::string & datalabel) noexcept(false);

	/**
	 * Send an ancillary file event associated to the given datalabel
	 * to the GMP.
	 *
	 * @param filename name of the ancillary file
	 * @param datalabel the data-label generated by an observe sequence
	 *        command that will be associated to this ancillary file
	 *
	 * @return status::OK if the event was dispatched properly or
	 *         status::ERROR if there is a problem.
	 *
	 * @throws GiapiException if there is an error accessing the GMP to post
	 *         the given Ancillary File Event
	 */
	static int postAncillaryFileEvent(const std::string & filename,
			const std::string & datalabel) noexcept(false);
	/**
	 * Send an intermediate file event associated to the given datalabel
	 * to the GMP and with the given hint.
	 *
	 * @param filename name of the intermediate file
	 * @param datalabel the data-label generated by an observe sequence
	 *        command that will be associated to this intermediate file
	 * @param hint hint The hint can be used to indicate different types
	 *        of intermediate files and is instrument dependent.
	 *
	 * @return status::OK if the event was dispatched properly or
	 *         status::ERROR if there is a problem.
	 *
	 * @throws GiapiException if there is an error accessing the GMP to
	 *         post the given Intermediate File Event
	 */
	static int postIntermediateFileEvent(const std::string & filename,
			const std::string & datalabel,
			const std::string & hint) noexcept(false);

private:
	DataUtil();
	virtual ~DataUtil();
};

}

#endif /*DATAUTIL_H_*/
