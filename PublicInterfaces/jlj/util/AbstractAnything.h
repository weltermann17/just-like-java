// AbstractAnything.h

//********************************************************************

#ifndef jlj_util_AbstractAnything_h
#define jlj_util_AbstractAnything_h

//********************************************************************

#include <jlj/util/Anything.h>
#include <jlj/util/AbstractList.h>
#include <jlj/util/AbstractMap.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

class ExportedByJLJutil AbstractAnythingI 
	: public AnythingI
{
public:
	~AbstractAnythingI();
	AbstractAnythingI();

	bool equals(const Object&) const;
	int hashCode() const;

	String toString() const;
	UTF8String toUTF8String() const;
	UTF8String toBase64(bool zip) const throw (IOException);
	UTF8String toJSON(int) const throw (IOException);

	bool exists() const;
	bool isNull() const;

	operator bool() const;
	operator int() const;
	operator double() const;
	operator String() const;
	operator UTF8String() const;

	/**
	* returns a null pointer except for Stream- and RawAnything
	*/
	InputStream getInputStream() const throw (IOException); 
	
	void serialize(OutputStream) const throw (IOException);
	void write(Writer) const throw (IOException);
	bool writeXml(Writer) const throw (IOException); 

public:
	void clear();
	bool isEmpty() const;
	int size() const;

public:
	Anything get(int index) const throw (IndexOutOfBoundsException);
	Iterator<Anything> iterator();
	Iterator<Anything> iterator() const;
	ListIterator<Anything> listIterator(bool tail);
	ListIterator<Anything> listIterator(bool tail) const;
	Anything removeAt(int index) throw (IndexOutOfBoundsException);

	Set<MapEntry<String, Anything> > entrySet() const;
	Anything get(const String& key) const throw (IndexOutOfBoundsException);
	Set<String> keySet() const;
	bool put(const String&, const Anything&);
	Anything remove(const String& key) throw (IndexOutOfBoundsException);
	Collection<Anything> values() const;

	void add(int, const Anything&) throw (IndexOutOfBoundsException);
	void addAll(int, const Collection<Anything>&) throw (IndexOutOfBoundsException);
	int indexOf(const Anything&) const;
	int lastIndexOf(const Anything&) const;
	Anything set(int, const Anything&) throw (IndexOutOfBoundsException);
	List<Anything> subList(int, int) const throw (IndexOutOfBoundsException, IllegalArgumentException);

	bool containsKey(const String&) const;
	bool containsValue(const Anything&) const;
	void putAll(const Map<String,Anything>&);

	bool add(const Anything&);
	bool addAll(const Collection<Anything>&);
	bool contains(const Anything&) const;
	bool containsAll(const Collection<Anything>&) const;
	bool remove(const Anything&);
	bool removeAll(const Collection<Anything>&);
	bool retainAll(const Collection<Anything>&);

protected:	
	friend struct Anything;
	static void validate(int positiveinteger) throw (IOException);
	
	bool serializeReference(OutputStream&, Map<void*, int>&, void*) const throw (IOException);
	bool writeReference(Writer&, int level, Map<void*, int>&, void*) const throw (IOException);
	bool writeXml__(Writer&, int level, const String&, const Anything&) const throw (IOException);
	void indent(Writer&, int level) const throw (IOException);

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
