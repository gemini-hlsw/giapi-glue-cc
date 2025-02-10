#include <giapi/CommandUtil.h>
#include "LogCommandUtil.h"
#include "JmsCommandUtil.h"

#include <stdexcept> //For handle standard exceptions

namespace giapi {

CommandUtil::CommandUtil() {
}

CommandUtil::~CommandUtil() {
}

/**
 * @brief Subscribes to a sequence command.
 * @param id The sequence command ID.
 * @param activities The set of activities for the command.
 * @param handler The sequence command handler.
 * @return Status code indicating success or failure.
 * @throws GiapiException if the handler is not initialized.
 */
int CommandUtil::subscribeSequenceCommand(command::SequenceCommand id,
		command::ActivitySet activities,
		pSequenceCommandHandler handler) /*throw (GiapiException)*/ noexcept(false){ //Indicates the function may throw exceptions
	//Uninitialized sequence command handler
	/*if (handler == 0) {
		return giapi::status::ERROR;
	}
	return JmsCommandUtil::Instance()->subscribeSequenceCommand(id, activities, handler);*/
	 try {
        if (handler == nullptr) {
            throw GiapiException("Error: The sequence command handler is not initialized");
        }
        return JmsCommandUtil::Instance()->subscribeSequenceCommand(id, activities, handler);
    } catch (const std::exception &e) {
        // Catch any standard exceptions and return an error status
        std::cerr << "Exception in subscribeSequenceCommand: " << e.what() << std::endl;
        return giapi::status::ERROR;
    }

}

/**
 * @brief Subscribes to an apply command.
 * @param prefix The command prefix.
 * @param activities The set of activities for the command.
 * @param handler The apply command handler.
 * @return Status code indicating success or failure.
 * @throws GiapiException if the handler is not initialized.
 */
int CommandUtil::subscribeApply(const std::string & prefix,
		command::ActivitySet activities,
		pSequenceCommandHandler handler) /*throw (GiapiException)*/ noexcept(false){
	//Uninitialized sequence command handler
	/*if (handler == 0) {
		return giapi::status::ERROR;
	}
	return JmsCommandUtil::Instance()->subscribeApply(prefix, activities, handler);*/

	try {
        if (handler == nullptr) {
            throw GiapiException("Error: The apply handler is not initialized");
        }
        return JmsCommandUtil::Instance()->subscribeApply(prefix, activities, handler);
    } catch (const std::exception &e) {
        std::cerr << "Exception in subscribeApply: " << e.what() << std::endl;
        return giapi::status::ERROR;
    }
}


/**
 * @brief Posts completion information for a command action.
 * @param id The action ID.
 * @param response The response handler.
 * @return Status code indicating success or failure.
 * @throws GiapiException if the response is invalid.
 */
int CommandUtil::postCompletionInfo(command::ActionId id,
		pHandlerResponse response) /*throw (GiapiException)*/ noexcept(false) {
	//Uninitialized handler response for completion info
	/*if (response == 0) {
		return giapi::status::ERROR;
	}
	//Invalid responses for completion info.
	//Validates the response is either COMPLETED or ERROR. If
	//ERROR, check whether we have a message or not. It is
	//an error to generate an ERROR response without an error
	//message
	switch (response->getResponse()) {
		case HandlerResponse::COMPLETED:
			break; //all right
		case HandlerResponse::ERROR:
			if (!(response->getMessage().empty()))
				break; //all right
		default:
			//in all the other cases, return error
			return giapi::status::ERROR;
	}

	return JmsCommandUtil::Instance()->postCompletionInfo(id, response);*/

	try {
        if (response == nullptr) {
            throw GiapiException("Error: Completion response is not initialized");
        }

        // Validate response: must be either COMPLETED or ERROR with a valid message
        switch (response->getResponse()) {
            case HandlerResponse::COMPLETED:
                break;  // Valid response
            case HandlerResponse::ERROR:
                if (!(response->getMessage().empty()))
                    break;  // Valid error response
                [[fallthrough]];  // Allow falling through to default if validation fails
            default:
                throw GiapiException("Error: Invalid completion response");
        }

        return JmsCommandUtil::Instance()->postCompletionInfo(id, response);
    } catch (const std::exception &e) {
        std::cerr << "Exception in postCompletionInfo: " << e.what() << std::endl;
        return giapi::status::ERROR;
    }
}

}
