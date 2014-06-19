// jlj_util_logging_Logger.h

//********************************************************************

#ifndef jlj_util_logging_Logger
#define jlj_util_logging_Logger

//********************************************************************
   
#include <jlj/util/logging/Level.h>
#include <jlj/util/logging/Handler.h>
#include <jlj/util/Collection.h>
#include <jlj/util/Map.h>
using namespace jlj::util;
#include <jlj/util/concurrent/locks/Lock.h>
using namespace jlj::util::concurrent::locks;
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
using namespace jlj::util::concurrent::atomic;

//********************************************************************			   

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil Logger;

//********************************************************************

class ExportedByJLJutil LoggerI 
	: public ObjectI
{
public:	
	~LoggerI();
	LoggerI(const String& name, const Level&);

public:
	virtual void addHandler(Handler);
	virtual Collection<Handler> getHandlers() const;
	virtual String getName() const;
	virtual Logger getParent() const;
	virtual Level getLevel() const;
	virtual bool getFlushAfterLog() const;
	virtual bool isLoggable(const Level&) const;
	virtual void log(const Level&, const String& message);
	virtual void log(LogRecord);
	virtual void removeHandler(Handler);
	virtual void setLevel(const Level&);
	virtual void setParent(const Logger&);
	virtual void setUseParentHandlers(bool);
	virtual void setFlushAfterLog(bool value);

	virtual void all(const String& message);
	virtual void finest(const String& message);
	virtual void finer(const String& message);
	virtual void fine(const String& message);
	virtual void config(const String& message);
	virtual void info(const String& message);
	virtual void warning(const String& message);
	virtual void severe(const String& message);
	virtual void off(const String& message);

	bool equals(const Object&) const;
	int hashCode() const;
	String toString() const;

private:
	String name;
	Level level;
	AtomicInteger levelvalue;
	AtomicInteger sequencenumber;
	AtomicBoolean useparenthandlers;
	AtomicBoolean flushafterlog;
	mutable Collection<Handler> handlers;
	WeakReference<LoggerI> parent;
	Lock lock;

};

//********************************************************************

class ExportedByJLJutil Logger : 
	public StrongReference<LoggerI>
{
public:
	StrongReferenceCommonDerivateImplementation(Logger);

	static const String GLOBAL_LOGGER_NAME;

	static Logger getLogger(const String& name);
	static void removeLogger(const String& name);

};

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
