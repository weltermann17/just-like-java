// AnythingMessage.h

//********************************************************************

#ifndef jmstools_implementation_reactor_AnythingMessage_h
#define jmstools_implementation_reactor_AnythingMessage_h

//********************************************************************

#include <jlj/jms/AnythingMessage.h>
#include <jmstools/implementation/reactor/AbstractMessage.h>

using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation AnythingMessageI 
	: public jlj::jms::AnythingMessageI
	, public jmstools::implementation::reactor::AbstractMessageI
{
protected:
	~AnythingMessageI();
	friend class TypedReference<AnythingMessageI>;

public:
	AnythingMessageI();
	AnythingMessageI(Anything);

	Anything getAnything() const;
	void setAnything(Anything);

};

//********************************************************************

struct ExportedByJMSreactorimplementation AnythingMessage : StrongReference<AnythingMessageI>
{
	StrongReferenceCommonDerivateImplementation(AnythingMessage);
};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
