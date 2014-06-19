// UTF8StringBuffer.h

//********************************************************************

#ifndef UTF8StringBuffer_h
#define UTF8StringBuffer_h

//********************************************************************

#include <jlj/lang/UTF8String.h>
#include <jlj/lang/String.h>
#include <jlj/core/ptypes/ptypes.h>
#include <iostream>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class ExportedByJLJlang StringBuffer;

//********************************************************************

class ExportedByJLJlang UTF8StringBuffer
{
public:
	~UTF8StringBuffer();

	UTF8StringBuffer();
	UTF8StringBuffer(int initialcapacity);
	UTF8StringBuffer(const char*);					 
	UTF8StringBuffer(const std::string&);					 
	UTF8StringBuffer(const UTF8String&);				
	UTF8StringBuffer(const String&);				

	explicit UTF8StringBuffer(std::string&);	// swap buffers
	explicit UTF8StringBuffer(UTF8String&);		// swap buffers

public:
	UTF8StringBuffer& append(bool);
	UTF8StringBuffer& append(char);
	UTF8StringBuffer& append(const char*);
	UTF8StringBuffer& append(const char*, int offset, int length);
	UTF8StringBuffer& append(float);
	UTF8StringBuffer& append(double);
	UTF8StringBuffer& append(int);
	UTF8StringBuffer& append(long);
	UTF8StringBuffer& append(pt::large);
	UTF8StringBuffer& append(const Object&);
	UTF8StringBuffer& append(const UTF8String&);
	UTF8StringBuffer& append(const String&);
	UTF8StringBuffer& append(const UTF8StringBuffer&);	
	UTF8StringBuffer& append(const StringBuffer&);	
	int capacity() const;
	char charAt(int index) const;
	UTF8StringBuffer clone() const;
	UTF8StringBuffer& deleteFromTo(int start, int end);
	UTF8StringBuffer& deleteCharAt(int index);
	void ensureCapacity(int capacity);
	bool equals(const UTF8StringBuffer&) const;
	void getChars(int start, int end, char* dest, int deststart) const;
	int hashCode() const;
	int indexOf(const UTF8String&) const;
	int indexOf(const UTF8String&, int from) const;	
	UTF8StringBuffer& insert(int offset, bool);
	UTF8StringBuffer& insert(int offset, char);
	UTF8StringBuffer& insert(int offset, const char*);
	UTF8StringBuffer& insert(int index, const char*, int offset, int length);
	UTF8StringBuffer& insert(int offset, float);
	UTF8StringBuffer& insert(int offset, double);
	UTF8StringBuffer& insert(int offset, int);
	UTF8StringBuffer& insert(int offset, long);
	UTF8StringBuffer& insert(int offset, pt::large);
	UTF8StringBuffer& insert(int offset, const Object&);
	UTF8StringBuffer& insert(int offset, const UTF8String&);
	UTF8StringBuffer& insert(int offset, const String&);
	UTF8StringBuffer& insert(int offset, const UTF8StringBuffer&);
	UTF8StringBuffer& insert(int offset, const StringBuffer&);
	int lastIndexOf(const UTF8String&) const;
	int lastIndexOf(const UTF8String&, int from) const;
	int length() const;
	UTF8StringBuffer& replace(int start, int end, const UTF8String& with);
	UTF8StringBuffer& replace(const UTF8String& what, const UTF8String& with);
	UTF8StringBuffer& replaceAll(const UTF8String& what, const UTF8String& with);
	UTF8StringBuffer& reverse();
	void setCharAt(int index, char);
	void setLength(int length);
	UTF8String substring(int start) const;
	UTF8String substring(int start, int end) const;
	UTF8String toUTF8String() const;
	String toString() const;	

	operator const char*() const; 
	operator const std::string&() const;

public:
	UTF8StringBuffer(const UTF8StringBuffer&);
	UTF8StringBuffer& operator=(const UTF8StringBuffer&);

	UTF8StringBuffer* operator->();
	const UTF8StringBuffer* operator->() const;

private:
	void* operator new(size_t);

	mutable std::string buffer;
	static const int DEFAULTCAPACITY;
	friend class ExportedByJLJlang UTF8String;	

};

//********************************************************************

ExportedByJLJlang bool operator==(const UTF8StringBuffer&, const UTF8StringBuffer&);
ExportedByJLJlang bool operator==(const UTF8StringBuffer&, const char*);
ExportedByJLJlang bool operator==(const char*, const UTF8StringBuffer&);
ExportedByJLJlang bool operator!=(const UTF8StringBuffer&, const UTF8StringBuffer&);
ExportedByJLJlang bool operator!=(const UTF8StringBuffer&, const char*);
ExportedByJLJlang bool operator!=(const char*, const UTF8StringBuffer&);
ExportedByJLJlang bool operator<(const UTF8StringBuffer&, const UTF8StringBuffer&);
ExportedByJLJlang bool operator>(const UTF8StringBuffer&, const UTF8StringBuffer&);

//********************************************************************

ExportedByJLJlang const char* constcharptr(const UTF8StringBuffer&);

//********************************************************************

ExportedByJLJlang std::ostream& operator<<(std::ostream&, const UTF8StringBuffer&);
ExportedByJLJlang std::wostream& operator<<(std::wostream&, const UTF8StringBuffer&);

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
