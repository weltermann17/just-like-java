// AnythingMessage.h

//********************************************************************

#ifndef jlj_jms_AnythingMessage_h
#define jlj_jms_AnythingMessage_h

//********************************************************************

#include <jlj/jms/Message.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms AnythingMessageI 
	: public virtual MessageI
{
public:
	virtual ~AnythingMessageI() = 0;
	virtual Anything getAnything() const = 0;
	virtual void setAnything(Anything) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(AnythingMessage, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
