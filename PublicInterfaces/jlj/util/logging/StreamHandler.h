// jlj_util_logging_StreamHandler.h

//********************************************************************

#ifndef jlj_util_logging_StreamHandler
#define jlj_util_logging_StreamHandler

//********************************************************************
   
#include <jlj/util/logging/Handler.h>
#include <jlj/util/logging/Formatter.h>
#include <jlj/io/OutputStream.h>
using namespace jlj::io;
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
using namespace jlj::util::concurrent::atomic;
#include <jlj/util/concurrent/locks/Lock.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil StreamHandlerI
	: public HandlerI
{
public:	
	~StreamHandlerI();
	StreamHandlerI(OutputStream);

public:
	void close();
	void flush();
	Formatter getFormatter() const;
	Level getLevel() const;
	bool isLoggable(const Level&) const;
	void publish(LogRecord);
	void setLevel(const Level&);
	void setFormatter(const Formatter&);

protected:
	void writeHead();
	void writeTail();

	Level level;
	Formatter formatter;
	AtomicInteger levelvalue;
	OutputStream outputstream;
	bool closed;
	Lock lock;

};

//********************************************************************

StrongReferenceCommonDeclaration(StreamHandler, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
