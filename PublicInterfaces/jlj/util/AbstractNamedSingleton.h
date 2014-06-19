// AbstractNamedSingleton.h

//********************************************************************

#ifndef jlj_util_AbstractNamedSingleton_h
#define jlj_util_AbstractNamedSingleton_h

//********************************************************************

#include <jlj/util/Collections.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T, typename D >
struct /* ExportedByJLJutil */ AbstractNamedSingleton
{
	static T getInstance() throw (Exception);
	static T newInstance() throw (Exception); // just in case you prefer this name
	static void removeInstance();
	void setInstance(T);
};
	
//********************************************************************

template< typename T, typename D >
T AbstractNamedSingleton<T, D>::getInstance() throw (Exception)
{
	T factory;
	try
	{
		Map<String, Object> namedsingletons;
		LifetimeManager::getInstance(L"jlj")->get(L"namedsingletons", namedsingletons);

		const String clazzname = ClassI::normalizeClassName(typeid(D).name());

		if (namedsingletons->containsKey(clazzname))
		{
			namedsingletons->get(clazzname)->downcast(factory);
		}
		else
		{
			factory = new D();
			namedsingletons->put(clazzname, factory);
		}
	} 
	catch (const Exception& e)
	{
		String prefix = ClassI::normalizeClassName(typeid(T).name());
		Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::SEVERE, prefix + L"::getInstance() failed : " + e->toString());
	}
	return factory;
}

//********************************************************************

template< typename T, typename D >
T AbstractNamedSingleton<T, D>::newInstance() throw (Exception)
{
	return getInstance();
}

//********************************************************************

template< typename T, typename D >
void AbstractNamedSingleton<T, D>::removeInstance()
{
	Map<String, Object> namedsingletons;
	LifetimeManager::getInstance(L"jlj")->get(L"namedsingletons", namedsingletons);

	
	const String clazzname = ClassI::normalizeClassName(typeid(D).name());

	if (namedsingletons->containsKey(clazzname))
	{
		namedsingletons->remove(clazzname);
	}
}

//********************************************************************

template< typename T, typename D >
void AbstractNamedSingleton<T, D>::setInstance(T newinstance)
{
	Map<String, Object> namedsingletons;
	LifetimeManager::getInstance(L"jlj")->get(L"namedsingletons", namedsingletons);

	const String clazzname = ClassI::normalizeClassName(typeid(D).name());

	namedsingletons->put(clazzname, newinstance);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
