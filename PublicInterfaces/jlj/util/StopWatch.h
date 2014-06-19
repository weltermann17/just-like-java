// StopWatch.h

//********************************************************************

#ifndef StopWatch_h
#define StopWatch_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
using namespace jlj::lang;
#include <jlj/core/ptypes/ptime.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil StopWatchI
	: public ObjectI
{
protected:
	~StopWatchI();
	friend class TypedReference<StopWatchI>;

public: 	
	/**
	* all constructors call start() implicitly
	*/
	StopWatchI();	
	StopWatchI(const String& name);	

	String toString() const;

	virtual void start();
	virtual void stop();
	virtual double getSeconds() const;
	
private: 
	pt::datetime s;
	pt::datetime e;
	char name[80];
	
};

//********************************************************************

StrongReferenceCommonDeclaration(StopWatch, ExportedByJLJutil);

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
