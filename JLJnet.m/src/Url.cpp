// Url.cpp

//********************************************************************

#ifndef jlj_net_Url_cpp
#define jlj_net_Url_cpp

//********************************************************************

#include <jlj/io/Base64.h>
#include <jlj/io/InputStream.h>
using namespace jlj::io;

#include <jlj/lang/StringBuffer.h>
#include <jlj/lang/Thread.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

#include <jlj/net/Socket.h>
using namespace jlj::net;

#include <jlj/util/StringAnything.h>
using namespace jlj::util;

#include <string>
#include <sstream>
using namespace std;

//********************************************************************

#include <jlj/net/Url.h>
#include <jlj/net/HttpURLConnection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

URLI::~URLI() throw (IOException)
{}

//********************************************************************

URLI::URLI(const String& url) throw (IOException)
: port(80)
{
	parseURL(url);
}

//********************************************************************

String URLI::getHost()
{
	return hostname;
}

//********************************************************************

String URLI::getUserInfo() 
{
	return userinfo;
}
//********************************************************************

int URLI::getPort()
{
	return port;
}

//********************************************************************

String URLI::getPath()
{
	return path;
}
//********************************************************************

String URLI::getLogin()
{
	return login;
}

//********************************************************************

String URLI::getPassword()
{
	return password;
}

//********************************************************************

URLConnection URLI::openConnection()
{
	return new HttpURLConnectionI(this);
}
//********************************************************************


void URLI::parseURL(const String& urlstring)  throw (IOException)
{
	StringBuffer url(urlstring);
	StringBuffer query;
	StringBuffer fragment;
	StringBuffer hostnameport;
	StringBuffer portbuffer;

	int querystart = url->indexOf(L"?");
	int userinfoend = url->indexOf(L"@");
	int authoritystart = url->indexOf(L"//");

	int pathstart = -1;

	if (authoritystart !=-1) 
	{
		pathstart = url->indexOf(L'/', authoritystart + 2);
		if (pathstart == -1) throw IOException(L"Url::parseURL:: No / for path detected");
		{
			path = url->substring(pathstart, url->length());
		}
	}
	else 
		throw IOException(L"Url::parseURL:: No // Detected");

	if( userinfoend == -1) 
		hostnameport = url->substring(authoritystart + 2, pathstart);
	else
	{ 
		StringBuffer userinfobuffer = url->substring(authoritystart + 2, userinfoend);
		userinfo = userinfobuffer->toString();

		int passwordstart = userinfobuffer->indexOf(L":");
		if (passwordstart == -1) 
			throw IOException(L"Url::parseURL:: Error Syntax User Info URL");
		else 
		{
			login = userinfobuffer->substring(0, passwordstart)->toString();
			password = userinfobuffer->substring(passwordstart + 1, userinfo->length())->toString();
		}

		hostnameport = url->substring(userinfoend+1,pathstart);
	}

	int portstart = hostnameport->indexOf(L":");

	if (portstart == -1) 
		hostname = hostnameport->toString();
	else
	{
		hostname = hostnameport->substring(0, portstart)->toString();
		portbuffer = hostnameport->substring(portstart + 1, hostnameport->length());
		port = atoi(constcharptr(portbuffer));

		if (0 == port) throw IOException(L"Url::parseURL::Error Port value");
	}
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof