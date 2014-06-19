// UTF8StringBuffer.cpp

//********************************************************************

#ifndef UTF8StringBuffer_cpp
#define UTF8StringBuffer_cpp

//********************************************************************

#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/Class.h>

//********************************************************************

#include <jlj/core/hashing/hashfunc.h>
using namespace jlj::core::hashing;
#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;

//********************************************************************

#include <algorithm>
#include <sstream> 
#include <typeinfo>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

const int UTF8StringBuffer::DEFAULTCAPACITY = 2048 / sizeof(char);

//********************************************************************

UTF8StringBuffer::~UTF8StringBuffer() 
{}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer() 
{
	buffer.reserve(DEFAULTCAPACITY);
}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(int initialcapacity) 
{
	buffer.reserve(initialcapacity);
}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(const UTF8StringBuffer& b) 
: buffer(b.buffer)
{}

//********************************************************************

void* UTF8StringBuffer::operator new(size_t)
{
	return 0;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::operator=(const UTF8StringBuffer& b) 
{
	if (this != &b)
	{
		buffer = b.buffer;
	}
	return *this;
}

//********************************************************************

UTF8StringBuffer* UTF8StringBuffer::operator->()
{
	return this;
}

//********************************************************************

const UTF8StringBuffer* UTF8StringBuffer::operator->() const
{
	return this;
}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(const std::string& s) 
: buffer(s)
{}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(const char* s) 
: buffer(s)
{}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(const UTF8String& s) 
: buffer(constcharptr(s), s->length())
{}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(const String& s) 
{
	UTF8String a = s;
	std::string b(constcharptr(a), a->length());
	buffer.swap(b);
}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(UTF8String& s) 
{
	buffer.swap(s->buffer); 
}

//********************************************************************

UTF8StringBuffer::UTF8StringBuffer(std::string& s) 
{
	buffer.swap(s);
}

//********************************************************************

int UTF8StringBuffer::hashCode() const 
{
	return UTF8String(buffer)->hashCode();
}

//********************************************************************

bool UTF8StringBuffer::equals(const UTF8StringBuffer& b) const 
{
	return buffer == b->buffer;
}

//********************************************************************

UTF8StringBuffer UTF8StringBuffer::clone() const 
{
	return buffer;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(bool value)
{
	buffer += (value ? "true" : "false");
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(char value)
{
	buffer += value;
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const char* value)
{
	buffer.append(value);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const char* value, 
										   int offset, 
										   int length)
{
	buffer.append(value + offset, length);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(float value)
{
	std::ostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(double value)
{
	LocaleGuard localeguard;
	std::ostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(int value)
{
	std::ostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(long value)
{
	std::ostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(pt::large value)
{
	std::ostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const Object& value)
{
	UTF8String a(value->toString());
	buffer.append(constcharptr(a));
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const UTF8String& value)
{
	buffer.append(constcharptr(value), value->length());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const String& value)
{
	UTF8String a(value);
	buffer.append(constcharptr(a), a->length());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const UTF8StringBuffer& value)
{
	buffer.append(value->buffer);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::append(const StringBuffer& value)
{
	UTF8String a(value->toUTF8String());
	buffer.append(a->buffer);
	return *this;
}

//********************************************************************


int UTF8StringBuffer::capacity() const
{
	return buffer.capacity();
}

//********************************************************************

char UTF8StringBuffer::charAt(int index) const
{
	return buffer[index];
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::deleteFromTo(int start, int end)
{
	buffer.erase(start, end - start);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::deleteCharAt(int index)
{
	buffer.erase(index, 1);
	return *this;
}

//********************************************************************

void UTF8StringBuffer::ensureCapacity(int capacity)
{
	buffer.reserve(capacity);
}
//********************************************************************

void UTF8StringBuffer::getChars(int start, 
								int end, 
								char* dest, 
								int deststart) const
{
	memcpy(dest + deststart, buffer.c_str() + start, end - start);
	dest[deststart + end - start] = '\0';
}

//********************************************************************

int UTF8StringBuffer::indexOf(const UTF8String& value) const
{
	return buffer.find(constcharptr(value));
}

//********************************************************************

int UTF8StringBuffer::indexOf(const UTF8String& value, int from) const
{
	return buffer.find(constcharptr(value), from);
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, bool value)
{
	buffer.insert(offset, value ? "true" : "false");
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, char value)
{
	std::ostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, const char* value)
{
	buffer.insert(offset, value);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int index, 
										   const char* value, 
										   int offset,  
										   int length)
{
	buffer.insert(index, value + offset, length);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, float value)
{
	std::ostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, double value)
{
	LocaleGuard localeguard;
	std::ostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, int value)
{
	std::ostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, long value)
{
	std::ostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, pt::large value)
{
	std::ostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, const Object& value)
{
	UTF8String a = value->toString();
	buffer.insert(offset, constcharptr(a), a->length());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, const UTF8String& value)
{
	buffer.insert(offset, constcharptr(value), value->length());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, const String& value)
{
	UTF8String a = value;
	buffer.insert(offset, constcharptr(a), a->length());
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, const UTF8StringBuffer& value)
{
	buffer.insert(offset, value->buffer);
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::insert(int offset, const StringBuffer& value)
{
	UTF8String a = value->toUTF8String();
	buffer.insert(offset, a->buffer);
	return *this;
}

//********************************************************************


int UTF8StringBuffer::lastIndexOf(const UTF8String& value) const
{
	return buffer.rfind(constcharptr(value));
}

//********************************************************************

int UTF8StringBuffer::lastIndexOf(const UTF8String& value, int from) const
{
	return buffer.rfind(constcharptr(value), from);
}

//********************************************************************

int UTF8StringBuffer::length() const
{
	return buffer.length();
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::replace(int start, 
											int end, 
											const UTF8String& with)
{
	TRY;
	buffer.replace(start, end - start, constcharptr(with), with->length());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::replace(const UTF8String& what,
											const UTF8String& with)
{
	int start = buffer.find(constcharptr(what));
	if (-1 < start)
	{
		buffer.replace(start, what->length(), constcharptr(with), with->length());
	}
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::replaceAll(const UTF8String& what,
											   const UTF8String& with)
{
	if (0 == buffer.length()) return *this;
	try
	{
		int a = what->length();
		int b = with->length();
		std::string wh(constcharptr(what), a);
		std::string wi(constcharptr(with), b);

		int n = 0;
		int i = 0;
		while (-1 != (i = buffer.find(wh, i)))
		{
			++n; i += a;
		}

		if (1 < n)
		{
			buffer.reserve(buffer.length() + (n * (b - a)) + 1);

			i = 0;
			while (-1 != (i = buffer.find(wh, i)))
			{
				buffer.replace(i, a, wi);
				i += b;
			}
		}
		else
		{
			buffer.replace(buffer.find(wh), a, wi);
		}
	}
	catch (...)
	{
	}
	return *this;
}

//********************************************************************

UTF8StringBuffer& UTF8StringBuffer::reverse()
{
	std::reverse(buffer.begin(), buffer.end());
	return *this;
}

//********************************************************************

void UTF8StringBuffer::setCharAt(int index, char value)
{
	buffer.erase(index);
	buffer.insert(index, &value, 1);
}

//********************************************************************

void UTF8StringBuffer::setLength(int length)
{
	if (0 >= length) 
	{
		buffer.clear();
	}
	else
	{ 
		buffer.resize(length);
	}
}

//********************************************************************

UTF8String UTF8StringBuffer::substring(int start) const
{
	return buffer.substr(start);
}

//********************************************************************

UTF8String UTF8StringBuffer::substring(int start, int end) const
{
	return buffer.substr(start, end - start);
}

//********************************************************************

UTF8String UTF8StringBuffer::toUTF8String() const
{
	return buffer;
}

//********************************************************************

String UTF8StringBuffer::toString() const
{
	return String(buffer.c_str(), buffer.length());
}

//********************************************************************

UTF8StringBuffer::operator const char*() const
{
	return buffer.c_str(); 
}

//********************************************************************

UTF8StringBuffer::operator const std::string&() const
{
	return buffer; 
}

//********************************************************************
//********************************************************************

bool operator==(const UTF8StringBuffer& a, const UTF8StringBuffer& b)
{
	return a->equals(b);
}

//********************************************************************

bool operator==(const char* a, const UTF8StringBuffer& b)
{
	return 0 == strcmp(a, constcharptr(b));
}

//********************************************************************

bool operator==(const UTF8StringBuffer& a, const char* b)
{
	return 0 == strcmp(constcharptr(a), b);
}

//********************************************************************

bool operator!=(const UTF8StringBuffer& a, const UTF8StringBuffer& b)
{
	return !a->equals(b);
}

//********************************************************************

bool operator!=(const char* a, const UTF8StringBuffer& b)
{
	return 0 != strcmp(a, constcharptr(b));
}

//********************************************************************

bool operator!=(const UTF8StringBuffer& a, const char* b)
{
	return 0 != strcmp(constcharptr(a), b);
}

//********************************************************************

bool operator<(const UTF8StringBuffer& a, const UTF8StringBuffer& b)
{
	return a->operator const std::string&() < b->operator const std::string&();
}

//********************************************************************

bool operator>(const UTF8StringBuffer& a, const UTF8StringBuffer& b)
{
	return a->operator const std::string&() > b->operator const std::string&();
}

//********************************************************************

const char* constcharptr(const UTF8StringBuffer& s)
{
	return s->operator const char*();
}

//********************************************************************

std::ostream& operator<<(std::ostream& os, const UTF8StringBuffer& s)
{
	return os << constcharptr(s);
}

//********************************************************************

std::wostream& operator<<(std::wostream& os, const UTF8StringBuffer& s) 
{
	return os << s->toString();
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
