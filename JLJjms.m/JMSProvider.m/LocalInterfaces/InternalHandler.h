// InternalHandler.h

//********************************************************************

#ifndef jmstools_jmsprovider_InternalHandler_h
#define jmstools_jmsprovider_InternalHandler_h

//********************************************************************

#include <Handler.h>
#include <Consumers.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/nio/reactor/Connection.h>

using namespace jmstools::implementation::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProvider InternalHandlerI
    : public HandlerI
{
public:
    InternalHandlerI(Consumers, jlj::nio::reactor::Connection, Message);

public:
    void handle() throw (Exception);

protected:
    ~InternalHandlerI();
    friend class TypedReference<InternalHandlerI>;

private:
    void sendAcknowledgment(int) throw (Exception);

private:
    Consumers consumers;
    Message message;
    jlj::nio::reactor::Connection connection;
};

//********************************************************************

StrongReferenceCommonDeclaration(InternalHandler, ExportedByJMSProvider);

//********************************************************************

NAMESPACE_END(jmstools)

#endif

// eof