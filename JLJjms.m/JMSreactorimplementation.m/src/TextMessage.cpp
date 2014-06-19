// TextMessage.cpp

//********************************************************************

#ifndef TextMessage_cpp
#define TextMessage_cpp

//********************************************************************

#include <jmstools/implementation/reactor/TextMessage.h>
#include <jlj/util/NullAnything.h> 
#include <jlj/util/StringAnything.h> 
#include <jlj/util/MapAnything.h> 
#include <jlj/util/IntAnything.h> 
#include <jlj/util/RawAnything.h> 
#include <jlj/util/zip/ZipUtility.h> 
#include <jlj/util/UTF8StringAnything.h> 
#include <jlj/util/logging/Logger.h> 

using namespace jlj::util::logging;
using namespace jlj::util::zip;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

TextMessageI::~TextMessageI()
{}

//********************************************************************

TextMessageI::TextMessageI() 
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"TextMessageI::TextMessageI()");
	setMessageType(Message::TEXT_MESSAGE);
}

//********************************************************************

TextMessageI::TextMessageI(Anything anything)
: AbstractMessageI(anything)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"TextMessageI::TextMessageI(Anything)");
}

//********************************************************************

#define USEZIP

String TextMessageI::getText() const
{
	if (0 == text->length())
	{
		if (!getBody())
		{
			text = String();
		}
		else
		{
#if defined(USEZIP)
			UTF8String body = getBody()->toUTF8String();
			text = 0 == body->length() ? String() : Anything::decodeBase64(body)->toString();
#else
			text = getBody()->toString();
#endif
		}
	}
	return text;
}

//********************************************************************

void TextMessageI::setText(const String& text)
{
#if defined(USEZIP)
	UTF8String utf8(text);
	Anything body = new UTF8StringAnythingI(utf8);
	setBody(new UTF8StringAnythingI(body->toBase64(true)));
#else
	setBody(new StringAnythingI(text));
#endif
	this->text = text;
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
