// ServiceHandler.h

//********************************************************************

#ifndef jmstools_util_services_ServiceHandler_h
#define jmstools_util_services_ServiceHandler_h

//********************************************************************

#include <jmstools/util/services/ServiceImplementor.h>

//********************************************************************

#include <jlj/lang/Runnable.h>
#include <jlj/util/AbstractNamedSingleton.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/MapAnything.h>
using namespace jlj::util;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;
#include <jlj/util/concurrent/BlockingQueue.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent;
using namespace jlj::util::concurrent::atomic;
#include <jlj/jms/TextMessage.h>
#include <jlj/jms/QueueSession.h>
using namespace jlj::jms;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

class ExportedByJMSTOOLSutil ServiceHandlerI
	: public virtual jlj::lang::RunnableI
{
protected:
	~ServiceHandlerI();
	friend class TypedReference<ServiceHandlerI>;

public:
	ServiceHandlerI(TextMessage, ServiceImplementor, QueueSession, Logger);
	
	void run();

	static int getCountRunning();

private:
	TextMessage textmessage;
	ServiceImplementor serviceimplementor;
	QueueSession queuesession;
	Logger logger;
	static AtomicInteger countrunning;

};

//********************************************************************

StrongReferenceCommonDeclaration(ServiceHandler, ExportedByJMSTOOLSutil);

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
