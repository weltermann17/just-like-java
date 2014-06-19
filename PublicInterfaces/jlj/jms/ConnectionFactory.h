// ConnectionFactory.h

//********************************************************************

#ifndef jlj_jms_ConnectionFactory_h
#define jlj_jms_ConnectionFactory_h

//********************************************************************

#include <jlj/jms/Connection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms ConnectionFactoryI 
	: public ObjectI
{
public:
	virtual ~ConnectionFactoryI() = 0;
	virtual Connection createConnection() throw (JMSException) = 0;
	virtual void setConfiguration(const String& configuration) throw (JMSException) = 0;

};

//********************************************************************

struct ExportedByJLJjms ConnectionFactory : StrongReference<ConnectionFactoryI> 
{
	StrongReferenceCommonDerivateImplementation(ConnectionFactory);
	static ConnectionFactory lookup(
		const String& clazzname, 
		const String& library,
		const String& configuration) throw (Exception);
};

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
