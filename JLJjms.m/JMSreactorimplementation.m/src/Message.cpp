// Message.cpp

//********************************************************************

#ifndef jmstools_implementation_reactor_Message_cpp
#define jmstools_implementation_reactor_Message_cpp

//********************************************************************

#include <jmstools/implementation/reactor/Message.h>
#include <jmstools/implementation/reactor/Queue.h>
#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jmstools/implementation/reactor/TextMessage.h>

#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/UTF8StringAnything.h>
#include <jlj/util/BoolAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/DoubleAnything.h>
#include <jlj/util/ArrayList.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/Date.h>

using jlj::util::Uuid;
using jlj::util::Date;
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

const String MessageI::HEADER_TAG = L"H";

const String MessageI::HEADER_TAG_DESTINATION = L"HD";
const String MessageI::HEADER_TAG_DESTINATION_NAME = L"HDN";
const String MessageI::HEADER_TAG_DESTINATION_TYPE = L"HDT";
const String MessageI::HEADER_TAG_DESTINATION_REMOTE_ID = L"HDR";
const String MessageI::HEADER_TAG_DESTINATION_JSONENCODING = L"HDJ";

const String MessageI::HEADER_TAG_REPLYTO = L"HR";
const String MessageI::HEADER_TAG_REPLYTO_NAME = L"HRN";
const String MessageI::HEADER_TAG_REPLYTO_TYPE = L"HRT";
const String MessageI::HEADER_TAG_REPLYTO_REMOTE_ID = L"HRR";
const String MessageI::HEADER_TAG_REPLYTO_JSONENCODING = L"HRJ";

const String MessageI::HEADER_TAG_MESSAGE_ID = L"HMI";
const String MessageI::HEADER_TAG_TIMESTAMP = L"HTM";
const String MessageI::HEADER_TAG_MESSAGE_TYPE = L"HMT";

const String MessageI::HEADER_TAG_ERRORCODE = L"HEC";
const String MessageI::HEADER_TAG_ERRORMESSAGE = L"HEM";

const String MessageI::HEADER_TAG_PROPERTIES = L"HP";

const String MessageI::BODY_TAG = L"B";
const String MessageI::BODY_TAG_COMMAND = L"BC";
const String MessageI::BODY_TAG_COMMAND_PARAMETERS = L"BCP";
const String MessageI::BODY_TAG_LOGLEVEL = L"BL";
const String MessageI::BODY_TAG_DISPATCHING_TIMEOUT = L"BDT";

//********************************************************************

MessageI::~MessageI()
{}

//********************************************************************

MessageI::MessageI()
: anything(new MapAnythingI()) 
{
	setMessageID(Uuid::randomUuid()->toString());
	setTimestamp(Date::NOW()->getTime());
}

//********************************************************************

MessageI::MessageI(Anything anything) throw (jlj::jms::JMSException)
: anything(anything)
{
	if (!anything) throw jlj::jms::JMSException(WITHDETAILS(L"Anything is null"));
}

//********************************************************************

Anything MessageI::convertToAnything() const 
{
	return anything;
}

//********************************************************************

Anything MessageI::getHeader() const 
{
	if (!anything->containsKey(MessageI::HEADER_TAG))
	{
		anything->put(MessageI::HEADER_TAG, new MapAnythingI());
	}
	return anything->get(MessageI::HEADER_TAG);
}

//********************************************************************

Anything MessageI::getDestination() const 
{
	Anything header = getHeader();
	if (!header->containsKey(MessageI::HEADER_TAG_DESTINATION))
	{
		header->put(MessageI::HEADER_TAG_DESTINATION, new MapAnythingI());
	}
	return header->get(MessageI::HEADER_TAG_DESTINATION);
}

//********************************************************************

Anything MessageI::getProperties() const 
{
	Anything header = getHeader();
	if (!header->containsKey(MessageI::HEADER_TAG_PROPERTIES))
	{
		header->put(MessageI::HEADER_TAG_PROPERTIES, new MapAnythingI());
	}
	return header->get(MessageI::HEADER_TAG_PROPERTIES);
}

//********************************************************************

Anything MessageI::getReplyTo() const 
{
	Anything header = getHeader();
	if (!header->containsKey(MessageI::HEADER_TAG_REPLYTO))
	{
		header->put(MessageI::HEADER_TAG_REPLYTO, new MapAnythingI());
	}
	return header->get(MessageI::HEADER_TAG_REPLYTO);
}

//********************************************************************

Anything MessageI::getBody() const 
{
	Anything body;
	if (anything->containsKey(MessageI::BODY_TAG)) body = anything->get(MessageI::BODY_TAG);
	return body;
}

//********************************************************************

void MessageI::setBody(Anything body) const
{
	anything->put(MessageI::BODY_TAG, body);
}

//********************************************************************

String MessageI::getDestinationName() const
{
	if (getDestination()->containsKey(MessageI::HEADER_TAG_DESTINATION_NAME))
	{
		return getDestination()->get(MessageI::HEADER_TAG_DESTINATION_NAME);
	}
	else
	{
		return String();
	}
}

//********************************************************************

void MessageI::setDestinationName(const String& name)
{
	getDestination()->put(MessageI::HEADER_TAG_DESTINATION_NAME, new StringAnythingI(name)); 
}

//********************************************************************

int MessageI::getDestinationType() const
{
	return getDestination()->get(MessageI::HEADER_TAG_DESTINATION_TYPE);
}

//********************************************************************

void MessageI::setDestinationType(int type)
{
	getDestination()->put(MessageI::HEADER_TAG_DESTINATION_TYPE, new IntAnythingI(type));
}

//********************************************************************

String MessageI::getDestinationRemoteID() const 
{    
	return getDestination()->get(MessageI::HEADER_TAG_DESTINATION_REMOTE_ID);
}

//********************************************************************

void MessageI::setDestinationRemoteID(const String& destinationremoteid) 
{    
	getDestination()->put(MessageI::HEADER_TAG_DESTINATION_REMOTE_ID, new StringAnythingI(destinationremoteid));
}

//********************************************************************

bool MessageI::getDestinationJsonEncoding() const
{
	try 
	{
		return getDestination()->get(MessageI::HEADER_TAG_DESTINATION_JSONENCODING);
	}
	catch (const Exception&)
	{
		return true;
	}
}

//********************************************************************

void MessageI::setDestinationJsonEncoding(bool type)
{
	getDestination()->put(MessageI::HEADER_TAG_DESTINATION_JSONENCODING, new BoolAnythingI(type));
}

//********************************************************************

String MessageI::getReplyToName() const {    
	if (getReplyTo()->containsKey(MessageI::HEADER_TAG_REPLYTO_NAME))
	{
		return getReplyTo()->get(MessageI::HEADER_TAG_REPLYTO_NAME);
	}
	else
	{
		return String();
	}
}

//********************************************************************

void MessageI::setReplyToName(const String& replyto) 
{    
	getReplyTo()->put(MessageI::HEADER_TAG_REPLYTO_NAME, new StringAnythingI(replyto));
}

//********************************************************************

int MessageI::getReplyToType() const
{    
	return getReplyTo()->get(MessageI::HEADER_TAG_REPLYTO_TYPE);
}

//********************************************************************

void MessageI::setReplyToType(int type) 
{    
	getReplyTo()->put(MessageI::HEADER_TAG_REPLYTO_TYPE, new IntAnythingI(type));
}

//********************************************************************

String MessageI::getReplyToRemoteID() const
{
	if (getReplyTo()->containsKey(MessageI::HEADER_TAG_REPLYTO_REMOTE_ID))
	{
		return getReplyTo()->get(MessageI::HEADER_TAG_REPLYTO_REMOTE_ID);
	}
	else
	{
		return String();
	}
}

//********************************************************************

void MessageI::setReplyToRemoteID(const String& replytoremoteid)
{    
	getReplyTo()->put(MessageI::HEADER_TAG_REPLYTO_REMOTE_ID, new StringAnythingI(replytoremoteid));
}

//********************************************************************

bool MessageI::getReplyToJsonEncoding() const
{
	try 
	{
		return getReplyTo()->get(MessageI::HEADER_TAG_REPLYTO_JSONENCODING);
	}
	catch (const Exception&)
	{
		return true;
	}
}

//********************************************************************

void MessageI::setReplyToJsonEncoding(bool type)
{
	getReplyTo()->put(MessageI::HEADER_TAG_REPLYTO_JSONENCODING, new BoolAnythingI(type));
}

//********************************************************************

bool MessageI::propertyExists(const String& name) const 
{    
	return getProperties()->containsKey(name);
}

//********************************************************************

String MessageI::getStringProperty(const String& name) const 
{  
	if (propertyExists(name))
	{
		return getProperties()->get(name);
	}
	else
	{
		return L"";
	}
}

//********************************************************************

void MessageI::setStringProperty(const String& propertyname, const String& value) 
{
	getProperties()->put(propertyname, new StringAnythingI(value));
}

//********************************************************************

List<String> MessageI::getPropertyNames() const
{
	List<String> names = new ArrayListI<String>(getProperties()->size());
	names->addAll(getProperties()->keySet());
	return names;
}

//********************************************************************

String MessageI::getMessageID() const
{
	if (getHeader()->containsKey(MessageI::HEADER_TAG_MESSAGE_ID))
	{
		return getHeader()->get(MessageI::HEADER_TAG_MESSAGE_ID);
	}
	else
	{
		return String();
	}
}

//********************************************************************

void MessageI::setMessageID(const String& messageid) 
{
	getHeader()->put(MessageI::HEADER_TAG_MESSAGE_ID, new StringAnythingI(messageid));
}

//********************************************************************

int MessageI::getMessageType() const
{
	if (getHeader()->containsKey(MessageI::HEADER_TAG_MESSAGE_TYPE))
	{
		return getHeader()->get(MessageI::HEADER_TAG_MESSAGE_TYPE);
	}
	else
	{
		return Message::UNKNOWN_MESSAGE;
	}
}

//********************************************************************

void MessageI::setMessageType(int messagetype)
{
	getHeader()->put(MessageI::HEADER_TAG_MESSAGE_TYPE, new IntAnythingI(messagetype));
}

//********************************************************************

double MessageI::getTimestamp() const 
{
	return getHeader()->get(MessageI::HEADER_TAG_TIMESTAMP);
}

//********************************************************************

void MessageI::setTimestamp(double timestamp) 
{
	getHeader()->put(MessageI::HEADER_TAG_TIMESTAMP, new DoubleAnythingI(timestamp));
}

//********************************************************************

int MessageI::getErrorCode() const
{    
	if (getHeader()->containsKey(MessageI::HEADER_TAG_ERRORCODE))
	{
		return getHeader()->get(MessageI::HEADER_TAG_ERRORCODE);
	}
	else
	{
		return Message::SUCCESS;
	}
}

//********************************************************************

void MessageI::setErrorCode(int errorcode) 
{    
	getHeader()->put(MessageI::HEADER_TAG_ERRORCODE, new IntAnythingI(errorcode));
}

//********************************************************************

String MessageI::getErrorMessage() const
{    
	if (getHeader()->containsKey(MessageI::HEADER_TAG_ERRORMESSAGE))
	{
		return getHeader()->get(MessageI::HEADER_TAG_ERRORMESSAGE);
	}
	else
	{
		return String();
	}
}

//********************************************************************

void MessageI::setErrorMessage(const String& errormessage) 
{    
	getHeader()->put(MessageI::HEADER_TAG_ERRORMESSAGE, new StringAnythingI(errormessage));
}

//********************************************************************

int MessageI::getCommand() const 
{
	return getBody()->get(MessageI::BODY_TAG_COMMAND);
}

//********************************************************************

Anything MessageI::getCommandParameter(int parameter) const
{   
	return getBody()->get(MessageI::BODY_TAG_COMMAND_PARAMETERS)->get(String(parameter));
}

//********************************************************************

void MessageI::setCommand(int command) 
{   
	if (!getBody()) setBody(new MapAnythingI());
	getBody()->put(MessageI::BODY_TAG_COMMAND, new IntAnythingI(command));
}

//********************************************************************

void MessageI::setCommandParameter(int parameter, Anything value) 
{   
	if (!getBody()->containsKey(MessageI::BODY_TAG_COMMAND_PARAMETERS)) 
	{
		getBody()->put(MessageI::BODY_TAG_COMMAND_PARAMETERS, new MapAnythingI());
	}
	getBody()->get(MessageI::BODY_TAG_COMMAND_PARAMETERS)->put(String(parameter), value);
}

//********************************************************************

String MessageI::getLogLevel() const 
{
	return getBody()->get(MessageI::BODY_TAG_LOGLEVEL);
}

//********************************************************************

void MessageI::setLogLevel(const String& loglevel)
{    
	if (!getBody()) setBody(new MapAnythingI());
	getBody()->put(MessageI::BODY_TAG_LOGLEVEL, new StringAnythingI(loglevel));
}

//********************************************************************

int MessageI::getDispatcherTimeout() const 
{    
	return getBody()->get(MessageI::BODY_TAG_DISPATCHING_TIMEOUT);
}

//********************************************************************

void MessageI::setDispatcherTimeout(int timeout)
{    
	if (!getBody()) setBody(new MapAnythingI());
	getBody()->put(MessageI::BODY_TAG_DISPATCHING_TIMEOUT, new IntAnythingI(timeout));
}

//********************************************************************

jlj::jms::Message MessageI::convertToJMSMessage() throw (jlj::jms::JMSException)
{
	jlj::jms::Message message;
	switch (getMessageType())
	{
	case Message::TEXT_MESSAGE:
		{
			message = new TextMessageI(anything);
			break;
		}
	case Message::ANYTHING_MESSAGE:
		{
			message = new AnythingMessageI(anything);
			break;
		}
	default:
		{
			throw jlj::jms::JMSException(WITHDETAILS(L"Invalid message type : " + getMessageType()));
		}
	}
	return message;
}

//********************************************************************

Object MessageI::clone() const 
{
	Anything clone;
	anything->clone()->downcast(clone);
	return new MessageI(clone);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
