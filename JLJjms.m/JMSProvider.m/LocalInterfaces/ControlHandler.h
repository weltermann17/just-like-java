// ControlHandler.h

//********************************************************************

#ifndef jmstools_jmsprovider_ControlHandler_h
#define jmstools_jmsprovider_ControlHandler_h

//********************************************************************

#include <Handler.h>
#include <Consumers.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/nio/reactor/Connection.h>

using namespace jmstools::implementation::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProvider ControlHandlerI
	: public HandlerI
{
protected:
   	~ControlHandlerI();
    friend class TypedReference<ControlHandlerI>;

public:
	ControlHandlerI(ExecutorService<int>, Reactor, Consumers, Connection, Message, int dropped, int handled);

public:
	void handle() throw (Exception);

private:
	void handleMonitor(bool nagios) throw (Exception);
	void handleShutdown() throw (Exception);
private:
	ExecutorService<int> threadpool;
	Reactor reactor;
	Consumers consumers;
	Message message;
	jlj::nio::reactor::Connection connection;

	static int olddropped;
	static int oldhandled;
	static int oldtotal;

	int dropped;
	int handled;
	
};

//********************************************************************

StrongReferenceCommonDeclaration(ControlHandler, ExportedByJMSProvider);

//********************************************************************

NAMESPACE_END(jmstools)


#endif

// eof