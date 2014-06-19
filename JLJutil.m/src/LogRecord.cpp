// LogRecord.cpp

//********************************************************************

#ifndef LogRecord_cpp
#define LogRecord_cpp

//********************************************************************

#include <jlj/util/logging/LogRecord.h>
#include <jlj/util/Date.h>
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

#if defined(_WINDOWS_SOURCE)
#define EOL L"\n"
#else
#define EOL L"\n"
#endif

//********************************************************************

LogRecordI::~LogRecordI() 
{}

//********************************************************************

LogRecordI::LogRecordI(const Level& level, 
					   const String& message,
					   int sequencenumber,
					   double millissince1970,
					   const String& sourceclassname,
					   const String& sourcemethodname,
					   const String& loggername) 
					   : level(level)
					   , message(message)
					   , sequencenumber(sequencenumber)
					   , millissince1970(millissince1970)
					   , sourceclassname(sourceclassname)
					   , sourcemethodname(sourcemethodname)
					   , loggername(loggername)
{}

//********************************************************************

int LogRecordI::hashCode() const
{
	return sequencenumber;
}

//********************************************************************

bool LogRecordI::equals(const Object& object) const
{
	LogRecord b;
	if (object->instanceOf(b))
	{
		return toString()->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

String LogRecordI::toString() const
{
	StringBuffer buf;
	buf->append(sequencenumber);
	buf->append(L" (");
	Date date = new DateI(millissince1970);
	buf->append(date->toString());
	buf->append(L") #");
	buf->append(level->toString());
	buf->append(L"# : ");
	buf->append(message);
	buf->append(EOL);
	return buf->toString();
}

//********************************************************************

Level LogRecordI::getLevel() const
{
	return level;
}

//********************************************************************

String LogRecordI::getLoggerName() const
{
	return loggername;
}

//********************************************************************

String LogRecordI::getMessage() const
{
	return message;
}

//********************************************************************

double LogRecordI::getMillis() const
{
	return millissince1970;
}

//********************************************************************

int LogRecordI::getSequenceNumber() const
{
	return sequencenumber;
}

//********************************************************************

String LogRecordI::getSourceClassName() const
{
	return sourceclassname;
}

//********************************************************************

String LogRecordI::getSourceMethodName() const
{
	return sourcemethodname;
}

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
