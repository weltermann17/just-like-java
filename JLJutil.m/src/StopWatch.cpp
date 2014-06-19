// StopWatch.cpp

//********************************************************************

#ifndef StopWatch_cpp
#define StopWatch_cpp

//********************************************************************

#include <jlj/util/StopWatch.h>
#include <jlj/lang/UTF8String.h>

//********************************************************************

#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

StopWatchI::~StopWatchI() 
{}

//********************************************************************

StopWatchI::StopWatchI()
{
	memset(name, 0, 80);
	s = e = pt::now(true);
}

//********************************************************************

StopWatchI::StopWatchI(const String& name)
{
	strncpy(this->name, constcharptr(name), 80);
	s = e = pt::now(true);
}

//********************************************************************

String StopWatchI::toString() const
{
	const char* DEFAULTFMT = "%Y.%m.%d %H:%M:%S";
	char buf[1024];
	int dummy, smsec, emsec;
	pt::decodetime(s, dummy, dummy, dummy, smsec);
	pt::decodetime(e, dummy, dummy, dummy, emsec);
	sprintf(buf, "<StopWatch delta=\"%.3f\", name=\"%s\", start=\"%s:%03d\", end=\"%s:%03d\" />",
		getSeconds(),
		name,
		pt::pconst(pt::dttostring(s, DEFAULTFMT)), 
		smsec,
		pt::pconst(pt::dttostring(e, DEFAULTFMT)), 
		emsec);
	return String(buf, strlen(buf));
}

//********************************************************************

void StopWatchI::start() 
{
	s = pt::now(true);
}

//********************************************************************

void StopWatchI::stop() 
{
	e = pt::now(true);
}

//********************************************************************

double StopWatchI::getSeconds() const
{
	return pt::msecs(e - s) / 1000.;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
