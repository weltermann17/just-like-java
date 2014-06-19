// RequestI.cpp

//********************************************************************

#ifndef jmstools_request_cpp
#define jmstools_request_cpp

//********************************************************************

#include <Request.h>

#include <iostream>
using namespace std;

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <jmstools/implementation/reactor/Message.h>
using namespace jmstools::implementation::reactor;
#include <jlj/util/NullAnything.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/UTF8StringAnything.h>
#include <jlj/util/StreamAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/DoubleAnything.h>
#include <jlj/util/zip/ZipOutputStream.h>
#include <jlj/util/zip/ZipInputStream.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/Date.h>
#include <jlj/lang/Throwable.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <jlj/io/FileInputStream.h>

#include <jlj/nio/reactor/Reactor.h>

using namespace jlj::nio;
using namespace jlj::io;
using namespace zip;
using namespace jlj::util;
using namespace jlj::nio::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

RequestI::~RequestI()
{
	logger->log(logging::Level::FINEST, L"RequestI::~RequestI()");
}

//********************************************************************

RequestI::RequestI(Connection connection, 
				   Reactor reactor,
				   Logger logger,
				   BlockingQueue<int> requeststopped,
				   String& type,
				   String& path,
				   int minblocksize,
				   int maxblocksize,
				   int innerloop)
				   : connection(connection)
				   , reactor(reactor)
				   , logger(logger)
				   , requeststopped(requeststopped)
				   , type(type)
				   , path(path)
				   , minblocksize(minblocksize)
				   , maxblocksize(maxblocksize)
				   , innerloop(innerloop)
{}

//********************************************************************

Anything RequestI::receiveAnswer() throw (IOException)
{
	Anything anything;
	bool aborted = false;
	
	logger->log(Level::FINEST, L" RequestI::receiveAnswer()");	
	static AtomicInteger count(0);

	while (!aborted)
	{
		
		Anything anything = reactor->receiveQueue()->poll(30 * 1000);
		
		if (!anything)
		{
			aborted = true;
			logger->log(Level::SEVERE, L"RequestI::receiveAnswer() : timeout ");
		}
		else
		{
		
			aborted = anything->isNull();
			
			if (!aborted )
			{	
				++count;
				logger->log(Level::SEVERE, L"replies : " + count->toString());
				if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"content : " + anything->toString());	
				break;
			}

		}
	}
	logger->log(logging::Level::INFO, L"End Receiving : ");
	return anything;
}

//********************************************************************

int RequestI::call() throw (jlj::lang::Exception)
{
	try 
	{	
		logger->log(logging::Level::INFO, L"Start Sending : "+ type);

		for (int i = 0; i < innerloop; ++i)
		{
			logger->log(logging::Level::INFO, L"Start Sending : " + String(i));

			if ( type == L"STRING" ) 
			{
				logger->log(logging::Level::INFO, L"Sending message STRING mode: " + String(i));
				sendMessage(createRandomMessage(minblocksize,maxblocksize));
			}
			else if ( type == L"STREAM" ) 
			{
				logger->log(logging::Level::INFO, L"Sending message STREAM mode: " + String(i));
				sendMessage(createMessageFromFile(path));
			}
			else
			{
				logger->log(logging::Level::INFO, L"Sending message Error Undetermined type: " + String(i));
				break;
			}
			// receiveAnswer();			
		}
	}
	catch (const jlj::lang::Exception& e) 
	{
		cout << "RequestI::call() : " << e->toString() << endl;
	}
	catch (...)
	{
		cout << "RequestI::call() : Unknown exception." << endl;
	}

	Thread::sleep(5*1000); // important to be long enough for the other threads to complete the receive

	reactor->initializeShutdown(15*1000);
	requeststopped->put(-1);
	return 0;
}

//********************************************************************

UTF8String RequestI::createRandomString(int length) const
{           
	UTF8StringBuffer buf(length);
	bool ready = false;
	while (!ready)
	{
		int l = 1 + (rand() % 20000);
		char c = 'A' + (rand() % 26);
		for (int i = 0; i < l; ++i) 
		{
			if (length <= buf->length()) { ready = true; break; }
			buf->append(c);
		}
	}
	logger->log(logging::Level::INFO,  L"randomString() : " + String(length));
	return buf->toUTF8String();
}

//********************************************************************

Anything RequestI::createMessageHeader()
{
	Anything header = new MapAnythingI();

	header->put(MessageI::HEADER_TAG_DESTINATION, new StringAnythingI(L"SADIS"));
	header->put(MessageI::HEADER_TAG_DESTINATION_TYPE, new IntAnythingI(Message::DEST_PERSISTANT_QUEUE));

	header->put(MessageI::HEADER_TAG_REPLYTO, new StringAnythingI(Uuid::randomUuid()->toString()));
	header->put(MessageI::HEADER_TAG_REPLYTO_TYPE, new StringAnythingI(Message::DEST_TEMPORARY_QUEUE));

	logger->log(logging::Level::INFO,  L"RequestI::createMessageHeader() Local identifier :" + String(connection->getLocalIdentifier()) );
	logger->log(logging::Level::INFO,  L"RequestI::createMessageHeader() Remote identifier :" + String(connection->getRemoteIdentifier()) );

	header->put(MessageI::HEADER_TAG_REPLYTO_REMOTE_ID, new StringAnythingI(connection->getLocalIdentifier()));
	//header->put(HEADER_TAG_REPLYTO_REMOTE_ID, new StringAnythingI(connection->getRemoteIdentifier()));

	header->put(MessageI::HEADER_TAG_MESSAGE_ID, new StringAnythingI(Uuid::randomUuid()->toString()));
	header->put(MessageI::HEADER_TAG_TIMESTAMP, new DoubleAnythingI(Date::NOW()->getTime()));
	header->put(MessageI::HEADER_TAG_MESSAGE_TYPE, new StringAnythingI(L"AnythingMessage"));

	Anything result = new MapAnythingI();
	result->put(MessageI::HEADER_TAG,  header);
	return result;
}

//********************************************************************

Anything RequestI::createMessageFromFile(String& filename) throw (IOException)  
{
	logger->log(logging::Level::INFO, L"RequestI::createMessageFromFile :"+ filename);
	Anything message = createMessageHeader();
	InputStream file = new FileInputStreamI(filename);
	message->put(MessageI::BODY_TAG, new StreamAnythingI(file));
	return message;

}

//********************************************************************

Anything RequestI::createRandomMessage(int min, int max)
{
	int length = min + ((rand() * rand()) % (max - min)); 
	Message message = new MessageI();
	message->setMessageType(Message::TEXT_MESSAGE);
	message->setDestinationName(L"SADIS");
	message->setDestinationType(Message::DEST_PERSISTANT_QUEUE);
	message->setDestinationJsonEncoding(false);
	message->setReplyToName(L"DUMMY");
	message->setReplyToType(Message::DEST_TEMPORARY_QUEUE);
	message->setReplyToRemoteID(connection->getLocalIdentifier());
	message->setReplyToJsonEncoding(false);

	Anything body = new StringAnythingI(createRandomString(length));
	message->setBody(new UTF8StringAnythingI(body->toBase64(true)));

	if (logger->isLoggable(Level::FINEST)) logger->log(logging::Level::FINEST, L"RequestI::createRandomMessage() : " + message->convertToAnything()->toString());

	return message->convertToAnything();
}

//********************************************************************

void RequestI::sendMessage(Anything message) const throw (IOException)
{
	logger->log(logging::Level::INFO, L"RequestI::sendMessage : ");
	connection->send(message->toJSON(), ZipUtility::NO_COMPRESSION);
}

//********************************************************************

NAMESPACE_END(jmstools)

#endif
// eof