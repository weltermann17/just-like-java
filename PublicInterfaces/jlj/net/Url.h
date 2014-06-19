// Url.h

//********************************************************************

#ifndef jlj_net_Url_h
#define jlj_net_Url_h

//********************************************************************
//#include <jlj/net/UrlConnection.h>
#include <jlj/net/Socket.h>
using namespace jlj::net;

#include <jlj/util/StreamAnything.h>
#include <jlj/util/StringAnything.h>
using namespace jlj::util;

#include <jlj/xml/parsers/DocumentBuilderFactory.h>
using namespace jlj::xml;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

#define HTTP 80

//********************************************************************

struct ExportedByJLJnet URLConnection;

//********************************************************************

class ExportedByJLJnet URLI
	: public jlj::lang::ObjectI
{

protected:
    ~URLI() throw (IOException);
    friend class TypedReference<URLI>;

public:
	URLI(const String& url) throw (IOException);
	int getPort();
	String getHost();
	String getUserInfo();
	String getPath();
	String getLogin();
	String getPassword();
	URLConnection openConnection();

private:
	void parseURL(const String& url) throw (IOException);

private:
	int port;
	String hostname;
	String userinfo;
	String path;
	String login;
	String password;
	
};

//********************************************************************

StrongReferenceCommonDeclaration(URL, ExportedByJLJnet);

//********************************************************************


NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
