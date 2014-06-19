// JMSInitializer.h

//********************************************************************

#ifndef jmstools_jmsprovider_JMSInitializer_h
#define jmstools_jmsprovider_JMSInitializer_h

//********************************************************************

#include <JMSToolstests.h>

//********************************************************************

#include <jlj/lifetime/initialize/AbstractInitializer.h>
using namespace jlj::lifetime::initialize;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSToolstests JMSInitializerI
	: public AbstractInitializerI
{
protected:
	~JMSInitializerI();
	friend class TypedReference<JMSInitializerI>;

public:
	JMSInitializerI();
	JMSInitializerI(const String& filepath);

	void initialize() throw (Exception);

protected:
	String getLoggingLevelValue() const;
	String getLoggingHandlersValue() const;

};

//********************************************************************

StrongReferenceCommonDeclaration(JMSInitializer, ExportedByJMSToolstests);

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
