#include "ConnectionManager.h"
#include <src/util/PropertiesUtil.h>
#include <src/util/StringUtil.h>

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <activemq/library/ActiveMQCPP.h>
#include <decaf/io/IOException.h>
#include <iostream>

namespace gmp {
log4cxx::LoggerPtr ConnectionManager::logger(log4cxx::Logger::getLogger("giapi.gmp.ConnectionManager"));

//Time to wait (in seconds) to attempt a new connection in case the connection is lost
int ConnectionManager::RETRY_TIMEOUT = 10;

pConnectionManager ConnectionManager::INSTANCE(new ConnectionManager());

ConnectionManager::ConnectionManager() {
	//Initialize the ActiveMQ Library before it is used
	activemq::library::ActiveMQCPP::initializeLibrary();
}

ConnectionManager::~ConnectionManager() {

	LOG4CXX_DEBUG(logger, "Destroying connection manager");
	try {
		if (_connection.get() != 0) {
			_connection->stop();
		}
	}catch (CMSException &e) {
		LOG4CXX_WARN(logger, "Problem closing JMS Connection");
	}
	//release all the references to the objects stored
	_errorHandlersFunctions.clear();
	_errorHandlerObjects.clear();
	//TODO: shut down the ActiveMQ library (with CMS3.1.1 this
	//call is throwing an IOException
	//activemq::library::ActiveMQCPP::shutdownLibrary();
	LOG4CXX_DEBUG(logger, "Connection manager destroyed");

}

void ConnectionManager::registerOperation(giapi_error_handler op) {
	_errorHandlersFunctions.insert(op);
}

void ConnectionManager::registerHandler(pGiapiErrorHandler handler) {
	_errorHandlerObjects.insert(handler);
}


std::string ConnectionManager::createUri() {
	
    std::string hostname = giapi::util::PropertiesUtil::Instance().getProperty("gmp.hostname");
    if(giapi::util::StringUtil::isEmpty(hostname)){
        hostname = std::string("localhost");
    }

    std::string brokerURI =
        "failover:(tcp://"+hostname+":61616"
        "?wireFormat=openwire"
        //"&transport.useInactivityMonitor=false"
        //"&connection.alwaysSyncSend=true"
        "&connection.useAsyncSend=true"
        //"&transport.commandTracingEnabled=true"
        //"&transport.tcpTracingEnabled=true"
        //"&wireFormat.tightEncodingEnabled=true"
        ")?startupMaxReconnectAttempts=1&initialReconnectDelay=100";

	LOG4CXX_DEBUG(logger, "Broker URI: " << brokerURI);
	return brokerURI;

}

void ConnectionManager::startup() noexcept(false) {
	try {
		std::string brokerURI = createUri();
		LOG4CXX_DEBUG(logger, "Connecting to GMP broker at: " << brokerURI);
		std::auto_ptr<ConnectionFactory> connectionFactory(
				ConnectionFactory::createCMSConnectionFactory( brokerURI ));

		
		// Create a Connection
		_connection.reset(connectionFactory->createConnection());

		_connection->start();

		_connection->setExceptionListener(this);

	} catch (CMSException& e) {
		LOG4CXX_ERROR(logger, "Problem connecting to GMP. " << e.getMessage());
		throw GmpException("Problem connecting to GMP. " + e.getMessage());
	}
	catch (const std::exception& e) {
		LOG4CXX_ERROR(logger, "Standard exception connecting to GMP: " << e.what());
		throw GmpException(std::string("Problem connecting to GMP: ") + e.what());
    } catch (...) {
        LOG4CXX_ERROR(logger, "Unknown exception connecting to GMP");
        throw GmpException("Unknown problem connecting to GMP");
    }
}

pConnectionManager ConnectionManager::Instance() noexcept(false) {
	//if not connected, try to reconnect:
	if (INSTANCE->_connection.get() == 0) {
		INSTANCE->startup();
	}
	return INSTANCE;
}

void ConnectionManager::onException(const CMSException & ex) {
	LOG4CXX_ERROR(logger, "Communication Exception occurred ");
	ex.printStackTrace();
	if (_connection.get() != 0) {
		_connection->stop();
	}
	//reset the connection, so next time it will try to reconnect...
	_connection.reset(static_cast<Connection *>(0));
	//start a reconnection loop...
	decaf::util::concurrent::CountDownLatch lock(1);
	LOG4CXX_INFO(logger, "Waiting " << RETRY_TIMEOUT << " to attempt reconnection...");
	lock.await(RETRY_TIMEOUT * 1000);

	LOG4CXX_INFO(logger, "Attempting reconnection...");

	bool connected = false;
	while (!connected) {
		try {
			LOG4CXX_INFO(logger, "Attempting reconnection...");
			startup();
			connected = true;
		} catch (GmpException &e) {
			LOG4CXX_INFO(logger, "Problem attempting reconnection.. " << e.getMessage());
			LOG4CXX_INFO(logger, "Waiting " << RETRY_TIMEOUT << " seconds before attempting reconnection");
			lock.await(RETRY_TIMEOUT * 1000);
		}
	}

	LOG4CXX_INFO(logger, "Connection recovered. Invoking user provided error handlers");

	for (const auto &handler : _errorHandlersFunctions) {
		handler();
	}

	// Invoke object-based error handlers
	for (const auto &handlerObject : _errorHandlerObjects) {
		handlerObject->onError();
	}

}

pConnection ConnectionManager::createDedicatedConnection() {
    std::string brokerURI = createUri();
    LOG4CXX_DEBUG(logger, "Creating dedicated connection for consumer, broker URI: " << brokerURI);
	try {
		// Create a new connection factory (you may already store broker URI internally)
    	ConnectionFactory* factory = ConnectionFactory::createCMSConnectionFactory(brokerURI);
    	Connection* rawConnection = factory->createConnection();
    	LOG4CXX_DEBUG(logger, "Connection created, broker URI: " << brokerURI);
    	return pConnection(rawConnection);
	} catch (const CMSException& e) {
		LOG4CXX_ERROR(logger, "Error creating dedicated connection: " << e.what());
		throw CommunicationException("Error creating dedicated connection: " + std::string(e.what()));
	}
}

pSession ConnectionManager::createSession(cms::Session::AcknowledgeMode acknowledgeMode) noexcept(false) {
	pSession session(_connection->createSession(acknowledgeMode));
	return session;
}

pSession ConnectionManager::createSession() noexcept(false) {
	return createSession(cms::Session::CLIENT_ACKNOWLEDGE);
}

}
