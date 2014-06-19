// ConsoleHandlerI.cpp

//********************************************************************

#ifndef ConsoleHandler_cpp
#define ConsoleHandler_cpp

//********************************************************************

#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/io/FileOutputStream.h>
using namespace jlj::io;
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

ConsoleHandlerI::~ConsoleHandlerI() 
{}

//********************************************************************

ConsoleHandlerI::ConsoleHandlerI()
: StreamHandlerI(new FileOutputStreamI(stdout))
{}

//********************************************************************

void ConsoleHandlerI::close()
{
	writeTail();
	flush(); // do not close() stdout, but flush() at least
}


//********************************************************************

void ConsoleHandlerI::publish(LogRecord logrecord)
{
	StreamHandlerI::publish(logrecord);
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
