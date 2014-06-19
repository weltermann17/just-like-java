// AbstractReader.cpp

//********************************************************************

#ifndef AbstractReader_cpp
#define AbstractReader_cpp

//********************************************************************

#include <jlj/io/AbstractReader.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

AbstractReaderI::~AbstractReaderI() throw (IOException)
{
	close();
}

//********************************************************************

AbstractReaderI::AbstractReaderI() 
{}

//********************************************************************

void AbstractReaderI::close() throw (IOException)
{}

//********************************************************************

void AbstractReaderI::mark(int) throw (IOException)
{}

//********************************************************************

bool AbstractReaderI::markSupported() const
{
	return false;
}

//********************************************************************

void AbstractReaderI::reset() throw (IOException)
{}

//********************************************************************

bool AbstractReaderI::ready() const throw (IOException)
{
	return true;
}

//********************************************************************

int AbstractReaderI::skip(int) throw (IOException)
{
	return 0;
}

//********************************************************************

wchar_t AbstractReaderI::read() throw (IOException)
{
	wchar_t buf;
	read((wchar_t*) &buf, 0, 1);
	return buf;
}

//********************************************************************

int AbstractReaderI::read(wchar_t* result, int length) throw (IOException)
{
	return read(result, 0, length);
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
