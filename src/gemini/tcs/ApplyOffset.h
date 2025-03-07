/*
 * ApplyOffset.h
 *
 *  Created on: Aug 17, 2023
 *      Author: framos
 */

#ifndef APPLYOFFSET_H_
#define APPLYOFFSET_H_

#include <tr1/memory>

#include <giapi/giapi.h>

#include <stdexcept>

namespace giapi {

   namespace gemini {
   
      namespace tcs {
      
         class ApplyOffset {
         
            public:

            	/**
            	 * Apply P and Q offset in the TCS.
            	 * @param p         : Size of the P offset. Units in arcseconds.
            	 * @param q         : Size of the Q offset. Units in arcseconds.
            	 * @param OffsetType: The type of offset can be ACQ (acquisition stage of the sequence)
            	 *                     or SLOWGUINDING (tiny offset sent during the integration).
            	 * @param timeout   : timeout in millisecond for the request to complete.
            	 *                    If not specified, the call will block until the GMP replies back
            	 * @return status::OK if the Offset was applied properly.
            	 *         status::ERROR if there was an error applying the offset
            	 * @throws GiapiException if there is an error accessing the TCS to
            	 *         apply the offset, or a timeout occurs.
            	 *
            	 */
            	virtual int sendOffset(const double p, const double q,
            			               const OffsetType offsetType, const long timeout) noexcept(false)=0;
            
                    /**
            	 * Apply P and Q offset in the TCS.
            	 * @param p             : Size of the P offset. Units in arcseconds.
            	 * @param q             : Size of the Q offset. Units in arcseconds.
            	 * @param OffsetType    : The type of offset can be ACQ (acquisition stage of the sequence)
            	 *                        or SLOWGUINDING (tiny offset sent during the integration).
            	 * @param timeout       : timeout in millisecond for the request to complete.
            	 *                        If not specified, the call will block until the GMP replies back.
                     * @param callbackOffset: Callback function to be called after TCS offset has been applied
                     *                    
            	 * @return status::OK if the Offset was applied properly.
            	 *         status::ERROR if there was an error applying the offset
            	 * @throws GiapiException if there is an error accessing the TCS to
            	 *         apply the offset, or a timeout occurs.
            	 *
            	 */
            	virtual int sendOffset(const double p, const double q,
            	                       const OffsetType offsetType, const long timeout,
            	                       void (*callbackOffset)(int, std::string)) noexcept(false)=0;
            
            	/**
            	 * Destructor
            	 */
            	virtual ~ApplyOffset() {};
            
            
         };
      
      /**
       * A smart pointer definition for the TcsFetcher class.
       */
      typedef std::tr1::shared_ptr<ApplyOffset> pTcsOffset;
      
      }
   }
}


#endif /* SRC_GEMINI_TCS_APPLYOFFSET_H_ */
