// QueueConnectionFactory.h

//********************************************************************

#ifndef jlj_jms_QueueConnectionFactory_h
#define jlj_jms_QueueConnectionFactory_h

//********************************************************************

#include <jlj/jms/ConnectionFactory.h>
#include <jlj/jms/QueueConnection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms QueueConnectionFactoryI
	: public ConnectionFactoryI
{
public:
	virtual ~QueueConnectionFactoryI() = 0;
	virtual QueueConnection createQueueConnection() throw (JMSException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(QueueConnectionFactory, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
