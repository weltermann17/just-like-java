// Reply.h

//********************************************************************

#ifndef jmstools_Reply_h
#define jmstools_Reply_h

//********************************************************************

#include <JMSToolstests.h>

#include <jlj/util/concurrent/Callable.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/Reactor.h>

using namespace jlj::util::concurrent;
using namespace jlj::nio::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************
class ExportedByJMSToolstests ReplyI
	: public CallableI<int>
{
protected:
	~ReplyI();
	friend class TypedReference<ReplyI>;

public:
	ReplyI(Connection, Reactor, Logger, BlockingQueue<int>, int);

	int call() throw (jlj::lang::Exception);

private:
	String createRandomString(int) const;
	Anything createReplyMessage(Anything receivemessage, Anything answermessage);
	Anything createInternalMessage(int command);
	void sendMessage(Anything message) const;

private:
	Connection connection;
	Reactor reactor;
	Logger logger;
	BlockingQueue<int> replystopped;
	int replylength;

	static AtomicBoolean registeronce;

};

//********************************************************************

StrongReferenceCommonDeclaration(Reply, ExportedByJMSToolstests);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof




