// Logger.cpp

//********************************************************************

#ifndef Logger_cpp
#define Logger_cpp

//********************************************************************

#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/lang/Extension.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/Date.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/util/concurrent/locks/ReentrantLock.h>
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::util;
using namespace jlj::util::concurrent::locks;
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

LoggerI::~LoggerI() 
{
	synchronized(lock)
	{
		Iterator<Handler> i = handlers->iterator();
		while (i->hasNext())
		{
			i->next()->close();
		}
		handlers->clear();
	}
}

//********************************************************************

LoggerI::LoggerI(const String& name,
				 const Level& level) 
				 : name(name)
				 , handlers(new LinkedListI<Handler>())
				 , level(level)
				 , levelvalue(level->intValue())
				 , sequencenumber(0)
				 , useparenthandlers(true)
				 , flushafterlog(false)
				 , lock(new ReentrantLockI())
{}

//********************************************************************

int LoggerI::hashCode() const
{
	return name->hashCode();
}

//********************************************************************

bool LoggerI::equals(const Object& object) const
{
	Logger b;
	if (object->instanceOf(b))
	{
		return name->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

String LoggerI::toString() const
{
	return name;
}

//********************************************************************

void LoggerI::addHandler(Handler handler)
{
	Collection<Handler> existinghandlers = getHandlers();
	Iterator<Handler> i = existinghandlers->iterator();
	while (i->hasNext())
	{
		if (handler.get() == i->next().get()) return;
	}

	synchronized(lock)
	{
		handlers->add(handler);
	}
}

//********************************************************************

Collection<Handler> LoggerI::getHandlers() const
{
	Collection<Handler> copy;
	synchronized(lock)
	{
		copy = new LinkedListI<Handler>(handlers);
	}
	return copy;
}

//********************************************************************

String LoggerI::getName() const
{
	return name;
}

//********************************************************************

Logger LoggerI::getParent() const
{
	synchronized(lock) return parent;
	return Logger(); // not reached
}

//********************************************************************

Level LoggerI::getLevel() const
{
	Level level;
	synchronized(lock) level = this->level;
	return level;
}

//********************************************************************

bool LoggerI::getFlushAfterLog() const
{
	return flushafterlog->get();
}

//********************************************************************

bool LoggerI::isLoggable(const Level& level) const
{
	bool levelloggable = (level->intValue() >= levelvalue) || (level->intValue() == Level::ALL->intValue());
	if (levelloggable)
	{
		int handlerssize;
		synchronized(lock) handlerssize = 0 < handlers->size();
		if (0 < handlerssize) 
		{
			return true;
		}
		else
		{
			synchronized(lock) handlers->add(new ConsoleHandlerI());
			return true;
		}
	}
	else
	{
		return false;
	}
}

//********************************************************************

void LoggerI::log(const Level& loglevel, 
				  const String& message)
{
	if (isLoggable(loglevel))
	{
		log(new LogRecordI(
			loglevel,
			message, 
			++sequencenumber,
			-1,
			L"classname", // :TODO:
			L"methodname",
			name));
	}
}

//********************************************************************

void LoggerI::log(LogRecord logrecord) 
{
	if (isLoggable(logrecord->getLevel()))
	{
		synchronized(lock)
		{
			Iterator<Handler> i = handlers->iterator();
			while (i->hasNext())
			{
				Handler handler = i->next();
				handler->publish(logrecord);
				if (getFlushAfterLog()) handler->flush();
			}
		}

		if (useparenthandlers) 
		{
			synchronized(lock) 
			{
				Logger parent = this->parent;
				if (!!parent) parent->log(logrecord);
			}
		}
	}
}

//********************************************************************

void LoggerI::all(const String& message)
{
	log(Level::ALL, message);
}

//********************************************************************

void LoggerI::finest(const String& message)
{
	log(Level::FINEST, message);
}

//********************************************************************

void LoggerI::finer(const String& message)
{
	log(Level::FINER, message);
}

//********************************************************************

void LoggerI::fine(const String& message)
{
	log(Level::FINE, message);
}

//********************************************************************

void LoggerI::config(const String& message)
{
	log(Level::CONFIG, message);
}

//********************************************************************

void LoggerI::info(const String& message)
{
	log(Level::INFO, message);
}

//********************************************************************

void LoggerI::warning(const String& message)
{
	log(Level::WARNING, message);
}

//********************************************************************

void LoggerI::severe(const String& message)
{
	log(Level::SEVERE, message);
}

//********************************************************************

void LoggerI::off(const String& message)
{
	log(Level::OFF, message);
}

//********************************************************************

void LoggerI::removeHandler(Handler handler)
{
	synchronized(lock)
	{
		if (handlers->contains(handler)) handlers->remove(handler);
	}
}

//********************************************************************

void LoggerI::setLevel(const Level& level)
{
	synchronized(lock)
	{
		this->level = level;
		levelvalue->set(level->intValue());
	}
}

//********************************************************************

void LoggerI::setFlushAfterLog(bool value)
{
	flushafterlog->set(value);
}

//********************************************************************

void LoggerI::setParent(const Logger& parent) 
{
	synchronized(lock)
	{
		this->parent = parent;
	}
}

//********************************************************************

void LoggerI::setUseParentHandlers(bool value)
{
	useparenthandlers->set(value);
}

//********************************************************************
//********************************************************************

const String Logger::GLOBAL_LOGGER_NAME = L"GLOBAL_LOGGER";

//********************************************************************

static Logger& stdoutlogger()
{
	static Logger stdoutlogger_ = new LoggerI(L"stdout_logger", Level::parse(L"INVALID", 0));
	static bool once = true;
	if (once) 
	{
		once = false;
		Level::init();
		stdoutlogger_->addHandler(new ConsoleHandlerI());
		stdoutlogger_->setLevel(Level::SEVERE);
		stdoutlogger_->setFlushAfterLog(true);
	}
	return stdoutlogger_;   
}

//********************************************************************

Logger Logger::getLogger(const String& name)
{
	try
	{
		Map<String, Logger> loggers;
		LifetimeManager::getInstance(L"jlj")->get(L"loggers", loggers);
		Logger logger;
		String lname(name->toLowerCase());
		if (loggers->containsKey(lname))
		{
			logger = loggers->get(lname);
		}
		else
		{
			logger = new LoggerI(lname, Level::OFF);
			loggers->put(lname, logger);

			try
			{
				ClassLoader classloader = new ClassLoaderI();
				classloader->loadLibrary(L"JLJnaming");
				Extension extension;
				classloader->loadClass(L"jlj::naming::LoggerContextExtension")->newInstance()->downcast(extension);
				extension->extend(logger);
			}
			catch (const Exception&) {}
		}
		return logger;
	}
	catch (const Exception&)
	{
		return stdoutlogger();
	}
}

//********************************************************************

void Logger::removeLogger(const String& name)
{
	try
	{
		Map<String, Logger> loggers;
		LifetimeManager::getInstance(L"jlj")->get(L"loggers", loggers);
		String lname(name->toLowerCase());
		if (loggers->containsKey(lname)) loggers->remove(lname);
	}
	catch (const Exception&)
	{
	}
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
