// AbstractContext.cpp

//********************************************************************

#ifndef AbstractContext_cpp
#define AbstractContext_cpp

//********************************************************************

#include <jlj/naming/AbstractContext.h>
#include <jlj/naming/InitialContext.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/LinkedHashSet.h>
#include <jlj/util/TreeMap.h>
#include <jlj/util/TreeSet.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/ListAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/BoolAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/io/FileWriter.h>
#include <jlj/io/FileOutputStream.h>
#include <jlj/io/FileInputStream.h>
#include <jlj/net/HttpURLConnection.h>
#include <jlj/net/URLClassLoader.h>
using namespace jlj::util::logging;
using namespace jlj::net;
using namespace jlj::io;

#ifdef _WINDOWS_SOURCE
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#endif
#include <ios>
#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

AbstractContextI::~AbstractContextI()
{}

//********************************************************************

AbstractContextI::AbstractContextI()
: boundclasses(new LinkedHashMapI<String, Class>())
, boundobjects(new LinkedHashMapI<String, Object>())
, verbose(0 != getenv("JLJ_CONTEXT_VERBOSE"))
{
	static bool once = true;
	if (once) 
	{
		once = false;
		ios_base::sync_with_stdio(true);
		if (!verbose) cout << "JLJ context : For verbose output set JLJ_CONTEXT_VERBOSE=true" << endl;
	}
}

//********************************************************************

bool AbstractContextI::verboseOutput() const 
{
	return verbose;
}

//********************************************************************

void AbstractContextI::bind(const String& name, Object object) throw (NamingException)
{
	boundclasses->put(name, object->getClass());
	boundobjects->put(name, object);
}

//********************************************************************

void AbstractContextI::rebind(const String& name, Object object) throw (NamingException)
{
	if (isBound(name)) unbind(name);
	bind(name, object);
}

//********************************************************************

bool AbstractContextI::isBound(const String& name) const 
{
	return boundobjects->containsKey(name);
}

//********************************************************************

Object AbstractContextI::lookup(const String& name) const throw (NamingException)
{
	return lookupWithSyntaxCheck(name, true);
}

//********************************************************************

Object AbstractContextI::lookupClass(const String& name, const String& clazzname) const throw (NamingException)
{
	if (L"jlj::util::logging::Logger" == clazzname)
	{
		return lookupLogger(name); 
	}
	else
	{
		return lookup(name);
	}
}

//********************************************************************

void AbstractContextI::unbind(const String& name) throw (NamingException)
{
	if (!boundclasses->containsKey(name)) throw NameNotFoundException(WITHDETAILS(name));
	boundclasses->remove(name);
	boundobjects->remove(name);
}

//********************************************************************

inline bool startsWith(const String& what, const String& with)
{
	StringBuffer buffer;
	buffer->append(what);
	return 0 == buffer->indexOf(with);
}

//********************************************************************

inline bool endsWith(const String& what, const String& with)
{
	StringBuffer buffer;
	buffer->append(what);
	return what->length() - with->length() == buffer->lastIndexOf(with);
}

//********************************************************************

Map<String, Class> AbstractContextI::list(const String& contextname) const throw (NamingException)
{
	Map<String, Class> namesclasses = new LinkedHashMapI<String, Class>();
	Iterator<MapEntry<String, Class> > i = boundclasses->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, Class> current = i->next();
		if (startsWith(current->key, contextname))
		{
			namesclasses->put(current->key, current->value);
		}
	}
	return namesclasses;
}

//********************************************************************

Map<String, Object> AbstractContextI::listBindings(const String& contextname) const throw (NamingException)
{
	Map<String, Object> namesobjects = new LinkedHashMapI<String, Object>();
	Iterator<MapEntry<String, Object> > i = boundobjects->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, Object> current = i->next();
		if (startsWith(current->key, contextname))
		{
			namesobjects->put(current->key, current->value);
		}
	}
	return namesobjects;
}

//********************************************************************

void AbstractContextI::unloadBindings() throw (Exception)
{
	boundclasses->clear();
	boundobjects->clear();
}

//********************************************************************

String AbstractContextI::toString() const
{
	Anything buf = new MapAnythingI(new TreeMapI<String, Anything>());
	Iterator<MapEntry<String, Object> > i = listBindings(L"/")->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, Object> current = i->next();
		buf->put(current->key, current->value->toString());
	}
	return buf->toJSON();
}

//********************************************************************

String AbstractContextI::prettyPrint(int maxvisiblevaluelength) const
{
	StringBuffer buf(128*1024);
	buf->append(L"{");
	Iterator<MapEntry<String, Object> > i = listBindings(L"/")->entrySet()->iterator();
	int c = 0;
	while (i->hasNext())
	{
		MapEntry<String, Object> current = i->next();
		if (0 < c++) buf->append(L", ");
		buf->append(L"\n  \"");
		buf->append(current->key);
		buf->append(L"\" : ");
		StringBuffer value(current->value->toString());		 
		int len = value->length();
		if (-1 < maxvisiblevaluelength && maxvisiblevaluelength < len) 
		{
			value = value->substring(0, maxvisiblevaluelength);
			value->append(L" ... (");
			value->append(len - maxvisiblevaluelength);
			value->append(L")");
		}
		StringAnything stringany;
		if (current->value->instanceOf(stringany))
		{
			buf->append(L"\"");
			buf->append(value->toString());
			buf->append(L"\"");
		}
		else
		{
			Anything any;
			if (current->value->instanceOf(any))
			{
				buf->append(current->value->toString());
			}
			else
			{
				buf->append(L"\"");
				buf->append(value->toString());
				buf->append(L"\"");
			}
		}
	}
	buf->append(L"\n}");
	return buf->toString();
}

//********************************************************************

void AbstractContextI::loadBindings(const String& configuration) throw (Exception)
{
	// replace variables in the config string: eg. "mypath" : "${env/path}" => "mypath" : "c:\\windows ..."

	Anything config;
	try { config = Anything::decodeJSON(configuration, true); }
	catch (const Exception& e) { throw Exception(WITHDETAILS(L"Invalid configuration : " + e->toString() + L"\n" + configuration)); }
	StringBuffer path;
	createBindings(config, path, 1, false);

	StringBuffer buf = configuration;
	while (-1 < buf->indexOf(L"${"))
	{
		InitialContext initialcontext = InitialContext::newInstance();
		int bpos = 0;
		int epos = 0;
		while (bpos < buf->length())
		{
			bpos = buf->indexOf(L"${", bpos);
			if (-1 == bpos) break;
			epos = buf->indexOf(L"}", bpos);
			String variablename = buf->substring(bpos + 2, epos);
			StringAnything variablevalue ;
			initialcontext->lookup(L"/" + variablename, variablevalue);
			StringBuffer value = variablevalue->toString(); 
			if (-1 < value->indexOf(L"${" + variablename + L"}") )
				throw Exception(WITHDETAILS(L"Recursive reference found : ${" + variablename + L"}"));
			buf->replace(bpos, epos + 1, value->toString());
			bpos = epos + 1;
		}

		Anything config;
		try { config = Anything::decodeJSON(buf->toString(), true); }
		catch (const Exception& e) { throw Exception(WITHDETAILS(L"Invalid configuration (2nd pass) : " + e->toString() + L"\n" + buf->toString())); }
		StringBuffer path;
		createBindings(config, path, 1, true);
	}
}

//********************************************************************

#define shortenPath(path, level) \
{ \
	int pos = -1; \
	for (int i = 0; i < level; ++i) \
{ \
	pos = path->indexOf(L"/", pos + 1); \
} \
	path = path->substring(0, pos); \
}

//********************************************************************

void AbstractContextI::createBindings(Anything config, 
									  StringBuffer path,
									  int level,
									  bool rebind) throw (Exception)
{
	static int previouslevel = 0;
	Iterator<MapEntry<String, Anything> > i = config->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, Anything> current = i->next();
		if (previouslevel > level) shortenPath(path, level);
		path->append(L"/");
		path->append(current->key);
		if (rebind && isBound(path->toString())) unbind(path->toString());
		bind(path->toString(), current->value);
		createBindings(current->value, path, level + 1, rebind);
	}
	previouslevel = level;
}

//********************************************************************

Object AbstractContextI::lookupObjectFactory(const String& name,
											 bool& createdbyfactory,
											 String& redirectedname) const throw (NamingException)
{
	try
	{
		redirectedname = name;
		createdbyfactory = false;
		Object object = lookupWithSyntaxCheck(name, false);
		StringAnything referencelink;
		if (object->instanceOf(referencelink))
		{
			if (isBound(referencelink->toString()))
			{
				object = lookupURL(referencelink->toString());
				redirectedname = referencelink->toString();
			}
		}
		MapAnything objectfactory;
		if (object->instanceOf(objectfactory))
		{
			if (objectfactory->containsKey(L"class") && objectfactory->containsKey(L"library"))
			{
				bool load = true;
				if (objectfactory->containsKey(L"load")) load = objectfactory->get(L"load");
				if (load)
				{
					ClassLoader classloader;
					String clazzname = objectfactory->get(L"class");

					Anything library = objectfactory->get(L"library");
					if (Anything::ANY_STRING == library->type())
					{
						StringBuffer buf = library->toString();
						if (0 == buf->indexOf(L"http://"))
						{
							classloader = new URLClassLoaderI();
						}
						else
						{
							classloader = new ClassLoaderI();
						}
						classloader->loadLibrary(library);
					}
					else
					{
						if (Anything::ANY_LIST == library->type())
						{
							classloader = new URLClassLoaderI();
							Iterator<Anything> i = library->iterator();
							while (i->hasNext())
							{
								String library = i->next();
								classloader->loadLibrary(library);
							}
						}
					}
					object = classloader->loadClass(clazzname)->newInstance();
					const_cast<AbstractContextI*>(this)->rebind(name, object);
					createdbyfactory = true;
					const_cast<AbstractContextI*>(this)->bind(
						name + L"/" + name->toMD5(), new StringAnythingI(redirectedname));
				}
			}
		}
		else
		{
			createdbyfactory = isBound(name + L"/" + name->toMD5());
			if (createdbyfactory) redirectedname = lookup(name + L"/" + name->toMD5())->toString(); 
		}
		return object;
	}
	catch (const NamingException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		throw NamingException(WITHDETAILS(e->toString()));;
	}
}

//********************************************************************

Object AbstractContextI::lookupLogger(const String& name) const throw (NamingException)
{
	Object object = lookup(name);
	Logger logger;
	MapAnything loggerconfiguration;
	if (object->instanceOf(logger) || object->instanceOf(loggerconfiguration))
	{
		if (object->instanceOf(loggerconfiguration))
		{
			Anything loggername;
			lookup(name + L"/name", loggername);
			logger = Logger::getLogger(loggername);
		}
		object = logger;
		if (isBound(name + L"/level"))
		{
			Anything level;
			lookup(name + L"/level", level);
			logger->setLevel(Level::parse(level));
		}
		if (isBound(name + L"/flush"))
		{
			Anything flush;
			lookup(name + L"/flush", flush);
			logger->setFlushAfterLog(flush);
		}
		if (isBound(name + L"/handlers"))
		{
			Iterator<Handler> h = logger->getHandlers()->iterator();
			while (h->hasNext()) logger->removeHandler(h->next());
			Anything handlers;
			lookup(name + L"/handlers", handlers);
			Iterator<Anything> i = handlers->iterator();
			while (i->hasNext())
			{
				String filename = i->next();
				if (L"CONSOLE" == filename->toUpperCase())
				{
					logger->addHandler(ConsoleHandler::getInstance());
				}
				else
				{
					try 
					{
						logger->addHandler(new StreamHandlerI(new jlj::io::FileOutputStreamI(filename)));
					}
					catch (const Exception& e)
					{
						throw NamingException(WITHDETAILS(e->toString()));
					}
				}
			}
		}
		else
		{
			if (0 == logger->getHandlers()->size())	logger->addHandler(ConsoleHandler::getInstance());
		}
		const_cast<AbstractContextI*>(this)->rebind(name, object);
	}
	return object;
}

//********************************************************************

Object AbstractContextI::lookupWithSyntaxCheck(const String& name, bool lookupalias) const throw (NamingException)
{
	if (!boundclasses->containsKey(name)) throw NameNotFoundException(WITHDETAILS(name));	
	Object object = boundobjects->get(name);

	// handle switchcase

	MapAnything switchcase;
	if (object->instanceOf(switchcase))
	{
		if (switchcase->containsKey(L"switch"))
		{
			String casevalue = switchcase->get(L"switch");
			if (switchcase->containsKey(casevalue))
			{
				object = switchcase->get(casevalue);
			}
			else
			{
				if (switchcase->containsKey(L"default"))
				{
					object = switchcase->get(L"default");
				}
			}
		}
	}

	// handle reference links, alias lookup

	if (lookupalias) 
	{
		StringAnything referencelink;
		if (object->instanceOf(referencelink))
		{
			if (isBound(referencelink->toString()))
			{
				object = lookup(referencelink->toString());
			}
		}
	}
	// more special handling

	return object;
}

//********************************************************************

void AbstractContextI::copyBinding(const String& from, const String& to) throw (NamingException)
{
	if (isBound(from))
	{
		rebind(to, lookup(from));
	}
}

//********************************************************************

Object AbstractContextI::lookupURL(const String& name) const throw (NamingException)
{
	Object object = lookup(name);
	StringAnything stringanything;
	if (object->instanceOf(stringanything))
	{
		try
		{
			object = Anything::decodeJSON(
				loadValueFromURL(stringanything->toString()), true);
		}
		catch (const Exception& e)
		{
			throw NamingException(WITHDETAILS(e->toString()));
		}
	}
	return object;
}

//********************************************************************

String AbstractContextI::loadValueFromURL(const String& urlstring) const throw (Exception)
{
	String result;
	StringBuffer buf;
	buf->append(urlstring);
	if (0 == buf->indexOf(L"http://"))
	{
		URL url = new URLI(urlstring);
		HttpURLConnection httpurlconnection;
		url->openConnection()->downcast(httpurlconnection);
		if (verboseOutput()) cout << "JNDI context : Get response from url : " << urlstring << endl;
		int rc = httpurlconnection->getResponseCode();
		UTF8StringBuffer cbuf;
#if defined (_WINDOWS_SOURCE)
		cbuf->append(getenv("TEMP"));
		cbuf->append("\\");
		cbuf->append(getenv("USERNAME"));
		cbuf->append(".");
#else
		cbuf->append(getenv("USER_HOME"));
		cbuf->append("/.");
#endif
		cbuf->append(urlstring->toMD5());
		cbuf->append(".properties");
		String cachefilepath = cbuf->toString()->toLowerCase();
		if (200 != rc) 
		{
			if (verboseOutput()) cout << "JNDI context : HTTP status = " << rc << endl;
			if (verboseOutput()) cout << "JNDI context : Trying to load from cached file : " << cachefilepath << endl;
			InputStream file = new FileInputStreamI(cachefilepath);
			UTF8String content;
			file->read(content, 0);
			file->close();
			result = content->toString();
		}
		else
		{
			String response = httpurlconnection->getResponseMessage();
			Writer file = new FileWriterI(cachefilepath);
			file->write(response);
			file->close();
			if (verboseOutput()) cout << "JNDI context : Caching to file : " << cachefilepath << endl;
			result = response;
		}
	}
	else if (0 == buf->indexOf(L"file://"))
	{
		result = loadValueFromFile(urlstring);
	}
	else
	{
		throw NamingException(WITHDETAILS(L"Unsupported URI format: " + urlstring));
	}
	StringBuffer b = result;
	while (-1 < b->indexOf(L"${"))
	{
		InitialContext initialcontext = InitialContext::newInstance();
		int bpos = 0;
		int epos = 0;
		while (bpos < b->length())
		{
			bpos = b->indexOf(L"${", bpos);
			if (-1 == bpos) break;
			epos = b->indexOf(L"}", bpos);
			String variablename = b->substring(bpos + 2, epos);
			StringAnything variablevalue ;
			initialcontext->lookup(L"/" + variablename, variablevalue);
			StringBuffer value = variablevalue->toString(); 
			if (-1 < value->indexOf(L"${" + variablename + L"}") )
				throw Exception(WITHDETAILS(L"Recursive reference found : ${" + variablename + L"}"));
			b->replace(bpos, epos + 1, value->toString());
			bpos = epos + 1;
		}
	}
	return b->toString();
}

//********************************************************************

String AbstractContextI::loadValueFromFile(const String& fileurl) const throw (Exception)
{
	StringBuffer buf;
	buf->append(fileurl);
	if (0 != buf->indexOf(L"file://")) throw NamingException(WITHDETAILS(L"Expected url starting wtih 'file://' but found : " + fileurl));
	
	bool mustprependworkingdirectory = L'/' != buf->charAt(7);
	
	StringBuffer filename;
	if (L':' == buf->charAt(9)) // windows filepath
	{
		filename->append(buf->substring(8));
		filename = filename->replaceAll(L"/", L"\\");
	}
	else
	{
		filename->append(buf->substring(7));
	}


	String fullpath;
	if (mustprependworkingdirectory)
	{
		Anything value;
		lookup(L"/env/jlj_context_working_directory", value);
		String workingdirectory = value;
		fullpath = workingdirectory + filename;
	}
	else
	{
		fullpath = filename->toString();
	}
	if (verboseOutput()) cout << "JNDI context : Reading from file : " << fullpath << endl;
	InputStream file = new FileInputStreamI(fullpath);
	UTF8String content;
	file->read(content, 0);
	file->close();
	return content->toString();
}

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

//********************************************************************

// eof
