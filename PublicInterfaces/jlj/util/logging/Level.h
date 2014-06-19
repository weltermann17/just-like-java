// jlj_util_logging_Level.h

//********************************************************************

#ifndef jlj_util_logging_Level
#define jlj_util_logging_Level

//********************************************************************
   
#include <jlj/lang/Throwable.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(logging)

//********************************************************************

class ExportedByJLJutil LevelI 
	: public ObjectI
{
public:	
	~LevelI();
	LevelI(const String& name, int value);

public:
	bool equals(const Object&) const;
	int hashCode() const;
	String toString() const;

	virtual String getName() const;
	virtual int intValue() const;

private:
	String name;
	int value;
	
};
//********************************************************************

struct ExportedByJLJutil Level : StrongReference<LevelI>
{
	StrongReferenceCommonDerivateImplementation(Level);

	static Level ALL;
	static Level CONFIG;
	static Level FINE;
	static Level FINER;
	static Level FINEST;
	static Level INFO;
	static Level OFF;
	static Level SEVERE;
	static Level WARNING;

	static Level parse(const String& name, int value);
	static void init();
	static Level parse(const String& name);
	static int getValue(const String& name);
};

//********************************************************************

NAMESPACE_END(logging)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
