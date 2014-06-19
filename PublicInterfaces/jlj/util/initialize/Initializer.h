// Initializer.h

//********************************************************************

#ifndef jlj_lifetime_Initializer_h
#define jlj_lifetime_Initializer_h

//********************************************************************

#include <JLJlifetime.h>

#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
#include <jlj/jms/Exceptions.h>
#include <jlj/util/List.h>
#include <jlj/util/logging/Logger.h>

using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(initialize)

//********************************************************************

class ExportedByJLJjms InitializerI
	: public ObjectI
{
public:
	virtual ~InitializerI() = 0;

public:
	virtual void initialize() throw (Exception) = 0;
	
	virtual String getValueAsString(const String& name) const throw (Exception) = 0;
	virtual int getValueAsInt(const String& name) const throw (Exception) = 0;

	virtual jlj::util::logging::Level getLoggingLevel() const = 0;
	virtual List<jlj::util::logging::Handler> getLoggingHandlers() const = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Initializer, ExportedByJLJlifetime);

//********************************************************************

NAMESPACE_END(initialize)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
