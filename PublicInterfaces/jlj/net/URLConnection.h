// UrlConnection.h

//********************************************************************

#ifndef jlj_net_UrlConnection_h
#define jlj_net_UrlConnection_h

//********************************************************************
#include <jlj/net/Url.h>
#include <jlj/net/Socket.h>
using namespace jlj::net;

#include <jlj/util/StreamAnything.h>
#include <jlj/util/StringAnything.h>
using namespace jlj::util;

#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/lang/Thread.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;
//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

class ExportedByJLJnet URLConnectionI
	: public jlj::lang::ObjectI
{
protected:
	~URLConnectionI() throw (IOException);
	friend class TypedReference<URLConnectionI>;
	friend class ExportedByJLJnet URLI;
	URLConnectionI(URL url);
	URLConnectionI();

public:
	OutputStream getOutputStream();
	InputStream getInputStream();

	void connect() throw (IOException);
	UTF8String getContentType() const;
	int getContentLength() const;
	UTF8String getTransferEncoding() const;
	UTF8StringBuffer getHtmlContent() const;
	UTF8StringBuffer getHtmlHeader() const;

protected:
	String getContentWrittenToFilePath() const; 

private:
	UTF8String getValue(const UTF8String& name) const;
	void send() throw (IOException);	
	void sendHttpRequest(const String& httprequest) throw (IOException);	
	String createHttpEncoding(const String& login, const String& passwd);
	String createSecureHttpGetRequest(const String& parameters, const String& login, const String& passwd) throw (IOException);
	String createHttpGetRequest(const String& parameters) throw (IOException);
	void getDataAnswer();

private:
	Socket socket;
	InputStream in;
	OutputStream out;
	URL url;
	UTF8StringBuffer htmlheader;
	mutable char* contentbuffer;
	mutable int contentbuffersize;
	mutable String contentfilepath;

};

//********************************************************************

StrongReferenceCommonDeclaration(URLConnection, ExportedByJLJnet);

//********************************************************************


NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
