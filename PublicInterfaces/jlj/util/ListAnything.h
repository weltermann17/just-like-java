// ListAnything.h

//********************************************************************

#ifndef jlj_util_ListAnything_h
#define jlj_util_ListAnything_h

//********************************************************************

#include <jlj/util/AbstractAnything.h>
#include <jlj/util/ArrayList.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil ListAnythingI 
	: public AbstractAnythingI
{
protected:
	~ListAnythingI();
	friend class TypedReference<ListAnythingI>;

public:
	ListAnythingI();
	ListAnythingI(int capacity);
	ListAnythingI(const List<Anything>&);
	ListAnythingI(InputStream&, Map<int, void*>&) throw (IOException);
	template< typename T > ListAnythingI(const List<T>&);

public:
	int type() const;

	Object clone() const;
	bool equals(const Object& object) const;

	void serialize__(OutputStream&, Map<void*, int>&, Anything) const throw (IOException);
	void write__(Writer&, int level, Map<void*, int>&) const throw (IOException);
	void writeJSON__(UTF8StringBuffer&) const throw (IOException);

public:
	bool add(const Anything&);
	void add(int index, const Anything&) throw (IndexOutOfBoundsException);
	void clear();
	Anything get(int index) const throw (IndexOutOfBoundsException);
	Anything get(const String& key) const throw (IndexOutOfBoundsException);
	Iterator<Anything> iterator();
	Iterator<Anything> iterator() const;
	ListIterator<Anything> listIterator(bool tail);
	ListIterator<Anything> listIterator(bool tail) const;
	bool remove(const Anything&);
	Anything remove(const String&) throw (IndexOutOfBoundsException);
	Anything removeAt(int index) throw (IndexOutOfBoundsException);
	int size() const;

public:
	bool addAll(const Collection<Anything>&);
	bool contains(const Anything&) const;
	bool containsAll(const Collection<Anything>&) const;
	bool removeAll(const Collection<Anything>&);
	bool retainAll(const Collection<Anything>&);

private:
	List<Anything> list;

};

//********************************************************************

template< typename T > 
ListAnythingI::ListAnythingI(const List<T>& other)
: list(new ArrayListI<Anything>(other->size()))
{
	Iterator<T> i = other->iterator();
	while (i->hasNext())
	{
		list->add(i->next());
	}
}

//********************************************************************

StrongReferenceCommonDeclaration(ListAnything, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
