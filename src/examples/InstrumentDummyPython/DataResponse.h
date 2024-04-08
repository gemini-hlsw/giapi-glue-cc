#ifndef DATARESPONSE_H_
#define DATARESPONSE_H_

#include <iostream>
#include <giapi/giapi.h>
#include <giapi/SequenceCommandHandler.h>

using namespace giapi;

namespace instDummy {
        /*
	 * This class is used to save the response which will be sent to the GMP. 
	 */
	class DataResponse {
	   private:
		  HandlerResponse::Response _response;
		  std::string _msg;

	   public:
		  DataResponse () {
			  _response = HandlerResponse::Response::ERROR;
			  _msg = "";
		  }

		  DataResponse (HandlerResponse::Response response, std::string msg) {
			  _response = response;
			  _msg = msg;
		  }

		  virtual ~DataResponse() { }

		  inline HandlerResponse::Response getResponse() {
			  return _response;
		  }

		  inline std::string getMsg() {
			  return _msg;
		  }
	};
}
#endif
