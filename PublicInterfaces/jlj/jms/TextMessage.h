// TextMessage.h

//********************************************************************

#ifndef jlj_jms_TextMessage_h
#define jlj_jms_TextMessage_h

//********************************************************************

#include <jlj/jms/Message.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms TextMessageI 
	: public virtual MessageI
{
public:
	virtual ~TextMessageI() = 0;
	virtual String getText() const = 0;
	virtual void setText(const String&) = 0;
};

//*********************************************************************

StrongReferenceCommonDeclaration(TextMessage, ExportedByJLJjms);

//*********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//*********************************************************************

#endif

// eof
