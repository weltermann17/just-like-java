// InitialContext.cpp

//********************************************************************

#ifndef InitialContext_cpp
#define InitialContext_cpp

//********************************************************************

#include <jlj/naming/InitialContext.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/LinkedHashSet.h>
#include <jlj/util/TreeMap.h>
#include <jlj/util/TreeSet.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/Uuid.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/io/FileInputStream.h>
#include <jlj/net/HttpURLConnection.h>
using namespace jlj::util;
using namespace jlj::io;

//********************************************************************

#ifdef _WINDOWS_SOURCE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#endif
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

InitialContextI::~InitialContextI() throw (Exception)
{}

//********************************************************************

InitialContextI::InitialContextI() throw (Exception)
: loadedcontexts(new LinkedListI<AbstractContext>())
{}

//********************************************************************

void InitialContextI::loadBindings(const String& configuration) throw (Exception)
{
	loadEnvironmentBindings();

	String goodconfig = 0 < configuration->length() ? loadConfiguration(configuration) : loadConfiguration(L"{}");
	if (verboseOutput()) cout << goodconfig << endl;

	for (int i = 0; i < 2; ++i)
	{
		if (verboseOutput()) cout << "loadBindings() : parse = " << i + 1 << endl;
		
		AbstractContextI::loadBindings(goodconfig);

		ClassLoader classloader = new ClassLoaderI();
		Anything json;
		try 
		{ 
			json = Anything::decodeJSON(goodconfig, true);
		}
		catch (const Exception& e) 
		{ 
			throw Exception(WITHDETAILS(L"Invalid configuration : " + e->toString() + L"\n" + configuration));
		}

		if (0 < json->size()) 
		{
			const String initialcontext(L"java.naming.factory.initial");
			const String initialorder(L"java.naming.factory.initial.order");
			Anything order = json->containsKey(initialorder) ? json->get(initialorder) : new NullAnythingI();
			Collection<String> contextkeys;
			if (order->isNull())
			{
				contextkeys = json->get(initialcontext)->keySet();
			}
			else
			{
				contextkeys = new LinkedListI<String>();
				Iterator<Anything> j = order->iterator();
				while (j->hasNext()) 
				{
					contextkeys->add(j->next()->toString());
				}
			}
			Iterator<String> i = contextkeys->iterator();
			while (i->hasNext())
			{
				String name = L"/" + initialcontext + L"/" + i->next();
				bool createdbyfactory;
				String redirectedname;
				Object object = lookupObjectFactory(name, createdbyfactory, redirectedname);
				if (createdbyfactory)
				{
					AbstractContext context; object->downcast(context);
					loadedcontexts->add(context);

					Anything configuration = new MapAnythingI();
					if (isBound(redirectedname + L"/configuration")) 
					{
						lookupURL(redirectedname + L"/configuration", configuration);
					}
					String configstring = configuration->toJSON()->toString();
					if (verboseOutput()) cout << redirectedname << "/configuration : " << endl << configuration->toString() << endl;
					context->loadBindings(configstring);
				}
			}
		}
	}

	if (verboseOutput()) cout << prettyPrint(78) << endl;
}

//********************************************************************

void InitialContextI::unloadBindings() throw (Exception)
{
	ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
	while (i->hasPrevious())
	{
		AbstractContext context = i->previous();
		context->unloadBindings();
	}

	AbstractContextI::unloadBindings();
}

//********************************************************************

String getStringFromSystemCall(const String& command) 
{
	UTF8String filename = (0 == access("/tmp/ramdisk", 0) ? "/tmp/ramdisk/" : "/tmp/") + Uuid::randomUuid()->toString()->toUTF8String();
	UTF8String systemcall = command + " > " + filename;
	system(constcharptr(systemcall));
	InputStream file = new FileInputStreamI(filename->toString());
	UTF8String result;
	file->read(result, 0);
	file->close();
	remove(constcharptr(filename));
	return result;
}

//********************************************************************

void InitialContextI::loadEnvironmentBindings() throw (Exception)
{
#if defined (_WINDOWS_SOURCE)
	LPTSTR l_EnvStr= GetEnvironmentStringsW();
	LPTSTR l_str = l_EnvStr;
	LPTSTR l_base = l_EnvStr;
	int count = 0;
	while (true)
	{
		if (*l_str == 0) break;
		while (*l_str != 0) l_str++;
		l_str++;
		count++;
	}
	for (int i = 0; i < count; i++)
	{
		StringBuffer s(l_EnvStr);
		int pos = s->indexOf(L"=");
		String n = s->substring(0, pos);
		String value = s->substring(pos + 1);
		if (0 < n->length() && 0 < value->length())
		{
			String name = String(L"/env/") + n->toLowerCase();
			bind(name, new StringAnythingI(value));
		}
		while(*l_EnvStr != '\0') l_EnvStr++;
		l_EnvStr++;
	}
	FreeEnvironmentStringsW(l_base);
#else
	UTF8StringBuffer buffer(getStringFromSystemCall(L"env"));
	int eqlast = 0;
	int nllast = 0;
	while (0 < buffer->length())
	{
		int eqpos;
		try { eqpos = buffer->indexOf("=", eqlast); } catch (...) { eqpos = -1; }
		int nlpos;
		try { nlpos = buffer->indexOf("\n", nllast); } catch (...) { nlpos = buffer->length(); }
		if (-1 == eqpos) break;
		String name = buffer->substring(nllast, eqpos)->toString();
		eqlast = eqpos + 1;
		String value = buffer->substring(eqlast, nlpos)->toString();
		nllast = nlpos + 1;
		eqlast = nllast;
		if (0 < name->length() && 0 < value->length())
		{
			bind(String(L"/env/") + name->toLowerCase(), new StringAnythingI(value));
		}
	}
#endif
}

//********************************************************************

String InitialContextI::loadConfiguration(const String& defaultconfiguration) throw (Exception)
{
	InputStream configfile;
	String configfilename;

	String workingdirectory;
	String exepropertiesfile;
	String exepropertiesfileshort;
#ifdef _WINDOWS_SOURCE 
	TCHAR executable[MAX_PATH];
	GetModuleFileName(0, executable, MAX_PATH);
	exepropertiesfile = String(executable);
	StringBuffer buf(exepropertiesfile);
	int b = 0;
	int e = buf->lastIndexOf(L".");
	exepropertiesfile = buf->substring(b, e) + ".properties";
	b = buf->lastIndexOf(L"\\") + 1;
	exepropertiesfileshort = buf->substring(b, e) + ".properties";
	b = 0;
	e = buf->lastIndexOf(L"\\") + 1;
	workingdirectory = buf->substring(b, e);
#else
	exepropertiesfile = getStringFromSystemCall(L"which `ps eww " + String(getpid()) + L" | grep -v COMMAND | awk '{print $5}'`")->trim() + L".properties";
	StringBuffer buf = exepropertiesfile;
	int b = 0;
	int e = buf->lastIndexOf(L"/") + 1;
	workingdirectory = buf->substring(b, e);
	exepropertiesfileshort = buf->substring(e);
#endif
	// REMOVE!!
	exepropertiesfile = L"";

	bind(L"/env/jlj_context_working_directory", new StringAnythingI(workingdirectory));
	if (isBound(L"/env/jlj_context_file"))
	{
		cout << "JNDI context : JLJ_CONTEXT_FILE is deprecated and ignored, please use JLJ_CONTEXT_URL=file:// instead." << endl;
	}
	if (isBound(L"/env/jlj_context_url"))
	{
		Anything url;
		lookup(L"/env/jlj_context_url", url);
		if (verboseOutput()) cout << "JNDI context : JLJ_CONTEXT_URL found = " << url << endl;
		return loadValueFromURL(url);
	}
	else
	{
		bool propfileexists = 0 == access(constcharptr(exepropertiesfile), 0);
		if (verboseOutput() && !propfileexists) cout << "JNDI context : File not found : " << exepropertiesfile << endl;
		configfilename = propfileexists ? exepropertiesfile : String(L"jndi.properties");
		if (verboseOutput()) cout << "JNDI context : JLJ_CONTEXT_URL not set." << endl;
		if (verboseOutput()) cout << "JNDI context : Trying file = " << configfilename << endl;
	}
	try
	{
		configfile = new FileInputStreamI(configfilename);
	}
	catch (...)
	{
		try
		{
			Anything jlj_home;
			lookup(L"/env/jlj_home", jlj_home);
			if (verboseOutput()) cout << "JNDI context : Trying file = " << jlj_home->toString() + L"/lib/" + configfilename << endl;
			configfile = new FileInputStreamI(jlj_home->toString() + L"/lib/" + configfilename);
		}
		catch (...)
		{
			if (verboseOutput()) cout << "JNDI context : Nothing else found, need to fall back to default configuration. " << endl;
			return defaultconfiguration;
		}
	}
	UTF8String config;
	configfile->read(config, 0);
	return config->toString();
}

//********************************************************************

void InitialContextI::bind(const String& name, Object object) throw (NamingException)
{
	AbstractContextI::bind(name, object);
}

//********************************************************************

bool InitialContextI::isBound(const String& name) const 
{
	if (AbstractContextI::isBound(name)) return true;
	ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
	while (i->hasPrevious())
	{
		AbstractContext context = i->previous();
		if (context->isBound(name)) return true;
	}
	return false;
}

//********************************************************************

Object InitialContextI::lookup(const String& name) const throw (NamingException)
{
	if (!isBound(name)) throw NameNotFoundException(WITHDETAILS(name));
	Object object;
	if (AbstractContextI::isBound(name))
	{
		object = AbstractContextI::lookup(name);
	}
	else
	{
		ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
		while (i->hasPrevious())
		{
			AbstractContext context = i->previous();
			if (context->isBound(name)) 
			{
				object = context->lookup(name);
				break;
			}
		}
	}
	return object;
}

//********************************************************************

Object InitialContextI::lookupClass(const String& name, const String& clazzname) const throw (NamingException)
{
	Object object;
	if (AbstractContextI::isBound(name))
	{
		object = AbstractContextI::lookupClass(name, clazzname);
	}
	else
	{
		ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
		while (i->hasPrevious())
		{
			AbstractContext context = i->previous();
			if (context->isBound(name)) 
			{
				object = context->lookupClass(name, clazzname);
				break;
			}
		}
	}
	if (!object) throw NameNotFoundException(WITHDETAILS(name));
	return object;
}

//********************************************************************

void InitialContextI::unbind(const String& name) throw (NamingException)
{
	if (!isBound(name)) throw NameNotFoundException(WITHDETAILS(name));

	if (AbstractContextI::isBound(name)) 
	{
		AbstractContextI::unbind(name);
	}
	else
	{
		ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
		while (i->hasPrevious())
		{
			AbstractContext context = i->previous();
			if (context->isBound(name)) 
			{
				context->unbind(name);
				break;
			}
		}
	}
}

//********************************************************************

Map<String, Class> InitialContextI::list(const String& contextname) const throw (NamingException)
{
	Map<String, Class> namesclasses = new TreeMapI<String, Class>();
	namesclasses->putAll(AbstractContextI::list(contextname));
	ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
	while (i->hasPrevious())
	{
		AbstractContext context = i->previous();
		namesclasses->putAll(context->list(contextname));
	}
	return namesclasses;
}

//********************************************************************

Map<String, Object> InitialContextI::listBindings(const String& contextname) const throw (NamingException)
{
	Map<String, Object> namesobjects = new TreeMapI<String, Object>();
	namesobjects->putAll(AbstractContextI::listBindings(contextname));
	ListIterator<AbstractContext> i = loadedcontexts->listIterator(true);
	while (i->hasPrevious())
	{
		AbstractContext context = i->previous();
		namesobjects->putAll(context->listBindings(contextname));
	}
	return namesobjects;
}

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

//********************************************************************

// eof
