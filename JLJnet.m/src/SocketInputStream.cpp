// SocketInputstream->cpp

//********************************************************************

#ifndef jlj_net_SocketInputStream_cpp
#define jlj_net_SocketInputStream_cpp

//********************************************************************

#include <jlj/net/SocketInputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

SocketInputStreamI::~SocketInputStreamI() throw (IOException)
{
	close();
}

//********************************************************************

SocketInputStreamI::SocketInputStreamI(pt::ipstream* stream,
									 int timeout) throw (IOException)
									 : stream(stream)
									 , timeout(timeout)
{}

//********************************************************************

void SocketInputStreamI::close() throw (IOException)
{
	stream = 0;
}

//********************************************************************

int SocketInputStreamI::available() const throw (IOException)
{
	check();
	try
	{
		return stream->get_dataavail();
	}
	catch (pt::estream* e)
	{
		return check(e);
	}
}

//********************************************************************

int SocketInputStreamI::skip(int n) throw (IOException)
{
	check();
	try
	{
		return stream->skip(n);
	}
	catch (pt::estream* e)
	{
		return check(e);
	}
}

//********************************************************************

int SocketInputStreamI::read(char* result, int offset, int length) throw (IOException)
{
	check();
	try
	{
		return stream->read(result + offset, length);
	}
	catch (pt::estream* e)
	{
		return check(e);
	}
}

//********************************************************************

int SocketInputStreamI::tell() const throw (IOException)
{
	check();
	try
	{
		return stream->tell(true);
	}
	catch (pt::estream* e)
	{
		return check(e);
	}
}

//********************************************************************

int SocketInputStreamI::check(pt::estream* e) const throw (IOException)
{
	String msg(e->get_message(), pt::length(e->get_message()));
	delete e;
	throw IOException(WITHDETAILS(msg));
	return 0;
}

//********************************************************************

void SocketInputStreamI::check() const throw (IOException)
{
	if (0 == stream) throw IOException(WITHDETAILS("Inputstream is invalid."));
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
