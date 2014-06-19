// jlj_lifetime_JLJLifetimeImplementation.h

//********************************************************************

#ifndef jlj_lifetime_JLJLifetimeImplementation_h
#define jlj_lifetime_JLJLifetimeImplementation_h

//********************************************************************

#include "JLJlifetime.h"

//********************************************************************

#include <jlj/lang/lifetimemanagement/AbstractLifetimeImplementation.h>
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lifetime)

//********************************************************************

class ExportedByJLJlifetime JLJLifetimeImplementationI
	: public AbstractLifetimeImplementationI
{
public:
	~JLJLifetimeImplementationI() throw (jlj::lang::Exception); 
	JLJLifetimeImplementationI(const String& defaultconfiguration) throw (jlj::lang::Exception); 
	
	void initialize() throw (jlj::lang::Exception);
	void finalize() throw (jlj::lang::Exception);

private:
	bool initialized;
	String defaultconfiguration;

};
	
//********************************************************************

NAMESPACE_END(lifetime)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
