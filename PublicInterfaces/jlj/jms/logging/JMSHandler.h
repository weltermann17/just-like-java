// jlj_util_logging_JMSHandler.h

//********************************************************************

#ifndef jlj_jms_logging_JMSHandler
#define jlj_jms_logging_JMSHandler

//********************************************************************
   
#include <jlj/util/logging/Handler.h>
#include <jlj/util/logging/Formatter.h>
#include <jlj/jms/MessageProducer.h>
#include <jlj/jms/Session.h>
using namespace jlj::util::logging;
using namespace jlj::jms;
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
using namespace jlj::util::concurrent::atomic;
#include <jlj/util/concurrent/locks/Lock.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJjms JMSHandlerI
	: public HandlerI
{
public:	
	~JMSHandlerI();
	JMSHandlerI(MessageProducer, Session);
	
public:
	void close();
	void flush();
	Formatter getFormatter() const;
	Level getLevel() const;
	bool isLoggable(const Level&) const;
	void publish(LogRecord);
	void setLevel(const Level&);
	void setFormatter(const Formatter&);

protected:
	void writeHead();
	void writeTail();

	Level level;
	Formatter formatter;
	AtomicInteger levelvalue;
	bool closed;
	Lock lock;

	MessageProducer messageproducer;
	Session session;

};

//********************************************************************

StrongReferenceCommonDeclaration(JMSHandler, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
