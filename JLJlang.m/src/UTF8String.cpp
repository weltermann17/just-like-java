// UTF8String.cpp

//********************************************************************

#ifndef UTF8String_cpp
#define UTF8String_cpp

//********************************************************************

#include <jlj/lang/UTF8String.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/lang/Throwable.h> 
#include <jlj/lang/Class.h> 
#include <jlj/lang/String.h>
#include <jlj/core/ptypes/pasync.h>
#include <jlj/core/ptypes/pstreams.h>

//********************************************************************

#include <wchar.h>
#include <sstream> 
#include <string>
#include <locale>
#include <typeinfo>

//********************************************************************

#if !defined(_WINDOWS_SOURCE)
#include <iconv.h>
#endif

//********************************************************************

#include <jlj/core/platform/LocaleGuard.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

const int MAXSTACK = 8 * 1024;

//********************************************************************
//********************************************************************

UTF8String::~UTF8String() 
{}

//********************************************************************

UTF8String::UTF8String() 
: hash(0)
{}

//********************************************************************

UTF8String::UTF8String(const UTF8String& b) 
: buffer(b.buffer)
, hash(b.hash)
{}

//********************************************************************

UTF8String::UTF8String(UTF8String& b) 
: hash(b.hash)
{
	buffer.swap(b.buffer);
	b.hash = 0;
}

//********************************************************************

UTF8String& UTF8String::operator=(const UTF8String& b) 
{
	if (this != &b)
	{
		buffer = b.buffer;
		hash = b.hash;
	}
	return *this;
}

//********************************************************************

UTF8String* UTF8String::operator->()
{
	return this;
}

//********************************************************************

const UTF8String* UTF8String::operator->() const
{
	return this;
}

//********************************************************************

UTF8String::UTF8String(const std::string& s)      
: buffer(s)
, hash(0)
{}

//********************************************************************

UTF8String::UTF8String(std::string& s) 
: hash(0)
{
	buffer.swap(s);
}

//********************************************************************

UTF8String& UTF8String::operator=(std::string& s) 
{
	buffer.swap(s);
	hash = 0;
	return *this;
}

//********************************************************************

UTF8String::UTF8String(UTF8StringBuffer& s) 
: hash(0)
{
	buffer.swap(s->buffer); 
}

//********************************************************************

UTF8String& UTF8String::operator=(UTF8StringBuffer& b) 
{
	buffer.swap(b->buffer);
	hash = 0;
	return *this;
}

//********************************************************************

UTF8String::UTF8String(const char* s) 
: buffer(s)
, hash(0)
{}

//********************************************************************

UTF8String::UTF8String(const char* s, int offset, int size) 
: buffer(s, offset, size)
, hash(0)
{}

//********************************************************************

void UTF8String::init(const wchar_t* s, int srclen) 
{
#if defined(_WINDOWS_SOURCE)
	int  destlen = WideCharToMultiByte(
		CP_UTF8,
		0,
		s,
		srclen,
		0,
		0,
		0,
		0);

	if (destlen + 1 < MAXSTACK)
	{
		char buf[MAXSTACK];
		destlen = WideCharToMultiByte(
			CP_UTF8,
			0,
			s,
			srclen,
			buf,
			MAXSTACK,
			0,
			0);
		buffer.assign(buf, destlen);
	}
	else
	{
		char* buf = new char[destlen + 1];
		destlen = WideCharToMultiByte(
			CP_UTF8,
			0,
			s,
			srclen,
			buf,
			destlen + 1,
			0,
			0);
		buffer.assign(buf, destlen);
		delete [] buf;
	}
#else
	// do not call iconv_open/close often, it degrades performance strongly

	static iconv_t cd = 0;
	if (0 == cd)
	{
#if defined(PLATEFORME_DS64)
		cd = iconv_open("UTF-8", "UTF-32");
#else
		cd = iconv_open("UTF-8", "UTF-16");
#endif
	}

	static pt::rwlock* rw = new pt::rwlock();

	size_t i = srclen * sizeof(wchar_t);
	size_t o = 6 * srclen;
	const size_t destlen = o;

	if (destlen + 1 < MAXSTACK)
	{
		char buf[MAXSTACK];
		char* outptr = buf;
		char* inptr = (char*) s;
		size_t rc;

		{
			pt::scopewrite scope(*rw);
			rc = iconv(
				cd,
				&inptr,
				&i,
				&outptr,
				&o);
		}

		if (0 == rc) 
		{
			buffer.assign(buf, destlen - o);
		}
	}
	else
	{
		char* buf = new char[o + 1];
		char* outptr = buf;
		char* inptr = (char*) s;
		size_t rc;

		{
			pt::scopewrite scope(*rw);
			rc = iconv(
				cd,
				&inptr,
				&i,
				&outptr,
				&o);
		}

		if (0 == rc) 
		{
			buffer.assign(buf, destlen - o);
		}
		delete [] buf;
	}

	// iconv_close() never called
#endif
}

//********************************************************************

UTF8String::UTF8String(const wchar_t* s, int size) 
: hash(0)
{
	init(s, size);
}

//********************************************************************

UTF8String::UTF8String(const String& s)  
: hash(0)
{
	init(constwchar_tptr(s), s->length());
} 

//********************************************************************

UTF8String::UTF8String(char value) 
: hash(0)
{
	std::ostringstream s;
	s << value;
	buffer = s.str();
}

//********************************************************************

UTF8String::UTF8String(int value) 
: hash(0)
{
	std::ostringstream s;
	s << value;
	buffer = s.str();
}

//********************************************************************

UTF8String::UTF8String(long value) 
:  hash(0)
{
	std::ostringstream s;
	s << value;
	buffer = s.str();
}

//********************************************************************

UTF8String::UTF8String(double value) 
: hash(0)
{
	LocaleGuard localeguard;
	std::ostringstream s;
	s.precision(15);
	s << value;
	buffer = s.str();
}

//********************************************************************

void* UTF8String::operator new(size_t)
{
	return 0;
}

//********************************************************************

int UTF8String::hashCode() const 
{
	if (0 == hash) 
	{
		for (int i = 0; i < buffer.length(); ++i) 
		{ 
			hash = ((hash << 19) - hash) + buffer[i]; 
		} 
	}
	return hash;
}

//********************************************************************

bool UTF8String::equals(const UTF8String& b) const 
{
	return buffer == b->operator const std::string&();
}

//********************************************************************

Class UTF8String::getClass() const 
{
	return new ClassI(typeid(*this).name());
}

//********************************************************************

UTF8String UTF8String::clone() const 
{
	return *this;
}

//********************************************************************

void UTF8String::downcast(UTF8String& other) const throw (ClassCastException)
{
	other = *this;
}

//********************************************************************

int UTF8String::length() const 
{
	return buffer.length();
}

//********************************************************************

int UTF8String::lengthInUTF8Characters() const 
{
	return toString()->length();
}

//********************************************************************

UTF8String::operator const char*() const
{
	return buffer.c_str(); 
}

//********************************************************************

UTF8String::operator const std::string&() const
{
	return buffer; 
}

//********************************************************************

String UTF8String::toString() const
{ 
	return String(buffer.c_str(), buffer.length());
}

//********************************************************************

UTF8String UTF8String::toLowerCase() const
{
	std::string copy(buffer);
	for (std::string::iterator i = copy.begin(); copy.end() != i; ++i)
	{
		*i = std::tolower(*i, std::locale());
	}
	return UTF8String(copy);
}

//********************************************************************

UTF8String UTF8String::toUpperCase() const
{
	std::string copy(buffer);
	for (std::string::iterator i = copy.begin(); copy.end() != i; ++i)
	{
		*i = std::toupper(*i, std::locale());
	}
	return UTF8String(copy);
}

//********************************************************************

UTF8String UTF8String::toMD5() const
{
	pt::outmd5 o;
	o.open();
	o.put(buffer.c_str());
	pt::string result = o.get_digest();
	return UTF8String(result);
}

//********************************************************************

UTF8String UTF8String::trim() const
{
	std::string copy(buffer);
	const std::string whitespace(" \t\n");
    size_t startpos = copy.find_first_not_of(whitespace);
    size_t endpos = copy.find_last_not_of(whitespace);

	if (std::string::npos == startpos || std::string::npos == endpos)
    {
        copy.resize(0);
    }
    else
	{
        copy = copy.substr(startpos, endpos - startpos + 1);
	}
	return UTF8String(copy);
}

//********************************************************************
//********************************************************************

bool operator==(const UTF8String& a, const UTF8String& b)
{
	return a->equals(b); 
}

//********************************************************************

bool operator==(const char* a, const UTF8String& b)
{
	return 0 == strcmp(a, constcharptr(b));
}

//********************************************************************

bool operator==(const UTF8String& a, const char* b)
{
	return 0 == strcmp(constcharptr(a), b);
}

//********************************************************************

bool operator!=(const UTF8String& a, const UTF8String& b)
{
	return !a->equals(b); 
}

//********************************************************************

bool operator!=(const char* a, const UTF8String& b)
{
	return 0 != strcmp(a, constcharptr(b));
}

//********************************************************************

bool operator!=(const UTF8String& a, const char* b)
{
	return 0 != strcmp(constcharptr(a), b);
}

//********************************************************************

bool operator<(const UTF8String& a, const UTF8String& b)
{
	return a->operator const std::string&() < b->operator const std::string&();
}

//********************************************************************

bool operator>(const UTF8String& a, const UTF8String& b)
{
	return a->operator const std::string&() > b->operator const std::string&();
}

//********************************************************************

UTF8String operator+(const UTF8String& a, const UTF8String& b)
{
	std::string result;
	result.reserve(a->length() + b->length() + 1);
	result.append(a->operator const std::string&());
	result.append(b->operator const std::string&());
	return UTF8String(result);
}

//********************************************************************

UTF8String operator+(const char* a, const UTF8String& b)
{
	std::string result;
	int l = strlen(a);
	result.reserve(l + b->length() + 1);
	result.append(a, l);
	result.append(b->operator const std::string&());
	return UTF8String(result);
}

//********************************************************************

UTF8String operator+(const UTF8String& a, const char* b)
{
	std::string result;
	int l = strlen(b);
	result.reserve(a->length() + l + 1);
	result.append(a->operator const std::string&());
	result.append(b, l);
	return UTF8String(result);
}

//********************************************************************

const char* constcharptr(const UTF8String& s)
{
	return s->operator const char*();
}

//********************************************************************

std::ostream& operator<<(std::ostream& os, const UTF8String& s)
{
	return os << constcharptr(s);
}

//********************************************************************

std::wostream& operator<<(std::wostream& os, const UTF8String& s)
{
	return os << s->toString();
}

//********************************************************************
//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
