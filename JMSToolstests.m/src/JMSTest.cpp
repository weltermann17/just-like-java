// JMSTest.cpp

//********************************************************************

#ifndef jmstools_jmstest_cpp
#define jmstools_jmstest_cpp

//********************************************************************

#include <JMSTest.h>
#include <Request.h>
#include <Reply.h>

#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/nio/channels/ServerSocketChannel.h>
#include <jlj/nio/channels/SocketChannel.h>
#include <jlj/nio/channels/Selector.h>
#include <jlj/nio/channels/SelectionKey.h>
#include <jlj/nio/reactor/Reactor.h>
#include <jlj/nio/reactor/Connector.h>
#include <jlj/nio/reactor/Acceptor.h>

using namespace jlj::nio;
using namespace jlj::nio::reactor;

#include <jlj/jms/Message.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/DoubleAnything.h>
#include <jlj/util/Date.h>
#include <jlj/util/concurrent/ThreadPoolExecutor.h>
#include <jmstools/implementation/reactor/Message.h>
using namespace jmstools::implementation::reactor;

using namespace jlj::util;
using namespace jlj::util::concurrent;

#include <jlj/net/InetSocketAddress.h>
#include <jlj/lang/Throwable.h>
#include <jlj/lang/String.h>
#include <jlj/lang/Thread.h>
using namespace jlj::lang;
#include <jlj/naming/InitialContext.h>
using namespace jlj::naming;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

JMSTestI::~JMSTestI()
{}

//********************************************************************

JMSTestI::JMSTestI(const String& type,
				   const String& path,
				   int outerloop,
				   int submitloop,
				   int innerloop,
				   int min_blocksize,
				   int max_blocksize)
				   : type(type)
				   , path(path)
				   , outerloop(outerloop)
				   , submitloop(submitloop)
				   , innerloop(innerloop)
				   , min_blocksize(min_blocksize)
				   , max_blocksize(max_blocksize)
				   , logger(Logger::getLogger(jlj::jms::JMS_LOGGER_NAME))
{}

//********************************************************************

void JMSTestI::RequestSimpleConnection()
{
	logger->log(Level::INFO, L"REQUEST SIDE");

	ExecutorService<int> threadpool = new ThreadPoolExecutorI<int>(
		4, 
		64, 
		15000, 
		new LinkedBlockingQueueI<Runnable>());

	Reactor reactor = Reactor::open(threadpool, 15000);
	reactor->setShutdownOnLastConnectionClosed(true);

	InitialContext initialcontext = InitialContext::newInstance();

	Anything providerhost = initialcontext->isBound(L"/env/jms_provider_host") ? initialcontext->lookup(L"/env/jms_provider_host")->toString() : String(L"localhost");
	Anything providerport = 14014;
	if (initialcontext->isBound(L"/env/jms_provider_port"))
		initialcontext->lookup(L"/env/jms_provider_port", providerport);

	Connector connector = Connector::open(reactor, 15000);
	Connection connection = connector->connect(new InetSocketAddressI(
		providerhost, 
		providerport));

	BlockingQueue<int> requeststopped = new LinkedBlockingQueueI<int>();

	for (int i = 0 ; i < submitloop ; i++)
	{
		threadpool->submit(
			new RequestI(
			connection,
			reactor,
			logger,
			requeststopped,
			type,
			path,
			min_blocksize,
			max_blocksize,
			innerloop));
	}

	reactor->setShutdownOnLastConnectionClosed(true);
	reactor->run();

	for (int i = 0 ; i < submitloop ; i++)
	{
		requeststopped->take();
	}
	logger->log(Level::SEVERE, L"JMSTestI::RequestSimpleConnection() : stopped");
}

//********************************************************************

void JMSTestI::ReplyConnection()
{
	logger->log(Level::INFO, L"Reply:");

	ExecutorService<int> threadpool = new ThreadPoolExecutorI<int>(
		4, 
		64, 
		15000, 
		new LinkedBlockingQueueI<Runnable>());

	Reactor reactor = Reactor::open(threadpool, 15000);
	reactor->setShutdownOnLastConnectionClosed(true);

	InitialContext initialcontext = InitialContext::newInstance();

	Anything providerhost = initialcontext->isBound(L"/env/jms_provider_host") ? initialcontext->lookup(L"/env/jms_provider_host")->toString() : String(L"localhost");
	Anything providerport = 14014;
	if (initialcontext->isBound(L"/env/jms_provider_port"))
		initialcontext->lookup(L"/env/jms_provider_port", providerport);

	Connector connector = Connector::open(reactor, 15000);
	Connection connection = connector->connect(new InetSocketAddressI(
		providerhost, 
		providerport));

	BlockingQueue<int> replystopped = new LinkedBlockingQueueI<int>();

	for (int i = 0 ; i < submitloop ; i++)
	{
		threadpool->submit(
			new ReplyI(
			connection,
			reactor,
			logger,
			replystopped,
			10));
	}

	reactor->run();

	for (int i = 0 ; i < submitloop ; i++)
	{
		replystopped->take();
		logger->log(Level::SEVERE, L"JMSTestI::ReplyConnection() : stopped = " + String(i + 1));
	}
}


//********************************************************************

void JMSTestI::RequestConnection()
{
	for (int i = 0; i < outerloop; ++i)
	{
		logger->log(logging::Level::SEVERE, L"Start Connection : " + String(i));
		RequestSimpleConnection();

		int interval = i == outerloop - 1 ? 0 : (rand() % 10) * 1000;
		if (0 < interval)
		{
			logger->log(logging::Level::SEVERE, L"sleep before next connection : " + String(interval));
			Thread::sleep(interval);
		}
	}
}

//********************************************************************

NAMESPACE_END(jmstools)

#endif

// eof
