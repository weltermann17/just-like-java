// SocketInputStream.h

//********************************************************************

#ifndef jlj_net_SocketInputStream_h
#define jlj_net_SocketInputStream_h

//********************************************************************

#include <jlj/net/Socket.h>
#include <jlj/io/AbstractInputStream.h>
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet SocketInputStreamI
	: public jlj::io::AbstractInputStreamI
{

protected:
    ~SocketInputStreamI() throw (IOException);
	friend class TypedReference<SocketInputStreamI>;

public:
	SocketInputStreamI(pt::ipstream*, int timeout) throw (IOException);

public:
	void close() throw (IOException);
	int available() const throw (IOException);
	int skip(int) throw (IOException);
	int read(char*, int offset, int length) throw (IOException);
	int tell() const throw (IOException);

private:
	int check(pt::estream*) const throw (IOException);
	void check() const throw (IOException);

private:	
	mutable pt::ipstream* stream;
	int timeout;

};

//********************************************************************

StrongReferenceCommonDeclaration(SocketInputStream, ExportedByJLJnet);

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
