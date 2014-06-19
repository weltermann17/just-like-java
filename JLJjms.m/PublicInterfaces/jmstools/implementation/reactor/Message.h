// Message.h

//********************************************************************

#ifndef jmstools_implementation_reactor_Message_h
#define jmstools_implementation_reactor_Message_h

//********************************************************************

#include <JMSreactorimplementation.h>
#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
#include <jlj/util/Anything.h>
#include <jlj/jms/Message.h>
#include <jlj/jms/Exceptions.h>

using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

class ExportedByJMSreactorimplementation MessageI 
	: public ObjectI
{
protected:
	~MessageI();
	friend class TypedReference<MessageI>;

public:
	MessageI();
	MessageI(Anything) throw (jlj::jms::JMSException);

public:
	Anything convertToAnything() const;
	
	// every get can return a null pointer, so be prepared
	// evere set will overwrite an eventually existing value 

	Anything getBody() const;			
	void setBody(Anything anything) const;    

	String getDestinationName() const;
	int getDestinationType() const;
	String getDestinationRemoteID() const;
	bool getDestinationJsonEncoding() const;
	void setDestinationRemoteID(const String&);
	void setDestinationType(int);
	void setDestinationName(const String&);
	void setDestinationJsonEncoding(bool);

	bool propertyExists(const String&) const;
	List<String> getPropertyNames() const;
	String getStringProperty(const String&) const;
	void setStringProperty(const String& name, const String& value);

	String getReplyToName() const;
	int getReplyToType() const ;
	String getReplyToRemoteID() const;
	bool getReplyToJsonEncoding() const ;
	void setReplyToName(const String&);
	void setReplyToType(int);
	void setReplyToRemoteID(const String&);
	void setReplyToJsonEncoding(bool);

	int getMessageType() const;
	
	String getMessageID() const;
	double getTimestamp() const;
	void setMessageID(const String&);
	void setTimestamp(double);

	int getErrorCode() const;
	String getErrorMessage() const;
	int getCommand() const;
	Anything getCommandParameter(int) const;
	String getLogLevel() const;
	int getDispatcherTimeout() const;
	void setErrorCode(int);
	void setErrorMessage(const String&);
	void setCommand(int);
	void setCommandParameter(int, Anything);
	void setLogLevel(const String&);
	void setDispatcherTimeout(int);

	void setMessageType(int);
	jlj::jms::Message convertToJMSMessage() throw (jlj::jms::JMSException);

	Object clone() const; 

public:
	static const String HEADER_TAG;
	static const String HEADER_TAG_DESTINATION;
	static const String HEADER_TAG_DESTINATION_NAME;  //internal, control, temporary queue, persistent queue
	static const String HEADER_TAG_DESTINATION_TYPE;
	static const String HEADER_TAG_DESTINATION_REMOTE_ID;
	static const String HEADER_TAG_DESTINATION_JSONENCODING;

	static const String HEADER_TAG_REPLYTO;
	static const String HEADER_TAG_REPLYTO_NAME;
	static const String HEADER_TAG_REPLYTO_TYPE;
	static const String HEADER_TAG_REPLYTO_REMOTE_ID;
	static const String HEADER_TAG_REPLYTO_JSONENCODING;

	static const String HEADER_TAG_MESSAGE_ID;
	static const String HEADER_TAG_MESSAGE_TYPE;
	static const String HEADER_TAG_TIMESTAMP;

	static const String HEADER_TAG_ERRORCODE;
	static const String HEADER_TAG_ERRORMESSAGE;

	static const String HEADER_TAG_PROPERTIES;

	static const String BODY_TAG;
	static const String BODY_TAG_COMMAND;
	static const String BODY_TAG_COMMAND_PARAMETERS;
	static const String BODY_TAG_LOGLEVEL;
	static const String BODY_TAG_DISPATCHING_TIMEOUT;

private:
	Anything getHeader() const;
	Anything getDestination() const;
	Anything getReplyTo() const;
	Anything getProperties() const;

private:
	mutable Anything anything;

};

//********************************************************************

struct ExportedByJMSreactorimplementation Message : StrongReference<MessageI> 
{    
	StrongReferenceCommonDerivateImplementation(Message);
	

	static const String JMS_LOGGER_NAME;

	enum DESTINATION_TYPES
	{
		DEST_NOT_DETERMINED = 0,
		DEST_PERSISTANT_QUEUE = 1,
		DEST_TEMPORARY_QUEUE = 2,
		DEST_PERSISTANT_TOPIC,
		DEST_TEMPORARY_TOPIC,
		DEST_INTERNAL = 5,
		DEST_CONTROL = 6
	};

	enum ERRORCODE_TYPES
	{
		SUCCESS = 0,
		WARNING,
		FAILURE,
		UNKNOW
	};

	enum COMMAND_TYPES
	{
		COMMAND_NOT_DETERMINED = 0,
		COMMAND_SHUTDOWN,
		COMMAND_SETGLOBALLOGLEVEL,
		COMMAND_SETREACTORLOGLEVEL,
		COMMAND_SETCONSOLEHANDLERLOGLEVEL,
		COMMAND_SETLOGLEVEL,
		COMMAND_REGISTER = 6,
		COMMAND_UNREGISTER = 7,
		COMMAND_REGISTER_QUEUE = 8,
		COMMAND_MONITOR = 9,
		COMMAND_MONITOR_FORNAGIOS
	};

	enum MESSAGE_TYPES
	{
		UNKNOWN_MESSAGE = 0,
		TEXT_MESSAGE,
		ANYTHING_MESSAGE
	};

};

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

//********************************************************************

#endif