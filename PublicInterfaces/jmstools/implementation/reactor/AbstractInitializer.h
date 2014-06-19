// AbstractInitializer.h

//********************************************************************

#ifndef jmstools_implementation_reactor_AbstractInitializer_h
#define jmstools_implementation_reactor_AbstractInitializer_h

//********************************************************************

#include <jmstools/implementation/reactor/Initializer.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/IntAnything.h>

using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation AbstractInitializerI
	: public InitializerI
{
protected:
	~AbstractInitializerI();
	friend class TypedReference<AbstractInitializerI>;

public:
	AbstractInitializerI();

public:
	String getValueAsString(const String& name) const throw (Exception);
	int getValueAsInt(const String& name) const throw (Exception);
	jlj::util::logging::Level getLoggingLevel() const;
	List<jlj::util::logging::Handler> getLoggingHandlers() const;

	String toString() const;
		
protected:
	MapAnything namevaluepairs;

};

//********************************************************************

StrongReferenceCommonDeclaration(AbstractInitializer, ExportedByJMSreactorimplementation);

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof