// SocketOutputStream.h

//********************************************************************

#ifndef jlj_net_SocketOutputStream_h
#define jlj_net_SocketOutputStream_h

//********************************************************************

#include <jlj/net/Socket.h>
#include <jlj/io/AbstractOutputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet SocketOutputStreamI
	: public jlj::io::AbstractOutputStreamI
{

protected:
    ~SocketOutputStreamI() throw (IOException);
	friend class TypedReference<SocketOutputStreamI>;
public:
	SocketOutputStreamI(pt::ipstream*) throw (IOException);

public:
	void close() throw (IOException);
	void flush() throw (IOException);
	void write(const char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

private:
	int check(pt::estream*) const throw (IOException);
	void check() const throw (IOException);
	
private:	
	mutable pt::ipstream* stream;

};

//********************************************************************

StrongReferenceCommonDeclaration(SocketOutputStream, ExportedByJLJnet);

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
