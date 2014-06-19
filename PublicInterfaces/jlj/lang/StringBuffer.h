// StringBuffer.h

//********************************************************************

#ifndef StringBuffer_h
#define StringBuffer_h

//********************************************************************

#include <jlj/lang/String.h>
#include <jlj/lang/UTF8String.h>
#include <jlj/core/ptypes/ptypes.h>
#include <iostream>
#include <wchar.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang UTF8StringBuffer;
class ExportedByJLJlang IndexOutOfBoundsException;

//********************************************************************

class ExportedByJLJlang StringBuffer
{
public:
    ~StringBuffer();

	StringBuffer();
    StringBuffer(int initialcapacity);
	StringBuffer(const std::wstring&);					 
    StringBuffer(const wchar_t*);					 
    StringBuffer(const String&);				
    StringBuffer(const UTF8String&);				
	
	explicit StringBuffer(std::wstring&);	
	explicit StringBuffer(String&);		

public:
	StringBuffer& append(bool);
	StringBuffer& append(wchar_t);
	StringBuffer& append(const wchar_t*);
	StringBuffer& append(const wchar_t*, int offset, int length);
	StringBuffer& append(float);
	StringBuffer& append(double);
	StringBuffer& append(int);
	StringBuffer& append(long);
	StringBuffer& append(pt::large);
	StringBuffer& append(const Object&);
	StringBuffer& append(const String&);
	StringBuffer& append(const UTF8String&);
	StringBuffer& append(const StringBuffer&);	
	StringBuffer& append(const UTF8StringBuffer&);	
	int capacity() const;
	wchar_t charAt(int index) const throw (IndexOutOfBoundsException);
	StringBuffer clone() const;
	StringBuffer& deleteFromTo(int start, int end) throw (IndexOutOfBoundsException);
	StringBuffer& deleteCharAt(int index) throw (IndexOutOfBoundsException);
	void ensureCapacity(int capacity);
	bool equals(const StringBuffer&) const;
	void getChars(int start, int end, wchar_t* dest, int deststart) const throw (IndexOutOfBoundsException);
	int hashCode() const;
	int indexOf(const String&) const;
	int indexOf(const String&, int from) const throw (IndexOutOfBoundsException);	
	StringBuffer& insert(int offset, bool) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, wchar_t) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, const wchar_t*) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int index, const wchar_t*, int offset, int length) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, float) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, double) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, int) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, long) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, pt::large) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, const Object&) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, const String&) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, const UTF8String&) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, const StringBuffer&) throw (IndexOutOfBoundsException);
	StringBuffer& insert(int offset, const UTF8StringBuffer&) throw (IndexOutOfBoundsException);
	int lastIndexOf(const String&) const;
	int lastIndexOf(const String&, int from) const throw (IndexOutOfBoundsException);
	int length() const;
	StringBuffer& replace(int start, int end, const String& with) throw (IndexOutOfBoundsException);
	StringBuffer& replace(const String& what, const String& with);
	StringBuffer& replaceAll(const String& what, const String& with);
	StringBuffer& reverse();
	void setCharAt(int index, wchar_t) throw (IndexOutOfBoundsException);
	void setLength(int length);
	String substring(int start) const throw (IndexOutOfBoundsException);
	String substring(int start, int end) const throw (IndexOutOfBoundsException);
	UTF8String toUTF8String() const;
	String toString() const;	
	
	operator const wchar_t*() const; 
	operator const std::wstring&() const;

public:
	StringBuffer(const StringBuffer&);
	StringBuffer& operator=(const StringBuffer&);

	StringBuffer* operator->();
	const StringBuffer* operator->() const;

private:
	void* operator new(size_t);

	mutable UTF8String utf8string;
	mutable std::wstring buffer;
	static const int DEFAULTCAPACITY;
	friend class ExportedByJLJlang String;	
	friend ExportedByJLJlang const char* constcharptr(const StringBuffer&);
	
};

//********************************************************************

ExportedByJLJlang bool operator==(const StringBuffer&, const StringBuffer&);
ExportedByJLJlang bool operator==(const StringBuffer&, const wchar_t*);
ExportedByJLJlang bool operator==(const wchar_t*, const StringBuffer&);
ExportedByJLJlang bool operator!=(const StringBuffer&, const StringBuffer&);
ExportedByJLJlang bool operator!=(const StringBuffer&, const wchar_t*);
ExportedByJLJlang bool operator!=(const wchar_t*, const StringBuffer&);
ExportedByJLJlang bool operator<(const StringBuffer&, const StringBuffer&);
ExportedByJLJlang bool operator>(const StringBuffer&, const StringBuffer&);

//********************************************************************

ExportedByJLJlang const wchar_t* constwchar_tptr(const StringBuffer&);
ExportedByJLJlang const char* constcharptr(const StringBuffer&);

//********************************************************************

ExportedByJLJlang std::ostream& operator<<(std::ostream&, const StringBuffer&);
ExportedByJLJlang std::wostream& operator<<(std::wostream&, const StringBuffer&);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
