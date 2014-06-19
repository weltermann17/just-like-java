// InMemoryOutputStream.cpp

//********************************************************************

#ifndef InMemoryOutputStream_cpp
#define InMemoryOutputStream_cpp

//********************************************************************

#include <jlj/io/InMemoryOutputStream.h>
#include <jlj/lang/StringBuffer.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

InMemoryOutputStreamI::~InMemoryOutputStreamI() throw (IOException)
{}

//********************************************************************

InMemoryOutputStreamI::InMemoryOutputStreamI(char* buffer, 
											 int buffersize)
											 : buffer(buffer)
											 , buffersize(buffersize)
											 , position(0)
{}

//********************************************************************

void InMemoryOutputStreamI::write(const char* value, int offset, int length) throw (IOException)
{
	if (0 != buffer && 0 != value)
	{
		if (position + length <= buffersize)
		{
			memcpy(buffer + position, value + offset, length);
			position += length;
		}
		else 
		{
			if (position < buffersize)
			{
				int delta = buffersize - position;
				memcpy(buffer + position, value + offset, delta);
				position += delta;
			}

			StringBuffer msg;
			msg->append(L"position + length = ")->append(position + length)->append(L" > buffersize = ")->append(buffersize);
			throw IOException(msg->toString());
		}
	}
	else 
	{
		StringBuffer msg;
		msg->append(L"buffer = ")->append((buffer ? L"ok" : L"0"))->append(L", value = ")->append((value ? L"ok" : L"0"));
		throw IOException(msg->toString());
	}
}

//********************************************************************

int InMemoryOutputStreamI::tell() const throw (IOException)
{
	return position;
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
