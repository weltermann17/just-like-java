// StreamHandler.cpp

//********************************************************************

#ifndef StreamHandler_cpp
#define StreamHandler_cpp

//********************************************************************

#include <jlj/util/logging/StreamHandler.h>
#include <jlj/util/logging/SimpleFormatter.h>
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

StreamHandlerI::~StreamHandlerI()  
{}

//********************************************************************

StreamHandlerI::StreamHandlerI(OutputStream outputstream)
: outputstream(outputstream)
, formatter(new SimpleFormatterI())
, closed(false)
, lock(new ReentrantLockI())
{}

//********************************************************************

void StreamHandlerI::close()
{
	try
	{
		writeTail();
		synchronized(lock) 
		{
			closed = true;
			outputstream->close();
		}
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "%s\n", constcharptr(e->toString()));
	}
}

//********************************************************************

void StreamHandlerI::flush()
{
	try
	{
		synchronized(lock) if (!closed) outputstream->flush();
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "%s\n", constcharptr(e->toString()));
	}
}

//********************************************************************

void StreamHandlerI::writeHead()
{
	try
	{
		synchronized(lock)
		{
			if (closed) return;
			UTF8String h = formatter->getHead(this)->toUTF8String();
			outputstream->write(constcharptr(h), 0, h->length());
		}
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "%s\n", constcharptr(e->toString()));
	}
}

//********************************************************************

void StreamHandlerI::writeTail()
{
	try
	{
		synchronized(lock)
		{
			if (closed) return;
			UTF8String h = formatter->getTail(this)->toUTF8String();
			outputstream->write(constcharptr(h), 0, h->length());
		}
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "%s\n", constcharptr(e->toString()));
	}
}

//********************************************************************

Formatter StreamHandlerI::getFormatter() const
{
	Formatter formatter;
	synchronized(lock) formatter = this->formatter;
	return formatter;
}

//********************************************************************

Level StreamHandlerI::getLevel() const
{
	Level level;
	synchronized(lock) level = this->level;
	return level;
}

//********************************************************************

bool StreamHandlerI::isLoggable(const Level& level) const
{
	synchronized(lock) if (closed) return false;
	return level->intValue() >= levelvalue;
}

//********************************************************************

void StreamHandlerI::publish(LogRecord logrecord)
{
	try
	{
		synchronized(lock) 
		{
			if (closed) return;
			UTF8String l = formatter->format(logrecord)->toUTF8String();
			outputstream->write(constcharptr(l), 0, l->length());
			outputstream->flush();
		}
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "%s\n", constcharptr(e->toString()));
	}
}

//********************************************************************

void StreamHandlerI::setFormatter(const Formatter& formatter)
{
	synchronized(lock)
	{
		writeTail();
		this->formatter = formatter;
		writeHead();
	}
}

//********************************************************************

void StreamHandlerI::setLevel(const Level& level)
{
	synchronized(lock)
	{
		this->level = level;
		levelvalue->set(level->intValue());
	}
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
