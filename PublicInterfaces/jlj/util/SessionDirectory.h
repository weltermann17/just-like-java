// SessionDirectory.h

//********************************************************************

#ifndef SessionDirectory_h
#define SessionDirectory_h

//********************************************************************

#include <jlj/lang/String.h>
#include <jlj/lang/Throwable.h>
using namespace jlj::lang;
#include <jlj/util/AbstractNamedSingleton.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil SessionDirectoryI 
	: public ObjectI
{
public:
	~SessionDirectoryI();	
	SessionDirectoryI() throw (Exception);	

public:
	String getPath() const;

private:
	String path;

};

//********************************************************************

struct ExportedByJLJutil SessionDirectory 
	: StrongReference<SessionDirectoryI>
	, AbstractNamedSingleton<SessionDirectory, SessionDirectoryI>
{
	StrongReferenceCommonDerivateImplementation(SessionDirectory);
};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
