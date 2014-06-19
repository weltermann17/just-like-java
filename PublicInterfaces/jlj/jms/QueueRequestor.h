// QueueRequestor.h

//********************************************************************

#ifndef jlj_jms_QueueRequestor_h
#define jlj_jms_QueueRequestor_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/jms/Message.h>

using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms QueueRequestorI 
	: public ObjectI
{
public:
	virtual ~QueueRequestorI() = 0;

public:
	virtual Message request(Message) throw (JMSException) = 0;
	virtual Message request(Message, int timeout) throw (JMSException) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueRequestor, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
