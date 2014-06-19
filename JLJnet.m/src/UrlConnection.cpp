// HttpConnector.cpp

//********************************************************************

#ifndef jlj_net_HttpConnector_cpp
#define jlj_net_HttpConnector_cpp

//********************************************************************

#include <jlj/io/UTF8StringInputStream.h>
#include <jlj/io/Base64.h>
#include <jlj/io/InputStream.h>
#include <jlj/io/FileOutputStream.h>
#include <jlj/io/FileInputStream.h>
using namespace jlj::io;

#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/lang/Thread.h>
#include <jlj/lang/String.h>
using namespace jlj::lang;

#include <jlj/net/Socket.h>
using namespace jlj::net;

#include <jlj/util/StringAnything.h>
#include <jlj/util/SessionDirectory.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;

#include <string>
#include <iostream>
#include <sstream>
using namespace std;

//********************************************************************

#include <jlj/net/Url.h>
#include <jlj/net/URLConnection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

URLConnectionI::~URLConnectionI() throw (IOException)
{
	delete contentbuffer;
}

//********************************************************************

URLConnectionI::URLConnectionI(URL url)
: url(url)
, contentbuffer(0)
, contentbuffersize(0)
{
	connect();
	send();
	getDataAnswer();
}

//********************************************************************

URLConnectionI::URLConnectionI()
: contentbuffer(0)
, contentbuffersize(0)
{}

//********************************************************************

void URLConnectionI::connect() throw (IOException)
{
	socket = new SocketI(InetAddress::getByName(url->getHost()), url->getPort(), 5*1000);
	in = socket->getInputStream();
	out = socket->getOutputStream();
}

//********************************************************************

OutputStream URLConnectionI::getOutputStream()
{
	return out;
}

//********************************************************************

InputStream URLConnectionI::getInputStream()
{
	return in;
}

//********************************************************************

void URLConnectionI::send() throw (IOException)
{
	String httprequest;
	if(url->getLogin() == L"") 
	{
		httprequest = createHttpGetRequest(url->getPath());
	}
	else 
	{
		httprequest = createSecureHttpGetRequest(url->getPath(), url->getLogin(), url->getPassword());
	}
	sendHttpRequest(httprequest);

}

//********************************************************************

String URLConnectionI::createHttpEncoding(const String& login, const String& password)
{
	String cred;
	cred = String(login) + L":" + String(password);
	UTF8String encoding(cred);

	Base64 base64;
	base64.encode(encoding, (char*)constcharptr(encoding), encoding->length());
	return L"Authorization: Basic " + String(encoding->toString());
}

//********************************************************************

String URLConnectionI::createHttpGetRequest(const String& parameters) throw (IOException)
{
	String httprequest(L"GET "+ String(parameters) + L" HTTP/1.1\r\nHost: " + String(url->getHost()) + L"\r\nConnection: close\r\n\r\n");
	return httprequest;
}

//********************************************************************

String URLConnectionI::createSecureHttpGetRequest(const String& parameters, const String& login, const String& passwd) throw (IOException)
{
	String httprequest(L"GET "+ String(parameters) + L" HTTP/1.1\r\nHost: " + String(url->getHost()) + L"\r\n" + createHttpEncoding(login,passwd) + "\r\nConnection: close\r\n\r\n");
	return httprequest;
}

//********************************************************************

void URLConnectionI::sendHttpRequest(const String& httprequest) throw (IOException)
{
	out->write(constcharptr(httprequest->toUTF8String()), httprequest->toUTF8String()->length());
	out->flush();
}

//********************************************************************

void URLConnectionI::getDataAnswer()
{
#define BYTESTOREAD \
	input->available() < buffersize - pos ? input->available() : buffersize - pos; 

	const int buffersize = 32 * 1024;
	char* buffer = new char[buffersize]; 
	htmlheader->ensureCapacity(4 * 1024);
	InputStream input = getInputStream();

	int pos = 0;
	int n = 0;
	int endofheader = -1;

	int bytestoread = BYTESTOREAD;
	while (0 < bytestoread && 0 < (n = input->read(buffer, pos, bytestoread)))
	{
		htmlheader->append(buffer, pos, n);
		pos += n;
		if (pos >= buffersize) pos = 0;

		endofheader = htmlheader->indexOf("\r\n\r\n");
		if (0 < endofheader) break;
		
		bytestoread = BYTESTOREAD; 
	}

	if (0 < endofheader) htmlheader = htmlheader->substring(0, endofheader);
	int contentlength = getContentLength(); 
	bool writetomemory = false;
	if (-1 == contentlength)
	{
		writetomemory = true;
		contentlength = 128 * 1024;
	}
	else
	{ 
		writetomemory = contentlength < 128 * 1024;
	}
	bool logreading = false;
	if (logreading) cout << url->getPath() << " " << flush;

	if (endofheader + 4 <= pos)
	{
		int len = pos - (endofheader + 4);
		memcpy(buffer, buffer + endofheader + 4, pos - (endofheader + 4));
		pos = len;
	}
	OutputStream contentoutput;
	if (writetomemory)
	{
		contentbuffersize = contentlength;
		contentbuffer = new char[contentbuffersize];
	}
	else
	{
		SessionDirectory sessiondirectory = SessionDirectory::newInstance();
		contentfilepath = SessionDirectory::newInstance()->getPath() + Uuid::randomUuid()->toString()->toMD5() + L".tmp"; 
		contentoutput = new FileOutputStreamI(contentfilepath);
	}

	int progress = 0;
	int total = 0;
	bytestoread = BYTESTOREAD; 
	while ((0 < bytestoread) && (-1 == contentlength || total < contentlength) 
		&& (0 < (n = input->read(buffer, pos, bytestoread))))
	{
		pos += n;
		if (pos >= buffersize)
		{
			if (writetomemory) 
			{
				memcpy(contentbuffer + total, buffer, pos);
			}
			else
			{
				contentoutput->write(buffer, 0, pos);
			}
			total += pos;
			pos = 0;

			if (logreading)
			{
				double percentage = (total * 10. / contentlength);
				if (progress < (int) percentage)
				{
					cout << "." << flush;
					progress = percentage;
				}
			}
		}
		bytestoread = BYTESTOREAD; 
	}
	if (0 < pos)
	{
#if 0 
		// debug html answer
		for (int k = 0; k < pos; ++k) 
		{
			cout << k << " " << (int) buffer[k] << " " << (32 <= buffer[k] ? buffer[k] : '.') << ":";
		}
		cout << endl;
#endif
		if (writetomemory)
		{
			memcpy(contentbuffer + total, buffer, pos);
		}
		else
		{
			contentoutput->write(buffer, 0, pos);
		}
		total += pos;
	}
	delete [] buffer;
	if (!writetomemory) contentoutput->close();
	if (logreading) cout << ". ok" << endl << flush;
}

//********************************************************************

UTF8String URLConnectionI::getContentType() const
{
	return getValue("Content-Type:");
}

//********************************************************************

int URLConnectionI::getContentLength() const 
{
	UTF8String value = getValue("Content-Length:"); 
	return 0 < value->length() ? atoi(constcharptr(value)) : -1;
}

//********************************************************************

UTF8String URLConnectionI::getTransferEncoding() const 
{
	return getValue("Transfer-Encoding:");
}

//********************************************************************

UTF8String URLConnectionI::getValue(const UTF8String& name) const
{
	int start = htmlheader->indexOf(name);
	int end = htmlheader->indexOf("\r\n", start);
	if (-1 == start || -1 == end) return "";
	return htmlheader->substring(start + name->length() + 1, end);
}

//********************************************************************

UTF8StringBuffer URLConnectionI::getHtmlContent() const
{
	if (0 != contentbuffer && 0 < contentbuffersize)
	{
		UTF8StringBuffer content(contentbuffersize + 1);
		content->append(contentbuffer, 0, contentbuffersize);
		return content;
	}
	else
	{
		try
		{
			InputStream input = new FileInputStreamI(contentfilepath);
			contentbuffersize = input->available();
			contentbuffer = new char[contentbuffersize];
			input->read(contentbuffer, 0, contentbuffersize);
			UTF8StringBuffer content(contentbuffersize + 1);
			content->append(contentbuffer, 0, contentbuffersize);
			return content;
		}
		catch (const Exception&)
		{
			return "";
		}
	}
}

//********************************************************************

String URLConnectionI::getContentWrittenToFilePath() const
{
	if (0 == contentfilepath->length())
	{
		try
		{
			SessionDirectory sessiondirectory = SessionDirectory::newInstance();
			contentfilepath = SessionDirectory::newInstance()->getPath() + Uuid::randomUuid()->toString()->toMD5() + L".tmp"; 
			OutputStream contentoutput = new FileOutputStreamI(contentfilepath);
			contentoutput->write(contentbuffer, 0, contentbuffersize);
		}
		catch (const Exception&)
		{
		}		
	}
	return contentfilepath;
}

//********************************************************************

UTF8StringBuffer URLConnectionI::getHtmlHeader() const
{
	return htmlheader;
}

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif
