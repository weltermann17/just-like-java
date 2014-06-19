// TextMessage.h

//********************************************************************

#ifndef jmstools_implementation_reactor_TextMessage_h
#define jmstools_implementation_reactor_TextMessage_h

//********************************************************************

#include <jlj/jms/TextMessage.h>
#include <jmstools/implementation/reactor/AbstractMessage.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation TextMessageI 
	: public jlj::jms::TextMessageI
	, public jmstools::implementation::reactor::AbstractMessageI
{
protected:
	~TextMessageI();
	friend class TypedReference<TextMessageI>;

public:
	TextMessageI();
	TextMessageI(Anything);

	String getText() const;
	void setText(const String&);

private:
	mutable String text;

};

//********************************************************************

struct ExportedByJMSreactorimplementation TextMessage : StrongReference<TextMessageI>
{
	StrongReferenceCommonDerivateImplementation(TextMessage);
};

//********************************************************************
NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
