// AbstractInitializer.h

//********************************************************************

#ifndef jlj_lifetime_AbstractInitializer_h
#define jlj_lifetime_AbstractInitializer_h

//********************************************************************

#include <jlj/util/initialize/Initializer.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/IntAnything.h>

using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)
NAMESPACE_BEGIN(spi)

//********************************************************************

class ExportedByJLJjms AbstractInitializerI
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

StrongReferenceCommonDeclaration(AbstractInitializer, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(spi)
NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof