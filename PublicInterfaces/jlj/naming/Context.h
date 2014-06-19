// Context.h

//********************************************************************

#ifndef jlj_naming_Context_h
#define jlj_naming_Context_h

//********************************************************************

#include <jlj/naming/Exceptions.h>
#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
#include <jlj/lang/Class.h>
#include <jlj/util/List.h>
#include <jlj/util/Map.h>
using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

class ExportedByJLJnaming ContextI
	: public virtual ObjectI
{
public:
	virtual ~ContextI() = 0;

public:
	virtual void bind(const String& name, Object) throw (NamingException) = 0;
	virtual void rebind(const String& name, Object) throw (NamingException) = 0;
	virtual bool isBound(const String& name) const = 0;
	virtual Object lookup(const String& name) const throw (NamingException) = 0;
	virtual Map<String, Class> list(const String& contextname) const throw (NamingException) = 0;
	virtual Map<String, Object> listBindings(const String& contextname) const throw (NamingException) = 0;
	virtual void unbind(const String& name) throw (NamingException) = 0;
	
};

//********************************************************************

StrongReferenceCommonDeclaration(Context, ExportedByJLJnaming);

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
