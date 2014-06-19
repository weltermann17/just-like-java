// jlj_util_logging_LogRecord.h

//********************************************************************

#ifndef jlj_util_logging_LogRecord
#define jlj_util_logging_LogRecord

//********************************************************************

#include <jlj/util/logging/Level.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil LogRecordI 
	: public ObjectI
{
public:	
	~LogRecordI();
	LogRecordI(
		const Level& level, 
		const String& message,
		int sequencenumber,
		double millissince1970,
		const String& sourceclassname,
		const String& sourcemethodname,
		const String& loggername);

public:
	virtual Level getLevel() const;
	virtual String getLoggerName() const;
	virtual String getMessage() const;
	virtual double getMillis() const;
	virtual int getSequenceNumber() const;
	virtual String getSourceClassName() const;
	virtual String getSourceMethodName() const;

	bool equals(const Object&) const;
	int hashCode() const;
	String toString() const;

private:
	Level level; 
	String message;
	int sequencenumber;
	double millissince1970;
	String sourceclassname;
	String sourcemethodname;
	String loggername;

};

//********************************************************************

StrongReferenceCommonDeclaration(LogRecord, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
