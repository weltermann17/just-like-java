//JMSProviderManager.h

//********************************************************************
#ifndef jmstools_ProviderManager_h
#define jmstools_ProviderManager_h

//********************************************************************
#include <JMSProviderManager.h>

#include <jlj/lang/String.h>
#include <jlj/lang/Object.h>
#include <jlj/jms/QueueSender.h>
#include <jlj/jms/QueueSession.h>
#include <jlj/jms/QueueConnectionFactory.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/jms/Exceptions.h>
#include <jlj/jms/AnythingMessage.h>
#include <jlj/util/Anything.h>
#include <jlj/nio/reactor/Connection.h>

using namespace jlj::lang;
using namespace jlj::jms;
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProviderManager ProviderManagerI
	: public ObjectI
{
public:
	ProviderManagerI(QueueConnectionFactory, Logger);

public:
	int shutdown(int timeout) throw (JMSException);
	int monitor(int timeout, bool nagios) throw (JMSException);
	void reloadParameters(String filename) throw (JMSException);
	void setLogLevel(String level) throw (JMSException);
	void setGlobalLogLevel(String level) throw (JMSException);
	void setReactorLogLevel(String level) throw (JMSException);
	void setConsoleHandlerLogLevel(String level) throw (JMSException);
	void usage();
	bool isNagiosOn(const String&) const;
	void start(const String& command, const String& parameter, bool nagios) throw (JMSException);

protected:
	~ProviderManagerI();
	friend class TypedReference<ProviderManagerI>;

private:
	void sendLogLevel(int command,String level) throw (JMSException);
	void createControlConnection() throw (JMSException);
	bool isLogLevelAllowed(String) const;
	bool isLoggingFile(const String&) const;
	int getTimeOut(const String&) const;
private:
	Logger logger;
	QueueConnectionFactory queueconnectionfactory;
	QueueSession session;
	jlj::jms::Queue queue;

private:
	AnythingMessage createMessage(jlj::util::Anything message) throw (JMSException);
	QueueSender createQueueSender() throw (JMSException);
	QueueConnection connection;
};

//********************************************************************

StrongReferenceCommonDeclaration(ProviderManager, ExportedByJMSProviderManager);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************
#endif

// eof