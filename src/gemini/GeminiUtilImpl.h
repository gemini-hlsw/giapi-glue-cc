#ifndef GEMINIUTILIMPL_H_
#define GEMINIUTILIMPL_H_

#include <tr1/memory>
#include <log4cxx/logger.h>
#include <giapi/giapi.h>
#include <giapi/EpicsStatusHandler.h>
#include <gemini/epics/EpicsManager.h>
#include <gemini/epics/EpicsFetcher.h>
#include <gemini/pcs/PcsUpdater.h>
#include <gemini/tcs/TcsFetcher.h>
#include "tcs/ApplyOffset.h"

#include <stdexcept>
//Required for exception handling

namespace giapi {

/**
 * This is the delegate class used to implement the behavior
 * described in the Gemini Util class.
 */
class GeminiUtilImpl;
typedef std::tr1::shared_ptr<GeminiUtilImpl> pGeminiUtilImpl;

class GeminiUtilImpl {
	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

public:
	/**
 	* @throw GiapiException
	* If there is an error initializing the GeminiUtil instance,
 	*        typically due to issues with the EPICS Manager, PCS Updater, or TCS Fetcher initialization.
 	*/
	static pGeminiUtilImpl Instance() noexcept(false);

	/**
 	* @throw GiapiException
	* If subscribing to the EPICS status channel fails,
 	*        possibly due to connection issues or invalid channel names.
 	*/
	int subscribeEpicsStatus(const std::string &name, pEpicsStatusHandler handler) noexcept(false);

	int unsubscribeEpicsStatus(const std::string &name);

	int postPcsUpdate(double zernikes[], int size);

	/**
 	* @throw GiapiException
	* If there is an error retrieving the TCS context,
 	*        possibly due to a timeout or connection failure.
 	*/
	int getTcsContext(TcsContext& ctx, long timeout) const noexcept(false);

	/**
 	* @throw GiapiException
	* If there is an error applying the telescope offset,
 	*        potentially due to communication issues with the TCS system.
 	*/
	int tcsApplyOffset(const double p, const double q,
			           const OffsetType offsetType, const long timeout)const noexcept(false);

	/**
 	* @throw GiapiException
	* If there is an error applying the telescope offset with a callback,
 	*        possibly due to a timeout or connection failure.
 	*/
	int tcsApplyOffset(const double p, const double q,
		               const OffsetType offsetType, const long timeout,
		               void (*callbackOffset)(int, std::string)) const noexcept(false);

	/**
 	* @throw GiapiException
	* If there is an issue retrieving the EPICS channel data,
 	*        typically due to a timeout or communication failure.
 	*/
	pEpicsStatusItem getChannel(const std::string &name, long timeout) noexcept(false);

	virtual ~GeminiUtilImpl();
private:
	static pGeminiUtilImpl INSTANCE;

	/**
	 * Manager of Epics subscriptions
	 */
	pEpicsManager _epicsMgr;

	/**
	 * The PCS updater object
	 */
	gemini::pcs::pPcsUpdater _pcsUpdater;

	/**
	 * The TCS fetcher object
	 */
	gemini::tcs::pTcsFetcher _tcsFetcher;

        gemini::tcs::pTcsOffset     _tcsApplyOffset;

	/**
	 * The EPICS fetcher object
	 */
	gemini::epics::pEpicsFetcher _epicsFetcher;

	/**
 	* @throw GiapiException
	* EXPLICAR AQUI
 	*/
	GeminiUtilImpl() noexcept(false);

};

}

#endif /*GEMINIUTILIMPL_H_*/
