// AbstractRunnableI.cpp

//********************************************************************

#ifndef AbstractRunnableI_cpp
#define AbstractRunnableI_cpp

//********************************************************************

#include <jlj/lang/AbstractRunnable.h>
#include <jlj/lang/Thread.h>

//********************************************************************
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

AbstractRunnableI::~AbstractRunnableI() 
{}

//********************************************************************

AbstractRunnableI::AbstractRunnableI() 
{}

//********************************************************************

void AbstractRunnableI::setCurrentThread(Thread thread) 
{
	this->thread = thread;
}

//********************************************************************

void AbstractRunnableI::resetCurrentThread() 
{
	thread.reset(0);
}

//********************************************************************

Thread AbstractRunnableI::currentThread() 
{
	return thread;
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
