// Destination.h

//********************************************************************

#ifndef jlj_jms_Destination_h
#define jlj_jms_Destination_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
#include <jlj/jms/Exceptions.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

class ExportedByJLJjms DestinationI 
	: public ObjectI
{
public:
	virtual ~DestinationI() = 0;
public:
	virtual String toString() const = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Destination, ExportedByJLJjms);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
