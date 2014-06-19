// StringBuffer.cpp

//********************************************************************

#ifndef StringBuffer_cpp
#define StringBuffer_cpp

//********************************************************************

#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/lang/Class.h>
#include <jlj/lang/Throwable.h>

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

const int StringBuffer::DEFAULTCAPACITY = 2048 / sizeof(wchar_t);

//********************************************************************

StringBuffer::~StringBuffer() 
{}

//********************************************************************

StringBuffer::StringBuffer() 
{
	buffer.reserve(DEFAULTCAPACITY);
}

//********************************************************************

StringBuffer::StringBuffer(int initialcapacity) 
{
	buffer.reserve(initialcapacity);
}

//********************************************************************

StringBuffer::StringBuffer(const StringBuffer& b) 
: buffer(b.buffer)
{}

//********************************************************************

StringBuffer& StringBuffer::operator=(const StringBuffer& b)
{
	if (this != &b)
	{
		buffer = b.buffer;
	}
	return *this;
}

//********************************************************************

void* StringBuffer::operator new(size_t)
{
	return 0;
}

//********************************************************************

StringBuffer* StringBuffer::operator->()
{
	return this;
}

//********************************************************************

const StringBuffer* StringBuffer::operator->() const
{
	return this;
}

//********************************************************************

StringBuffer::StringBuffer(const std::wstring& s) 
: buffer(s)
{}

//********************************************************************

StringBuffer::StringBuffer(const wchar_t* s) 
: buffer(s)
{}

//********************************************************************

StringBuffer::StringBuffer(const String& s) 
: buffer(constwchar_tptr(s), s->length())
{}

//********************************************************************

StringBuffer::StringBuffer(const UTF8String& s) 
{
	String a = s;
	std::wstring b(constwchar_tptr(a), a->length());
	buffer.swap(b);
}

//********************************************************************

StringBuffer::StringBuffer(String& s) 
{
	buffer.swap(s->buffer); 
}

//********************************************************************

StringBuffer::StringBuffer(std::wstring& s) 
{
	buffer.swap(s);
}

//********************************************************************

int StringBuffer::hashCode() const 
{
	return String(buffer)->hashCode();
}

//********************************************************************

bool StringBuffer::equals(const StringBuffer& b) const 
{
	return buffer == b->buffer;
}

//********************************************************************

StringBuffer StringBuffer::clone() const 
{
	return buffer;
}

//********************************************************************

StringBuffer& StringBuffer::append(bool value)
{
	buffer += (value ? L"true" : L"false");
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(wchar_t value)
{
	buffer += value;
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const wchar_t* value)
{
	buffer.append(value);
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const wchar_t* value, 
								   int offset, 
								   int length)
{
	buffer.append(value + offset, length);
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(float value)
{
	std::wostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(double value)
{
	LocaleGuard localeguard;
	std::wostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(int value)
{
	std::wostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(long value)
{
	std::wostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(pt::large value)
{
	std::wostringstream s;
	s << value;
	buffer.append(s.str());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const Object& value)
{
	String a(value->toString());
	buffer.append(constwchar_tptr(a));
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const UTF8String& value)
{
	String a(value);
	buffer.append(constwchar_tptr(a), a->length());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const String& value)
{
	buffer.append(constwchar_tptr(value), value->length());
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const StringBuffer& value)
{
	buffer.append(value->buffer);
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::append(const UTF8StringBuffer& value)
{
	String a(value->toString());
	buffer.append(a->buffer);
	return *this;
}

//********************************************************************


int StringBuffer::capacity() const
{
	return buffer.capacity();
}

//********************************************************************

wchar_t StringBuffer::charAt(int index) const throw (IndexOutOfBoundsException)
{
	if (0 <= index || index < buffer.length())
	{
		return buffer[index];
	}
	else
	{
		throw IndexOutOfBoundsException(WITHDETAILS(L""));
	}
}

//********************************************************************

StringBuffer& StringBuffer::deleteFromTo(int start, int end) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.erase(start, end - start);
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::deleteCharAt(int index) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.erase(index, 1);
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

void StringBuffer::ensureCapacity(int capacity)
{
	buffer.reserve(capacity);
}
//********************************************************************

void StringBuffer::getChars(int start, 
							int end, 
							wchar_t* dest, 
							int deststart) const throw (IndexOutOfBoundsException)
{
	TRY;
	memcpy(dest + deststart, buffer.c_str() + start, (end - start) * sizeof(wchar_t));
	dest[deststart + end - start] = L'\0';
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

int StringBuffer::indexOf(const String& value) const
{
	return buffer.find(constwchar_tptr(value));
}

//********************************************************************

int StringBuffer::indexOf(const String& value, int from) const throw (IndexOutOfBoundsException)
{
	TRY;
	return buffer.find(constwchar_tptr(value), from);
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, bool value) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.insert(offset, value ? L"true" : L"false");
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, wchar_t value) throw (IndexOutOfBoundsException)
{
	TRY;
	std::wostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, const wchar_t* value) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.insert(offset, value);
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int index, 
								   const wchar_t* value, 
								   int offset,  
								   int length) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.insert(index, value + offset, length);
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, float value) throw (IndexOutOfBoundsException)
{
	TRY;
	std::wostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, double value) throw (IndexOutOfBoundsException)
{
	LocaleGuard localeguard;
	TRY;
	std::wostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, int value) throw (IndexOutOfBoundsException)
{
	TRY;
	std::wostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, long value) throw (IndexOutOfBoundsException)
{
	TRY;
	std::wostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, pt::large value) throw (IndexOutOfBoundsException)
{
	TRY;
	std::wostringstream s;
	s << value;
	buffer.insert(offset, s.str());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, const Object& value) throw (IndexOutOfBoundsException)
{
	TRY;
	String a = value->toString();
	buffer.insert(offset, constwchar_tptr(a), a->length());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, const UTF8String& value) throw (IndexOutOfBoundsException)
{
	TRY;
	String a = value;
	buffer.insert(offset, constwchar_tptr(a), a->length());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, const String& value) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.insert(offset, constwchar_tptr(value), value->length());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, const StringBuffer& value) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.insert(offset, value->buffer);
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::insert(int offset, const UTF8StringBuffer& value) throw (IndexOutOfBoundsException)
{
	TRY;
	String a = value->toString(); 
	buffer.insert(offset, a->buffer);
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************


int StringBuffer::lastIndexOf(const String& value) const
{
	return buffer.rfind(constwchar_tptr(value));
}

//********************************************************************

int StringBuffer::lastIndexOf(const String& value, int from) const throw (IndexOutOfBoundsException)
{
	TRY;
	return buffer.rfind(constwchar_tptr(value), from);
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

int StringBuffer::length() const
{
	return buffer.length();
}

//********************************************************************

StringBuffer& StringBuffer::replace(int start, 
									int end, 
									const String& with) throw (IndexOutOfBoundsException)
{
	TRY;
	buffer.replace(start, end - start, constwchar_tptr(with), with->length());
	return *this;
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

StringBuffer& StringBuffer::replace(const String& what,
									const String& with)
{
	int start = buffer.find(constwchar_tptr(what));
	if (-1 < start)
	{
		buffer.replace(start, what->length(), constwchar_tptr(with), with->length());
	}
	return *this;
}

//********************************************************************

StringBuffer& StringBuffer::replaceAll(const String& what,
									   const String& with)
{
	if (0 == buffer.length()) return *this;
	try 
	{
		int a = what->length();
		int b = with->length();
		std::wstring wh(constwchar_tptr(what), a);
		std::wstring wi(constwchar_tptr(with), b);

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

StringBuffer& StringBuffer::reverse()
{
	std::reverse(buffer.begin(), buffer.end());
	return *this;
}

//********************************************************************

void StringBuffer::setCharAt(int index, wchar_t value) throw (IndexOutOfBoundsException)
{
	TRY;	
	buffer.erase(index, 1);
	buffer.insert(index, &value,1);
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

void StringBuffer::setLength(int length)
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

String StringBuffer::substring(int start) const throw (IndexOutOfBoundsException)
{
	TRY;
	return buffer.substr(start);
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

String StringBuffer::substring(int start, int end) const throw (IndexOutOfBoundsException)
{
	TRY;
	return buffer.substr(start, end - start);
	CATCH(IndexOutOfBoundsException); 
}

//********************************************************************

UTF8String StringBuffer::toUTF8String() const
{
	utf8string = UTF8String(buffer.c_str(), buffer.length());
	return utf8string;
}

//********************************************************************

String StringBuffer::toString() const
{
	return buffer;
}

//********************************************************************

StringBuffer::operator const wchar_t*() const
{
	return buffer.c_str(); 
}

//********************************************************************

StringBuffer::operator const std::wstring&() const
{
	return buffer; 
}

//********************************************************************
//********************************************************************

bool operator==(const StringBuffer& a, const StringBuffer& b)
{
	return a->equals(b);
}

//********************************************************************

bool operator==(const wchar_t* a, const StringBuffer& b)
{
	return 0 == wcscmp(a, constwchar_tptr(b));
}

//********************************************************************

bool operator==(const StringBuffer& a, const wchar_t* b)
{
	return 0 == wcscmp(constwchar_tptr(a), b);
}

//********************************************************************

bool operator!=(const StringBuffer& a, const StringBuffer& b)
{
	return !a->equals(b);
}

//********************************************************************

bool operator!=(const wchar_t* a, const StringBuffer& b)
{
	return 0 != wcscmp(a, constwchar_tptr(b));
}

//********************************************************************

bool operator!=(const StringBuffer& a, const wchar_t* b)
{
	return 0 != wcscmp(constwchar_tptr(a), b);
}

//********************************************************************

bool operator<(const StringBuffer& a, const StringBuffer& b)
{
	return a->operator const std::wstring&() < b->operator const std::wstring&();
}

//********************************************************************

bool operator>(const StringBuffer& a, const StringBuffer& b)
{
	return a->operator const std::wstring&() > b->operator const std::wstring&();
}

//********************************************************************

const wchar_t* constwchar_tptr(const StringBuffer& s)
{
	return s->operator const wchar_t*();
}

//********************************************************************

const char* constcharptr(const StringBuffer& s)
{
	s->toUTF8String();
	return s->utf8string->operator const char*();
}

//********************************************************************

std::ostream& operator<<(std::ostream& os, const StringBuffer& s)
{
	return os << s->toUTF8String();
}

//********************************************************************

std::wostream& operator<<(std::wostream& os, const StringBuffer& s) 
{
	return os << constwchar_tptr(s);
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
