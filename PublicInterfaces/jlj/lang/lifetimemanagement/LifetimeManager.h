// LifetimeManager.h

//********************************************************************

#ifndef jlj_lang_LifetimeManager_h
#define jlj_lang_LifetimeManager_h

//********************************************************************

#include <jlj/lang/lifetimemanagement/AbstractLifetimeImplementation.h>
#include <jlj/lang/String.h>
#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)
NAMESPACE_BEGIN(lifetimemanagement)

//********************************************************************

struct ExportedByJLJlang LifetimeManager;

//********************************************************************

class ExportedByJLJlang LifetimeManagerI
	: public ObjectI
{
public:
	virtual ~LifetimeManagerI() throw (Exception); 

	LifetimeManagerI(const String& name) throw (Exception); 

public:
	bool containsKey(const String& key);
	template< typename T > bool put(const String& key, const T& value);
	template< typename T > void get(const String& key, T& value) throw (Exception);
	void remove(const String& key) throw (Exception);

private:
	friend struct ExportedByJLJlang LifetimeManager;
	friend class ExportedByJLJlang AbstractLifetimeImplementationI;
	static LifetimeManager getInstance(const String& name) throw (Exception); 	
	static void removeInstance(const String& name) throw (Exception); 	
	static void addInstance(const String& name, LifetimeManager) throw (Exception); 	

private:
	bool putObject(const String& key, const Object& value);
	Object getObject(const String& key) throw (Exception);
	Object removeObject(const String& key) throw (Exception);

	String name;

};

//********************************************************************

template< typename T >
void LifetimeManagerI::get(const String& key, T& value) throw (Exception)
{
	Object object = getObject(key);
	object->downcast(value);
}

//********************************************************************

template< typename T >
bool LifetimeManagerI::put(const String& key, const T& value) 
{
	Object object = value;
	return putObject(key, object);
}

//********************************************************************

struct ExportedByJLJlang LifetimeManager : StrongReference<LifetimeManagerI>
{    
	StrongReferenceCommonDerivateImplementation(LifetimeManager);	
	static LifetimeManager getInstance(const String& name) throw (Exception);
};

//********************************************************************

NAMESPACE_END(lifetimemanagement)
NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
