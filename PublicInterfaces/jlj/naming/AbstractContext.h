// AbstractContext.h

//********************************************************************

#ifndef jlj_naming_AbstractContext_h
#define jlj_naming_AbstractContext_h

//********************************************************************

#include <jlj/naming/Context.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/util/List.h>
#include <jlj/util/Anything.h>

using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

class ExportedByJLJnaming AbstractContextI
	: public ContextI
{
public:
	~AbstractContextI() = 0;
	AbstractContextI();

public:
	void bind(const String& name, Object) throw (NamingException);
	void rebind(const String& name, Object) throw (NamingException);
	bool isBound(const String& name) const;
	Object lookup(const String& name) const throw (NamingException);
	template< typename T > void lookup(const String& name, T&) const throw (NamingException);
	Map<String, Class> list(const String& contextname) const throw (NamingException);
	Map<String, Object> listBindings(const String& contextname) const throw (NamingException);
	void unbind(const String& name) throw (NamingException);

public:
	virtual void loadBindings(const String& configuration) throw (Exception);
	virtual void unloadBindings() throw (Exception);
	virtual void copyBinding(const String& from, const String& to) throw (NamingException);
	virtual Object lookupClass(const String& name, const String& clazzname) const throw (NamingException);

	/**
	* these two are extremely expensive,
	* they instanitiate all objects from all contexts
	*/
	virtual String prettyPrint(int maxvisiblevaluelength = -1) const; 
	String toString() const; 

	bool verboseOutput() const;

protected:
	virtual Object lookupObjectFactory(const String& name, bool& createdbyfactory, String& redirectedname) const throw (NamingException);
	template< typename T > void lookupURL(const String& name, T&) const throw (NamingException);
	virtual Object lookupURL(const String& url) const throw (NamingException);
	virtual String loadValueFromURL(const String& url) const throw (Exception);
	virtual String loadValueFromFile(const String& url) const throw (Exception);

private:
	void createBindings(Anything config, StringBuffer path, int level, bool rebind) throw (Exception);
	Object lookupLogger(const String& name) const throw (NamingException);
	Object lookupWithSyntaxCheck(const String& name, bool lookupalias) const throw (NamingException);

	mutable Map<String, Object> boundobjects;
	mutable Map<String, Class> boundclasses;
	bool verbose;
};

//********************************************************************

template< typename T > 
void AbstractContextI::lookup(const String& name, T& value) const throw (NamingException)
{	
	Object object = lookupClass(name, ClassI::normalizeClassName(typeid(value).name()));
	try 
	{ 
		object->downcast(value);
	}
	catch (ClassCastException& e)
	{
		throw NamingException(e->toString());
	}
}

//********************************************************************

template< typename T > 
void AbstractContextI::lookupURL(const String& name, T& value) const throw (NamingException)
{	
	Object object = lookupURL(name);
	object->downcast(value);
}

//********************************************************************

StrongReferenceCommonDeclaration(AbstractContext, ExportedByJLJnaming);

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
