// Reply.cpp

//********************************************************************

#ifndef jmstools_Reply_cpp
#define jmstools_Reply_cpp

//********************************************************************

#include <Reply.h>

#include <iostream>
using namespace std;

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>

#include <jmstools/implementation/reactor/Message.h>
using namespace jmstools::implementation::reactor;
#include <jlj/util/Collections.h>
#include <jlj/util/LinkedHashSet.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/StreamAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/DoubleAnything.h>
#include <jlj/util/Date.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/zip/ZipOutputStream.h>
#include <jlj/util/zip/ZipInputStream.h>
#include <jlj/io/InMemoryOutputStream.h>
#include <jlj/io/InMemoryInputStream.h>
#include <jlj/lang/Throwable.h>

using namespace jlj::nio;
using namespace jlj::util::zip;
using namespace jlj::util;
using namespace jlj::io;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

AtomicBoolean ReplyI::registeronce(false);

SynchronizedSet<String> tempqueues;


//********************************************************************

ReplyI::~ReplyI()
{}

//********************************************************************

ReplyI::ReplyI(Connection connection, 
               Reactor reactor, 
               Logger logger, 
               BlockingQueue<int> replystopped,
               int replylength)
               : connection(connection)
               , reactor(reactor)
               , logger(logger)
               , replystopped(replystopped)
               , replylength(replylength)
{
    if (!registeronce->getAndSet(true))
    {
        sendMessage(createInternalMessage(Message::COMMAND_REGISTER));
    }
}

//********************************************************************

int ReplyI::call() throw (jlj::lang::Exception)
{
    try 
    {
        static AtomicInteger count(0);
        bool aborted = false;

        while (!aborted)
        {
            Anything anything = reactor->receiveQueue()->poll(1 * 60 * 1000);
            if (!!anything)
            {
                Message message = new MessageI(anything);
                logger->log(Level::SEVERE, L"request : " + anything->toString());
                logger->log(Level::SEVERE, L"destination type : " + message->getDestinationType());
                if (Message::DEST_PERSISTANT_QUEUE == message->getDestinationType())
                {
                    ++count;
                    aborted = anything->isNull();

                    logger->log(Level::SEVERE, L"requests : " + count->toString());
                    if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, anything->toString());
                    if (!aborted)
                    {
                        //Send reply message					
                        sendMessage(createReplyMessage(anything, new StringAnythingI(createRandomString(23))));
                    }
                }
                else
                {
                    logger->log(Level::SEVERE, L"This is a control message");
                }
            }
            else
            {
                aborted = true;
                if (registeronce->getAndSet(false))
                {
                    logger->log(Level::SEVERE, L"Unregistering ...");
                    sendMessage(createInternalMessage(Message::COMMAND_UNREGISTER));
                    Anything ackn = reactor->receiveQueue()->poll(10*1000);
                    if (!!ackn) logger->log(Level::SEVERE, ackn->toString());
                    logger->log(Level::SEVERE, L"Shutting the reactor...");
                    reactor->initializeShutdown(15000);
                }
            }
        }
    }
    catch (const jlj::lang::Exception& e) 
    {
        logger->log(Level::SEVERE, L"ReplyI::call() : " + e->toString());
    }
    catch (...)
    {
        logger->log(Level::SEVERE, L"ReplyI::call() : Unknown exception.");
    }

    replystopped->put(-1);
    return 1;	
}

//********************************************************************

String ReplyI::createRandomString(int length) const
{
    StringBuffer buf(length);
    for (int i = 0; i < length; ++i)
    {
        wchar_t c = L'A' + (rand() % 26);
        buf->append(c);
    }
    return buf->toString();
}

//********************************************************************

Anything ReplyI::createInternalMessage(int command) 
{

    Message message = new MessageI();
    message->setMessageType(Message::TEXT_MESSAGE);
    message->setDestinationName(L"INTERNAL");
    message->setDestinationType(Message::DEST_INTERNAL);
	message->setDestinationJsonEncoding(false);
    message->setReplyToName(Uuid::randomUuid()->toString());
    message->setReplyToType(Message::DEST_TEMPORARY_QUEUE);
    message->setReplyToRemoteID(connection->getLocalIdentifier());
    message->setCommand(Message::COMMAND_REGISTER);
    
    message->setCommandParameter(Message::COMMAND_REGISTER_QUEUE, new StringAnythingI(L"SADIS"));
    //message->setCommandParameter(Message::COMMAND_REGISTER_QUEUE, new StringAnythingI(L"QueueI<SADIS>"));
   

    return message->convertToAnything();
}

//********************************************************************

Anything ReplyI::createReplyMessage(Anything receivedmessage, Anything answermessage)
{
    Message result = new MessageI(receivedmessage);


    result->setBody(answermessage);
    result->setDestinationName(result->getReplyToName());
    result->setDestinationType(result->getReplyToType());
 	result->setDestinationJsonEncoding(result->getReplyToJsonEncoding());
    result->setDestinationRemoteID(result->getReplyToRemoteID());


    /*if (!receivedmessage->containsKey(MessageI::HEADER_TAG)) return	result;
    if (!receivedmessage->get(MessageI::HEADER_TAG)->containsKey(MessageI::HEADER_TAG_REPLYTO_REMOTE_ID)) return result;

    Anything header = new MapAnythingI();

    header->put(MessageI::HEADER_TAG_DESTINATION_TYPE, new IntAnythingI(Message::DEST_TEMPORARY_QUEUE));
    header->put(MessageI::HEADER_TAG_DESTINATION_REMOTE_ID, receivedmessage->get(MessageI::HEADER_TAG)->get(MessageI::HEADER_TAG_REPLYTO_REMOTE_ID));

    header->put(MessageI::HEADER_TAG_MESSAGE_ID, new StringAnythingI(Uuid::randomUuid()->toString()));
    header->put(MessageI::HEADER_TAG_TIMESTAMP, new DoubleAnythingI(Date::NOW()->getTime()));
    header->put(MessageI::HEADER_TAG_MESSAGE_TYPE, new StringAnythingI(L"AnythingMessage"));

    result = new MapAnythingI();
    result->put(MessageI::HEADER_TAG, header);

    result->put(MessageI::BODY_TAG, answermessage);

    if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"Reply message : " + result->toString());	
    */
    return result->convertToAnything();
}

//********************************************************************

void ReplyI::sendMessage(Anything message) const
{
    if (!message) return;

    try 
    {
        connection->send(message, ZipUtility::NO_COMPRESSION);
    } 
    catch (const IOException& e)
    {
        cout << "ReplyI::sendMessage() : IO " << e->toString() << endl;
    }
}

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif
// eof