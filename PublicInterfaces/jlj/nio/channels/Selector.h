// Selector.h

//********************************************************************

#ifndef jlj_nio_channels_Selector_h
#define jlj_nio_channels_Selector_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Throwable.h>
using namespace jlj::lang;
#include <jlj/io/Exceptions.h>
using namespace jlj::io;
#include <jlj/util/Set.h>
#include <jlj/util/Map.h>
#include <jlj/util/List.h>
#include <jlj/util/Collections.h>
using namespace jlj::util;
#include <jlj/core/ptypes/pinet.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;
#include <jlj/util/concurrent/locks/Lock.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

struct ExportedByJLJnio SelectionKey; 
struct ExportedByJLJnio Selector; 
struct ExportedByJLJnio SelectableChannel; 

//********************************************************************

class ExportedByJLJnio SelectorI 
	: public ObjectI
{
protected:
	~SelectorI() throw (IOException);
	friend class TypedReference<SelectorI>;
	SelectorI() throw (IOException); 
	friend struct ExportedByJLJio Selector; 

public:
	void close() throw (IOException);
	bool isOpen() const;
	Set<SelectionKey> keys() const throw (ClosedSelectorException);
	int select() throw (IOException);
	int select(int milliseconds) throw (IOException);
	Set<SelectionKey> selectedKeys() const throw (ClosedSelectorException);
	int selectNow() throw (IOException);
	Selector wakeup() throw (ClosedSelectorException);
	Selector shutdown() throw (ClosedSelectorException);
	void initializeShutdown();

	// helper
	void registerWith(SelectionKey);
	void unregisterFrom(SelectionKey);

private:
	void fillSets(const SelectionKey&, Map<int, SelectionKey>, int op, void*);
	int fillKeys(Map<int, SelectionKey>, int op, void*);

	bool isopen;
	bool isshuttingdown;
	int errorcount;
	SynchronizedList<SelectionKey> selectablekeys;
	SynchronizedList<SelectionKey> selectedkeys;
	pt::ipstream localwriter; // used for wakeup
	pt::ipstream localreader; 
	Logger logger;
	Lock lock;

};

//********************************************************************

struct ExportedByJLJnio Selector : StrongReference<SelectorI> 
{    
	StrongReferenceCommonDerivateImplementation(Selector);

	static Selector open() throw (IOException);
};

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
