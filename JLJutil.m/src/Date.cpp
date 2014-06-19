// Date.cpp

//********************************************************************

#ifndef Date_cpp
#define Date_cpp

//********************************************************************

#include <jlj/util/Date.h>
#include <jlj/lang/String.h>

//********************************************************************

#include <time.h>
#include <string.h>
#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

DateI::~DateI() 
{}

//********************************************************************

DateI::DateI(double datetime) 
: dt(-1. == datetime ? pt::now(true) : (pt::large) datetime + pt::encodedate(1970, 1, 1))
{}

//********************************************************************

String DateI::convert(pt::datetime d) const
{
	const char* DEFAULTFMT = "%Y-%m-%dT%H:%M:%S";
	char buf[1024];
	int dummy, msec;
	pt::decodetime(d, dummy, dummy, dummy, msec);
	sprintf(buf, "%s.%03d", pt::pconst(pt::dttostring(d, DEFAULTFMT)), msec);
	return String(buf, strlen(buf));
}

//********************************************************************

String DateI::toString() const
{
	return convert(dt);
}

//********************************************************************

String DateI::toLocaleString() const
{
	return toString();
}

//********************************************************************

bool DateI::equals(const Object& object) const 
{
	Date date;
	if (object->instanceOf(date))
	{
		return dt == date->dt;
	}
	else
		return false;
}

//********************************************************************

int DateI::hashCode() const 
{
	return toString()->hashCode();
}

//********************************************************************

Object DateI::clone() const 
{
	return new DateI(getTime());
}

//********************************************************************

int DateI::compareTo(const Object& object) const 
{
	Date date;
	if (object->instanceOf(date))
	{
		return dt == date->dt ? 0 : dt < date->dt ? -1 : 1;
	}
	else
		return -1;
}

//********************************************************************

double DateI::getTime() const
{
	return dt - pt::encodedate(1970, 1, 1);
}

//********************************************************************
//********************************************************************

Date Date::NOW() 
{
	return new DateI(-1.);
}

//********************************************************************
//********************************************************************

bool operator<(const Date& a, const Date& b)
{
	return a->getTime() < b->getTime();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
