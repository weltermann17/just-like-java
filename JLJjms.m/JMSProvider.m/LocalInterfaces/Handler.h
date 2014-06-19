//Handler.h

//********************************************************************

#ifndef jmstools_jmsprovider_Handler_h
#define jmstools_jmsprovider_Handler_h

//********************************************************************

#include <JMSProvider.h>
#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>

using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProvider HandlerI
	: public ObjectI
{

public:
	virtual ~HandlerI() = 0;
	HandlerI();

public:
	virtual void handle() throw (Exception) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(Handler, ExportedByJMSProvider);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
