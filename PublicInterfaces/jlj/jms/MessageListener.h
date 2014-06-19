// MessageListener.h

//********************************************************************

#ifndef jlj_jms_MessageListener_h
#define jlj_jms_MessageListener_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/jms/Message.h>
#include <jlj/jms/Exceptions.h>

using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms MessageListenerI 
	: public virtual ObjectI
{
public:
	virtual ~MessageListenerI() = 0;
	
public:
	virtual void onMessage(Message message) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(MessageListener, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************
#endif

// eof
