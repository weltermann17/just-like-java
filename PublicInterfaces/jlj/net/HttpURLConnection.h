// HttpURLConnection.h

//********************************************************************

#ifndef jlj_net_HttpURLConnection_h
#define jlj_net_HttpURLConnection_h

//********************************************************************
#include <jlj/net/URLConnection.h>
#include <jlj/net/Socket.h>
using namespace jlj::net;

#include <jlj/util/StreamAnything.h>
#include <jlj/util/StringAnything.h>
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

#define HTTP_200_OK 200

#define HTTP_401_UNAUTHORIZED 401
#define HTTP_404_NOTFOUND  404
#define HTTP_406_NOTACCEPTABLE 406

//********************************************************************

class ExportedByJLJnet HttpURLConnectionI
	: public URLConnectionI
{
protected:
	~HttpURLConnectionI() throw (IOException);
	friend class TypedReference<HttpURLConnectionI>;
	friend class ExportedByJLJnet URLI;
	HttpURLConnectionI(URL url) throw (IOException);
	HttpURLConnectionI();

public:
	String getResponseMessage() throw (IOException);
	int getResponseCode() throw (IOException);

	String getResponseFilePath() const;

private:
	String getChunkedContent(const UTF8StringBuffer& buffer) throw (IOException);
	UTF8String readNextLine(const UTF8StringBuffer& buffer, int& pos) throw (IOException);
	int getIntFromStringHexa(const UTF8String& value);

};

//********************************************************************

StrongReferenceCommonDeclaration(HttpURLConnection, ExportedByJLJnet);

//********************************************************************


NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
