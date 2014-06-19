//# Request.h

//********************************************************************

#ifndef jmstools_request_h
#define jmstools_request_h

//********************************************************************

#include <JMSToolstests.h>

#include <jlj/util/concurrent/Callable.h>
#include <jlj/util/StreamAnything.h>
#
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/Reactor.h>

using namespace jlj::util::concurrent;
using namespace jlj::util;
using namespace jlj::nio::reactor;
using namespace jlj::net;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSToolstests RequestI 
	: public CallableI<int>
{

protected:

	~RequestI();
	friend class TypedReference<RequestI>;

public:

	RequestI(Connection, Reactor, Logger, BlockingQueue<int>, String& type, String& path,int minblocksize,int maxblocksize, int innerloop);
	int call() throw (jlj::lang::Exception);

private:

	void sendMessage(Anything message) const throw (IOException);
	UTF8String createRandomString(int length) const;
	Anything createMessageFromFile(String& filename) throw (IOException);
	Anything createMessageHeader();
	Anything createRandomMessage(int min,int max);
	Anything receiveAnswer() throw (IOException);

private:

	Connection connection;
	Reactor reactor;
	Logger logger;
	BlockingQueue<int> requeststopped;
	int minblocksize;
	int maxblocksize;
	int innerloop;
	String type;
	String path;
};

//********************************************************************

StrongReferenceCommonDeclaration(Request, ExportedByJMSToolstests);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof




