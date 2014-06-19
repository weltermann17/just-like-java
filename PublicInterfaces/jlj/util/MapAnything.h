// MapAnything.h

//********************************************************************

#ifndef jlj_util_MapAnything_h
#define jlj_util_MapAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil MapAnythingI 
	: public AbstractAnythingI
{
protected:
	~MapAnythingI();
	friend class TypedReference<MapAnythingI>;

public:
	MapAnythingI();
	MapAnythingI(int initialcapacity, double loadfactor);
	MapAnythingI(const Map<String, Anything>&);
	MapAnythingI(InputStream&, Map<int, void*>&) throw (IOException);
	template< typename T > MapAnythingI(const Map<String, T>&);

public:	
	int type() const;
	Object clone() const;
	bool equals(const Object& object) const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

public:
	void clear();
	bool containsKey(const String&) const;
	bool containsValue(const Anything&) const;
	Set<MapEntry<String, Anything> > entrySet() const;
	Anything get(int index) const throw (IndexOutOfBoundsException);
	Anything get(const String& key) const throw (IndexOutOfBoundsException);
	Set<String> keySet() const;
	bool put(const String&, const Anything&);
	void putAll(const Map<String,Anything>&);
	bool remove(const Anything&);
	Anything remove(const String&) throw (IndexOutOfBoundsException);
	int size() const;
	Collection<Anything> values() const;

private:
	Map<String, Anything> map;

};

//********************************************************************

template< typename T > 
MapAnythingI::MapAnythingI(const Map<String, T>& other)
: map(new LinkedHashMapI<String, Anything>(other->size()))
{
	Iterator<MapEntry<String, T> i = other->entrySet->iterator();
	while (i->hasNext())
	{
		MapEntry<String, T> entry = i->next();
		map->put(entry.key, entry.value);
	}
}

//********************************************************************

StrongReferenceCommonDeclaration(MapAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
