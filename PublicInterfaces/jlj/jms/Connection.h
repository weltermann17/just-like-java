// Connection.h

//********************************************************************

#ifndef jlj_jms_Connection_h
#define jlj_jms_Connection_h

//********************************************************************

#include <jlj/jms/Exceptions.h>
#include <jlj/lang/Object.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

struct ExportedByJLJjms Session;

//********************************************************************

class ExportedByJLJjms ConnectionI 
	: public ObjectI
{
public:
	virtual ~ConnectionI() = 0;
	virtual void close() throw (JMSException) = 0;
	virtual void start() throw (JMSException) = 0;
	virtual void stop() throw (JMSException) = 0;
	virtual Session createSession() throw (JMSException) = 0;
	
};

//********************************************************************

StrongReferenceCommonDeclaration(Connection, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
