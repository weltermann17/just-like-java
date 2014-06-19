// TestServiceImplementor.cpp

//********************************************************************

#include <TestServiceImplementor.h>

//********************************************************************

#include <iostream>
using namespace std;

//********************************************************************

#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/Thread.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

ClassLoaderInformationDefinition(jmstools::TestServiceImplementor, 179a2e8699bb1be11da5109b6cebab6e)

//********************************************************************

TestServiceImplementorI::~TestServiceImplementorI()
{}

//********************************************************************

TestServiceImplementorI::TestServiceImplementorI()
{}

//********************************************************************

String TestServiceImplementorI::doService(const String& in) throw (Exception)
{
	logger->log(Level::FINEST, L"TestServiceImplementorI::doService(" + getName() + L")");
	Iterator<MapEntry<String, Anything> > i = getInParameters()->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, Anything> current = i->next();
		getOutParameters()->put(current->value, current->key);
	}
	StringBuffer buf(in);
	return buf->reverse()->toString();
}

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

// eof