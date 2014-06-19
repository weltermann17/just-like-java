// Consumer.h

//********************************************************************

#ifndef jmstools_jmsprovider_Consumers_h
#define jmstools_jmsprovider_Consumers_h

//********************************************************************

#include <Dispatcher.h>
#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
#include <jlj/util/Map.h>
#include <jlj/util/List.h>
#include <jlj/nio/reactor/Connection.h>

using namespace jlj::lang;
using namespace jlj::util;
using namespace jlj::nio::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class ExportedByJMSProvider ConsumersI
	: public ObjectI
{
protected:
   	~ConsumersI();
    friend class TypedReference<ConsumersI>;  
	
public:
	ConsumersI();

public:
	void addConsumer(const String& consumer, const String& remoteidentifier) throw (Exception);
	void removeConsumer(const String& consumer, const String& remoteidentifier) throw (Exception);
	Connection getConnection(Reactor, const String& consumer) throw (Exception); 
	void cleanup(Reactor, ExecutorService<int>) throw (Exception);
	void cleanStaleConnections(Reactor) throw (Exception);
	int size() const;    

private:
	Map<String, List<String> > consumers;
	int roundrobin;

};

//********************************************************************

StrongReferenceCommonDeclaration(Consumers, ExportedByJMSProvider);

//********************************************************************

NAMESPACE_END(jmstools)


#endif

// eof