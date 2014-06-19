// Forwarder.h

//********************************************************************

#ifndef jmstools_provider_Forwarder_h
#define jmstools_provider_Forwarder_h

//********************************************************************

#include <Handler.h>
#include <jlj/util/concurrent/Callable.h>
#include <jlj/util/Anything.h>
#include <jlj/nio/reactor/Connection.h>

using namespace jlj::util::concurrent;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProvider ForwarderI
	: public RunnableI
	, public HandlerI 

{
protected:
	~ForwarderI();
	friend class TypedReference<ForwarderI>;

public:
	ForwarderI(jlj::nio::reactor::Connection, jlj::util::Anything, bool jsonencoding);
	void handle() throw (Exception);

private:
	void run();

	jlj::util::Anything anything;
	jlj::nio::reactor::Connection connection;
	bool jsonencoding;

};

//********************************************************************

struct ExportedByJMSProvider Forwarder : StrongReference<ForwarderI>
{
	StrongReferenceCommonDerivateImplementation(Forwarder);
};

//********************************************************************

NAMESPACE_END(jmstools)

#endif

// eof