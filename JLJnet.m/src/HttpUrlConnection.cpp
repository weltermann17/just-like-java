// HttpConnector.cpp

//********************************************************************

#ifndef jlj_net_HttpConnector_cpp
#define jlj_net_HttpConnector_cpp

//********************************************************************

#include <jlj/io/UTF8StringInputStream.h>
#include <jlj/io/Base64.h>
#include <jlj/io/InputStream.h>
using namespace jlj::io;

#include <jlj/lang/UTF8StringBuffer.h>
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

#include <jlj/net/HttpURLConnection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************
HttpURLConnectionI::~HttpURLConnectionI() throw (IOException)
{}

//********************************************************************

HttpURLConnectionI::HttpURLConnectionI(URL url) throw (IOException)
: URLConnectionI(url)
{}

//********************************************************************

HttpURLConnectionI::HttpURLConnectionI()
{}

//********************************************************************

String HttpURLConnectionI::getResponseMessage() throw (IOException)
{
	if (getTransferEncoding() == "chunked")
	{
		return getChunkedContent(getHtmlContent());
	}
	else if (getTransferEncoding() == "transfer-extension") 
	{
		return getHtmlContent()->toString();
	}
	else 
	{
		return getHtmlContent()->toString();
	}
}

//********************************************************************

String HttpURLConnectionI::getResponseFilePath() const
{
	return getContentWrittenToFilePath();
}

//********************************************************************

String HttpURLConnectionI::getChunkedContent(const UTF8StringBuffer& buffer) throw (IOException)
{
	UTF8StringBuffer htmlcontent(128*1024);
	int sizechunked = 0;
	int pos = 0;
	do
	{
		UTF8String nextline = readNextLine(buffer, pos);
		if (0 == nextline->length())
		{
			nextline = readNextLine(buffer, pos);
		}
		sizechunked = getIntFromStringHexa(nextline);
		if (sizechunked > 0)
		{
			htmlcontent->append(buffer->substring(pos, sizechunked + pos));
			pos = pos + sizechunked;
		}
	}
	while (sizechunked > 0);

	return htmlcontent->toString();
}

//********************************************************************

UTF8String HttpURLConnectionI::readNextLine(const UTF8StringBuffer& buffer, 
											int& pos) throw (IOException)
{
	int contentstart = buffer->indexOf(UTF8String("\r\n"), pos);
	if (-1 == contentstart) throw IOException(L"HttpURLConnectionI::ReadNextLine()");
	UTF8String ret = buffer->substring(pos, contentstart);
	pos = contentstart + 2;
	return ret;

}

//********************************************************************

int HttpURLConnectionI::getIntFromStringHexa(const UTF8String& value)
{
	int x; 
	std::istringstream iss((char*) constcharptr(value)); 
	iss >> std::hex >> x;
	return x;
}

//********************************************************************

int HttpURLConnectionI::getResponseCode() throw (IOException)
{
	int responsecodeline = getHtmlHeader()->indexOf(UTF8String(L"HTTP/"));
	int responsecodestart = getHtmlHeader()->indexOf(" ", responsecodeline);
	int responsecodeend = getHtmlHeader()->indexOf(" ", responsecodestart + 1);

	if (-1 == responsecodeline) throw IOException(L"HttpURLConnectionI::getResponseCode() : header = [" + getHtmlHeader()->toString() + L"]");

	responsecodeline = getHtmlHeader()->indexOf(UTF8String(L"HTTP/"), responsecodeend);

	if (-1 != responsecodeline )
	{
		responsecodestart = getHtmlHeader()->indexOf(" ", responsecodeline);
		responsecodeend = getHtmlHeader()->indexOf(" ", responsecodestart + 1);
	}
	if (-1 == responsecodestart || -1 == responsecodeend) throw IOException(L"HttpURLConnectionI::getResponseCode(): Not found.");

	Anything responsecode = new StringAnythingI(getHtmlHeader()->substring(
		responsecodestart + 1, responsecodeend)->toString());
	return responsecode->operator int();
}


//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif
