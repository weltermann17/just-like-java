// SocketOutputStream.cpp

//********************************************************************

#ifndef jlj_net_SocketOutputStream_cpp
#define jlj_net_SocketOutputStream_cpp

//********************************************************************

#include <jlj/net/SocketOutputStream.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

SocketOutputStreamI::~SocketOutputStreamI() throw (IOException) 
{
	close(); 
}

//********************************************************************

SocketOutputStreamI::SocketOutputStreamI(pt::ipstream* stream) throw (IOException)
: stream(stream) 
{}

//********************************************************************

void SocketOutputStreamI::close() throw (IOException) 
{
	if (0 != stream) flush();
	stream = 0;
}

//********************************************************************

void SocketOutputStreamI::flush() throw (IOException) 
{
	check();
	try
	{
		stream->flush();
	}
	catch (pt::estream* e)
	{
		check(e);
	}
}

//********************************************************************

void SocketOutputStreamI::write(const char* value, int offset, int length) throw (IOException) 
{
	try
	{
		stream->write(value + offset, length);
	}
	catch (pt::estream* e)
	{
		check(e);
	}
}

//********************************************************************

int SocketOutputStreamI::tell() const throw (IOException)
{
	try
	{
		return stream->tell(false);
	}
	catch (pt::estream* e)
	{
		check(e);
		return 0;
	}
}

//********************************************************************

int SocketOutputStreamI::check(pt::estream* e) const throw (IOException)
{
	String msg(e->get_message(), pt::length(e->get_message()));
	delete e;
	throw IOException(WITHDETAILS(msg));
	return 0;
}

//********************************************************************

void SocketOutputStreamI::check() const throw (IOException)
{
	if (0 == stream) throw IOException(WITHDETAILS("Outputstream is invalid."));
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
