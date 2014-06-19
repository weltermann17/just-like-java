// QueueConnection.h

//********************************************************************

#ifndef jlj_jms_QueueConnection_h
#define jlj_jms_QueueConnection_h

//********************************************************************

#include <jlj/jms/Connection.h>
#include <jlj/jms/QueueSession.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms QueueConnectionI
	: public virtual ConnectionI
{
public:	
	virtual ~QueueConnectionI() = 0;

public:
	virtual QueueSession createQueueSession() throw (JMSException) = 0;
};

//********************************************************************

StrongReferenceCommonDeclaration(QueueConnection, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
