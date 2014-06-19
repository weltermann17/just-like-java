// Thread.cpp

//********************************************************************

#ifndef Thread_cpp
#define Thread_cpp

//********************************************************************

#include <jlj/lang/Thread.h>
#include <jlj/lang/Runnable.h>

//********************************************************************

#include <jlj/lang/String.h>
#include <jlj/lang/Throwable.h>
#include <jlj/core/ptypes/pasync.h>
#include <jlj/core/ptypes/ptime.h>

//********************************************************************

#include <list>
#include <stdio.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

class _threadpool
{
public:
	~_threadpool();	
	_threadpool();

	ThreadImpl* nextIdle();
	void makeIdle(ThreadImpl*);
	void makeReadyForDelete(ThreadImpl*);

	void enlarge(int targetpoolsize);
	void remove(int delta);

	int size() const;

	void setMinSize(int);
	void setMaxSize(int);
	void setBatchSize(int);
	void setAddTimeout(int);
	void setAddTimeoutAtMaxSize(int);
	void setRemoveTimeout(int);
	void setDestroyTimeout(int);

	enum { 
		DEFAULT_MINSIZE = 0, 
		DEFAULT_MAXSIZE = 12, 
		DEFAULT_BATCHSIZE = 4, 
		DEFAULT_ADDTIMEOUT = 500,
		DEFAULT_ADDTIMEOUTATMAXSIZE = 90 * 1000, 
		DEFAULT_REMOVETIMEOUT = 90 * 1000, 
		DEFAULT_DESTROYTIMEOUT = 1 * 1000 
	}; 

private:
	class _adminthread
		: public pt::thread
	{
	public:
		~_adminthread();
		_adminthread();

		void setOwner(_threadpool*);		
		void execute();

	private:
		_threadpool* owner;

	};

	class _monitorthread
		: public pt::thread
	{
	public:
		~_monitorthread();
		_monitorthread();

		void setOwner(_threadpool*);		
		void execute();

	private:
		_threadpool* owner;

	};

	friend class _adminthread;
	friend class _monitorthread;
	friend class ThreadImpl;

	std::list<ThreadImpl*> pool;
	int poolsize;
	pt::jobqueue* idle;	
	pt::jobqueue* readyfordelete;	
	pt::jobqueue* requests;
	pt::jobqueue* monitor;
	_adminthread* adminthread;
	_monitorthread* monitorthread;

	enum { ABORT = 0, ENLARGE = 1, REMOVE = 2, BUSY = 3, DESTROY = 4, IDLE = 5 , WAITING = 6, WORKING = 7 };

	bool logging;
	int minsize;
	int maxsize;
	int batchsize;
	int addtimeout;
	int initialaddtimeout;
	int addtimeoutatmaxsize;
	int removetimeout;
	int destroytimeout;

	int id;

};

//********************************************************************

_threadpool*& threadpool()
{
	static _threadpool* threadpool_ = 0;
	return threadpool_;
}

//********************************************************************

class ExportedByJLJlang ThreadImpl
{
public:

	void pseudoDestruct();

	void pseudoConstruct(Runnable);
	void pseudoConstruct(Runnable, const String& name); 

	bool isAlive() const;
	bool isInterrupted() const;
	void interrupt();
	void join(int milliseconds) throw (InterruptedException);
	void join() throw (InterruptedException);
	void start();
	void run();

	String getName() const;
	void setName(const String&); 

	Runnable getRunnable();

	static void createPool(
		int size,
		int maxsize,
		int batchsize,
		int addtimeout,
		int addtimeoutatmax,
		int removetimeout,
		int destroytimeout) throw (NullPointerException);

	static void destroyPool();

private:

	~ThreadImpl();		// called manually only

	void setAlive(bool);

	void _start();		// really start 
	void signal();		// really interrupt
	void waitFor();		// really join

	bool isDestroyed() const;
	bool isSignaled() const;

	// real lifecycle

	friend class _threadpool;
	friend class _threadpool::_adminthread;

	ThreadImpl(int id);	// called by inplace new
	void finalize();

	static void* operator new(size_t, ThreadImpl*);

	// need to derive from pt::thread

	class threadimpl
		: public pt::thread
	{
	public:
		~threadimpl();
		threadimpl(ThreadImpl*);

		void execute();

	private:
		ThreadImpl* owner;		
	};

	threadimpl* thread;	
	Runnable runnable;

	int id;
	String name;

	int running;
	int interrupted;
	int constructed;

	pt::jobqueue* startjobs;	
	pt::jobqueue* stopjobs;	

	enum { ABORT = 0, START = 1, JOIN = 2 , HASSTARTED = 3 };
	enum { JOBTIMEOUT = 5 * 60 * 1000 };	
};

//********************************************************************
// 
// _ t h r e a d p o o l 
// 
//********************************************************************

_threadpool::~_threadpool()
{
	try
	{
		requests->post(new pt::message(ABORT)); 
		monitor->post(new pt::message(ABORT)); 

		adminthread->signal();
		adminthread->waitfor();

		delete adminthread;
		adminthread = 0;

		monitorthread->signal();
		monitorthread->waitfor();

		delete monitorthread;
		monitorthread = 0;

		delete idle;
		idle = 0;

		delete requests;
		requests = 0;

		delete monitor;
		monitor = 0;

		delete readyfordelete;
		readyfordelete = 0;

		if (0 < pool.size()) fprintf(stderr, "ThreadPool::~ThreadPool() : size = %d\n", pool.size());
	}
	catch (...)
	{}
}

//********************************************************************

_threadpool::_threadpool()
: adminthread(new _threadpool::_adminthread())
, monitorthread(new _threadpool::_monitorthread())
, poolsize(0)
, idle(new pt::jobqueue())
, requests(new pt::jobqueue())
, monitor(new pt::jobqueue())
, readyfordelete(new pt::jobqueue())
, minsize(DEFAULT_MINSIZE)
, maxsize(DEFAULT_MAXSIZE)
, batchsize(DEFAULT_BATCHSIZE)
, addtimeout(DEFAULT_ADDTIMEOUT)
, initialaddtimeout(DEFAULT_ADDTIMEOUT)
, addtimeoutatmaxsize(DEFAULT_ADDTIMEOUTATMAXSIZE)
, removetimeout(DEFAULT_REMOVETIMEOUT)
, destroytimeout(DEFAULT_DESTROYTIMEOUT)
, id(0)
, logging(0 != getenv("JLJ_THREADPOOL_LOGGING"))
{
	adminthread->setOwner(this);
	adminthread->start();

	monitorthread->setOwner(this);
	monitorthread->start();
}

//********************************************************************

ThreadImpl* _threadpool::nextIdle()
{
	ThreadImpl* thread = 0;

	int ADDTIMEOUTATMAXSIZE;
	pt::pexchange(&ADDTIMEOUTATMAXSIZE, addtimeoutatmaxsize);

	int TIMEOUT;
	pt::pexchange(&TIMEOUT, addtimeout);

	const int RETRIES = 0 >= TIMEOUT ? ADDTIMEOUTATMAXSIZE : ADDTIMEOUTATMAXSIZE / TIMEOUT;
	int retry = RETRIES;

	if (TIMEOUT == ADDTIMEOUTATMAXSIZE) TIMEOUT /= 0 < RETRIES ? RETRIES : 1;

	int POOLSIZE;
	pt::pexchange(&POOLSIZE, poolsize);

	while (0 == thread && 0 < retry)
	{		
		if (logging) fprintf(stdout, "threadpool::nextIdle() : %d %d %d\n", TIMEOUT, ADDTIMEOUTATMAXSIZE, retry);

		pt::message* message = 0 == POOLSIZE ? 0 : idle->getmessage(TIMEOUT);
		POOLSIZE = 1;

		if (0 == message)
		{
			if (TIMEOUT != ADDTIMEOUTATMAXSIZE) 
			{
				int BATCHSIZE;
				pt::pexchange(&BATCHSIZE, batchsize);

				enlarge(size() + BATCHSIZE);	

				TIMEOUT = 2000;
			}
			--retry;
		}
		else
		{
			thread = (ThreadImpl*) message->param;
			delete message;
		}
	}

	requests->post(new pt::message(BUSY, 0));

	return thread;
}

//********************************************************************

void _threadpool::makeIdle(ThreadImpl* thread)
{
	idle->post(new pt::message(IDLE, (pt::pintptr) thread));
}

//********************************************************************

void _threadpool::makeReadyForDelete(ThreadImpl* thread)
{
	readyfordelete->post(new pt::message(DESTROY, (pt::pintptr) thread)); 
}

//********************************************************************

void _threadpool::enlarge(int targetpoolsize)
{
	int BATCHSIZE;
	pt::pexchange(&BATCHSIZE, batchsize);
	while (0 != targetpoolsize % BATCHSIZE) ++targetpoolsize;

	if (0 < targetpoolsize)
	{
		int n = requests->get_count();
		while (0 < requests->get_count())
		{
			delete requests->getmessage(0);
		}
		requests->posturgent(new pt::message(ENLARGE, targetpoolsize));
	}
}

//********************************************************************

void _threadpool::remove(int delta)
{
	try 
	{
		if (0 < delta)
		{
			requests->post(new pt::message(REMOVE, delta));
		}
		else if (0 > delta)
		{
			requests->posturgent(new pt::message(REMOVE, delta));

			int DESTROYTIMEOUT;
			pt::pexchange(&DESTROYTIMEOUT, destroytimeout / 2);
			int ELAPSED = 0; 
			const int INTERVAL = 100;

			int i = 0;
			while (0 < size() && ELAPSED < DESTROYTIMEOUT)
			{
				Thread::sleep(INTERVAL);
				ELAPSED += INTERVAL;
			}
		}
	}
	catch (...)
	{}
}

//********************************************************************

int _threadpool::size() const
{
	int currentsize;
	pt::pexchange(&currentsize, poolsize);
	return currentsize;
}

//********************************************************************

void _threadpool::setMinSize(int minsize)
{
	pt::pexchange(&this->minsize, minsize);

	int currentsize = threadpool()->size();

	if (currentsize < minsize)
		threadpool()->enlarge(minsize);
}

//********************************************************************

void _threadpool::setMaxSize(int maxsize)
{
	pt::pexchange(&this->maxsize, maxsize);
}

//********************************************************************

void _threadpool::setBatchSize(int batchsize)
{
	pt::pexchange(&this->batchsize, batchsize);
}

//********************************************************************

void _threadpool::setAddTimeout(int addtimeout)
{
	pt::pexchange(&this->addtimeout, addtimeout);
}

//********************************************************************

void _threadpool::setAddTimeoutAtMaxSize(int addtimeoutatmaxsize) 
{
	pt::pexchange(&this->addtimeoutatmaxsize, addtimeoutatmaxsize);
}

//********************************************************************

void _threadpool::setRemoveTimeout(int removetimeout)
{
	pt::pexchange(&this->removetimeout, removetimeout);
}

//********************************************************************

void _threadpool::setDestroyTimeout(int destroytimeout)
{
	pt::pexchange(&this->destroytimeout, destroytimeout);
}

//********************************************************************
//********************************************************************

_threadpool::_adminthread::~_adminthread() 
{
	owner = 0;
}

//********************************************************************

_threadpool::_adminthread::_adminthread() 
: pt::thread(false)
{}

//********************************************************************

void _threadpool::_adminthread::setOwner(_threadpool* owner)
{
	this->owner = owner;
}

//********************************************************************

std::string now()
{
	const char* DEFAULTFMT = "%Y.%m.%d %H:%M:%S";
	char buf[1024];
	int dummy, msec;
	pt::datetime d = pt::now(true);
	pt::decodetime(d, dummy, dummy, dummy, msec);
	sprintf(buf, "%s:%03d", pt::pconst(pt::dttostring(d, DEFAULTFMT)), msec);
	return buf;
}

//********************************************************************

void _threadpool::_adminthread::execute()
{
	bool aborted = false;

	while (!aborted || (0 < owner->pool.size()))
	{
		try
		{
			int REMOVETIMEOUT;
			pt::pexchange(&REMOVETIMEOUT, owner->removetimeout);

			owner->monitor->post(new pt::message(WAITING, 0));

			pt::message* request = owner->requests->getmessage(REMOVETIMEOUT);		
			if (owner->logging) fprintf(stdout, "threadpool::adminthread::execute() : getmessage(%s) : id : %d\n", now().c_str(), (0 != request ? request->id : 0));

			owner->monitor->post(new pt::message(WORKING, 0));

			if (0 != request)
			{
				switch (request->id)
				{
				default:
				case _threadpool::ABORT:
					{		
						if (aborted) break;

						int j = 0;
						for (std::list<ThreadImpl*>::iterator i = owner->pool.begin(); i != owner->pool.end(); ++i, ++j)
						{
							owner->makeReadyForDelete(*i);
						}

						if (0 < j) owner->requests->post(new pt::message(DESTROY, 0));

						aborted = true;

						break;
					}
				case _threadpool::ENLARGE:
					{
						if (aborted) break;

						const int n = request->param;

						int MAXSIZE;
						pt::pexchange(&MAXSIZE, owner->maxsize);

						int size = owner->size();

						while (size <= n)
						{
							if (size < n && size < MAXSIZE)
							{
								ThreadImpl* thread = (ThreadImpl*) ::operator new(sizeof(ThreadImpl));

								int ID = pt::pincrement(&owner->id);
								thread = new (thread) ThreadImpl(ID);						

								// only this thread is allowed to call _start and waitfor

								thread->_start();

								owner->pool.push_front(thread);
								pt::pexchange(&owner->poolsize, ++size);
								if (owner->logging) fprintf(stdout, "threadpool::adminthread::execute() : poolsize(+) : %d\n", size);

								pt::pexchange(&owner->addtimeout, owner->initialaddtimeout);

								owner->makeIdle(thread);							
							}
							else if (size == MAXSIZE)
							{
								// wait for a much longer time when maxsize is reached 

								int ADDTIMEOUTATMAXSIZE;
								pt::pexchange(&ADDTIMEOUTATMAXSIZE, owner->addtimeoutatmaxsize);

								pt::pexchange(&owner->addtimeout, ADDTIMEOUTATMAXSIZE);

								if (owner->logging) fprintf(stdout, "threadpool::adminthread::execute() : poolsize == maxsize : %d\n", MAXSIZE);

								break;
							}
							else // ugly
							{
								break;
							}
						}

						break;
					}
				case _threadpool::REMOVE:
					{
						if (aborted) break;

						int n = request->param;
						int MINSIZE = 0;

						if (0 < n) pt::pexchange(&MINSIZE, owner->minsize);

						int size = owner->size();
						if (MINSIZE < owner->size())
						{
							int i = 0;
							while (0 > n ? true : i < n)
							{
								pt::message* message = owner->idle->getmessage(0);

								if (0 != message)
								{
									pt::pexchange(&owner->addtimeout, owner->initialaddtimeout);

									ThreadImpl* thread = (ThreadImpl*) message->param; 
									delete message;

									owner->makeReadyForDelete(thread);
								}
								else
								{
									break; // while
								}

								++i;
							}

							if (0 < i) owner->requests->post(new pt::message(DESTROY, 0));
						}		

						break;
					}
				case _threadpool::DESTROY:
					{
						int j = 0;
						while (true)
						{
							pt::message* message = owner->readyfordelete->getmessage(0);

							if (0 != message)
							{
								ThreadImpl* thread = (ThreadImpl*) message->param; 
								delete message;

								// now wait for the thread to finish

								if (!thread->isSignaled())
								{
									thread->signal();
									owner->makeReadyForDelete(thread); 
								}
								else
								{
									thread->finalize();

									// if it is not yet destroyed we rather put it back into the idle queue

									if (thread->isDestroyed())
									{
										// remove it from the pool

										owner->pool.remove(thread);
										pt::pexchange(&owner->poolsize, owner->pool.size());

										if (owner->logging) fprintf(stdout, "threadpool::adminthread::execute() : poolsize(-) : %d\n", owner->pool.size());

										// then really delete it

										thread->~ThreadImpl();
										::operator delete(thread); thread = 0;
									}
									else
									{
										thread->pseudoDestruct();
										owner->makeReadyForDelete(thread);		
									}
								}
							}
							else
							{
								break; //while
							}
						}

						break;
					}
				case _threadpool::BUSY:
					{
						// nothing to do, just to avoid REMOVE while the pool is busy

						break;
					}
				}

				delete request;
			}
			else
			{
				if (aborted) break;

				// woken up without a message, will try to remove some threads

				int MINSIZE;
				pt::pexchange(&MINSIZE, owner->minsize);
				int CURRENTSIZE;
				pt::pexchange(&CURRENTSIZE, owner->poolsize);
				if (MINSIZE < CURRENTSIZE)
				{
					if (0 < owner->idle->get_count())
					{
						int BATCHSIZE;
						pt::pexchange(&BATCHSIZE, owner->batchsize);
						BATCHSIZE = BATCHSIZE < (CURRENTSIZE - MINSIZE) ? BATCHSIZE : (CURRENTSIZE - MINSIZE);

						owner->requests->post(new pt::message(REMOVE, BATCHSIZE));
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			fprintf(stderr, "_threadpool::_adminthread::execute() : %s\n", e.what());
		}
		catch (...)
		{
			fprintf(stderr, "_threadpool::_adminthread::execute() : unknown exception\n");
		}
	}
}

//********************************************************************
//********************************************************************

_threadpool::_monitorthread::~_monitorthread() 
{
	owner = 0;
}

//********************************************************************

_threadpool::_monitorthread::_monitorthread() 
: pt::thread(false)
{}

//********************************************************************

void _threadpool::_monitorthread::setOwner(_threadpool* owner)
{
	this->owner = owner;
}

//********************************************************************

void _threadpool::_monitorthread::execute()
{
	bool aborted = false;
	bool iswaiting = true;
	int count = 0;

	while (!aborted)
	{
		try
		{
			int REMOVETIMEOUT;
			pt::pexchange(&REMOVETIMEOUT, owner->removetimeout);

			pt::message* request = owner->monitor->getmessage(3 * REMOVETIMEOUT);

			if (0 != request)
			{
				switch (request->id)
				{
				default:
				case _threadpool::ABORT:
					{		
						aborted = true;
						break;
					}
				case _threadpool::WAITING:
					{	
						iswaiting = true;
						break;
					}
				case _threadpool::WORKING:
					{		
						iswaiting = false;
						break;
					}
				}
				delete request;
			}
			else
			{
				// error situation only if the adminthread is not waiting

				if (!iswaiting)
				{
					++count;
					if (0 < count)
					{
						fprintf(stderr, "threadpool::monitorthread::execute() : program will be killed now, count = %d\n", count);
						exit(-1);
					}
					else
					{
						fprintf(stderr, "threadpool::monitorthread::execute() : adminthread not responding, count = %d\n", count);
					}
					iswaiting = true;
				}
			}
		}
		catch (const std::exception& e)
		{
			fprintf(stderr, "_threadpool::_monitorthread::execute() : %s\n", e.what());
		}
		catch (...)
		{
			fprintf(stderr, "_threadpool::_monitorthread::execute() : unknown exception\n");
		}
	}
}

//********************************************************************
// 
// T h r e a d I m p l
// 
//********************************************************************

//********************************************************************

ThreadImpl::~ThreadImpl() 
{}

//********************************************************************

ThreadImpl::ThreadImpl(int id) 
: running(0)
, interrupted(0)
, constructed(0)
, id(id)
, startjobs(new pt::jobqueue())
, stopjobs(new pt::jobqueue())
{
	thread = new threadimpl(this);
}

//********************************************************************

void* ThreadImpl::operator new(size_t, ThreadImpl* p)
{
	return p;
}

//********************************************************************

void ThreadImpl::pseudoConstruct(Runnable runnable) 
{
	this->runnable = runnable;
	pt::pexchange(&this->running, 0);
	pt::pexchange(&this->interrupted, 0);
	pt::pexchange(&this->constructed, 1);
}

//********************************************************************

void ThreadImpl::pseudoConstruct(Runnable runnable, 
								 const String& name) 
{
	setName(name);
	this->runnable = runnable;
	pt::pexchange(&this->running, 0);
	pt::pexchange(&this->interrupted, 0);
	pt::pexchange(&this->constructed, 1);
}

//********************************************************************

void ThreadImpl::pseudoDestruct() 
{
	pt::pexchange(&this->constructed, 0);
}

//********************************************************************

void ThreadImpl::finalize() 
{
	// rather wait for the destruction until isDestroyed is true

	if (!isDestroyed() || isAlive()) 
		return; 

	waitFor();

	delete thread; 
	thread = 0;

	delete startjobs;
	startjobs = 0;

	delete stopjobs;
	stopjobs = 0;		
}

//********************************************************************

bool ThreadImpl::isAlive() const 
{
	int RUNNING;
	pt::pexchange(&RUNNING, this->running);
	return 1 == RUNNING;
}

//********************************************************************

void ThreadImpl::setAlive(bool alive)
{
	pt::pexchange(&this->running, alive ? 1 : 0);
}

//********************************************************************

bool ThreadImpl::isInterrupted() const 
{
	int INTERRUPTED;
	pt::pexchange(&INTERRUPTED, this->interrupted);
	return 1 == INTERRUPTED;
}

//********************************************************************

void ThreadImpl::interrupt() 
{
	pt::pexchange(&this->interrupted, 1);
}

//********************************************************************

void ThreadImpl::join(int timeout) throw (InterruptedException)
{
	if (isAlive())
	{
		pt::message* job = stopjobs->getmessage(timeout);

		if (0 != job)
		{
			switch (job->id)
			{
			case ABORT:
			case HASSTARTED:
			case JOIN:
			default:
				{
					delete job;
					if (isInterrupted())
					{
						pt::pexchange(&this->interrupted, 0);
						throw InterruptedException(L"ThreadImpl::join() was interrupted.");
					}
					break;
				}
			}
		}
		else
		{
			throw InterruptedException(L"ThreadImpl::join() timed out.");
		}
	}
}

//********************************************************************

void ThreadImpl::join() throw (InterruptedException)
{
	join(JOBTIMEOUT);
}

//********************************************************************

void ThreadImpl::signal() 
{
	startjobs->posturgent(ABORT);
	thread->signal();
	interrupt();
	Thread::yield();
}

//********************************************************************

void ThreadImpl::waitFor() 
{
	thread->waitfor();
}

//********************************************************************

bool ThreadImpl::isDestroyed() const
{
	int CONSTRUCTED;
	pt::pexchange(&CONSTRUCTED, this->constructed);
	return 0 == CONSTRUCTED;
}

//********************************************************************

bool ThreadImpl::isSignaled() const
{
	return 1 == thread->get_signaled();
}

//********************************************************************

String ThreadImpl::getName() const 
{
	return name->clone();
}

//********************************************************************

void ThreadImpl::setName(const String& n) 
{
	name = n->clone();
}

//********************************************************************

Runnable ThreadImpl::getRunnable() 
{
	return runnable;
}

//********************************************************************

void ThreadImpl::start() 
{
	if (thread->get_running())
	{
		startjobs->post(START);

		pt::message* job = stopjobs->getmessage(JOBTIMEOUT);

		if (0 != job)
		{
			switch (job->id)
			{
			case ABORT:
			case HASSTARTED:
			case JOIN:
			default:
				{
					break;
				}
			}
			delete job;
		}
	}
}

//********************************************************************

void ThreadImpl::_start() 
{
	thread->start();
}

//********************************************************************

void ThreadImpl::run()
{
	bool aborted = false; 

	while (!aborted)
	{
		try
		{
			pt::message* job = startjobs->getmessage(JOBTIMEOUT);

			if (0 != job)
			{
				switch (job->id)
				{
				default:
				case ABORT:
					{				
						aborted = true;
						break;
					}
				case START:
					{
						if (0 != runnable.get())
						{						
							setAlive(true);
							stopjobs->post(HASSTARTED);

							runnable->run();	

							// let a runnable wanting to join join, note: this is not waitFor

							stopjobs->post(JOIN);

							runnable->resetCurrentThread();
							runnable.reset(0);

							// not running anymore, must be reset after resetting runnable

							setAlive(false);	
						}
						// else ignore

						threadpool()->makeIdle(this); // safe to reuse this thread

						break;
					}
				}						
				delete job;
			}
		}
		catch (const Exception& e)
		{
			fprintf(stderr, "ThreadImpl::run() : Should be caught earlier: %s\n", constcharptr(e->toString()));
		}
		catch (const std::exception& e)
		{
			fprintf(stderr, "ThreadImpl::run() : std exception: %s\n", e.what());
		}
		catch (...)
		{
			fprintf(stderr, "ThreadImpl::run() : unknown exception\n");
		}
	}

	stopjobs->posturgent(ABORT);
}

//********************************************************************

void ThreadImpl::createPool(int	minsize,
							int maxsize,
							int batchsize,
							int addtimeout,
							int addtimeoutatmaxsize,
							int removetimeout, 
							int destroytimeout)	throw (NullPointerException)   

{
	if (0 == threadpool()) throw NullPointerException(WITHDETAILS(L"ThreadPool not initialized."));

	threadpool()->setMinSize(minsize);
	threadpool()->setMaxSize(maxsize);
	threadpool()->setBatchSize(batchsize);
	threadpool()->setAddTimeout(addtimeout);
	threadpool()->setAddTimeoutAtMaxSize(addtimeoutatmaxsize);
	threadpool()->setRemoveTimeout(removetimeout);
	threadpool()->setDestroyTimeout(destroytimeout);
}

//********************************************************************

void ThreadImpl::destroyPool()
{
	try
	{
		if (0 != threadpool())
		{
#if defined(_AIX)

			// better to do nothing

#else
			int size = threadpool()->size();

			if (0 == size) 
			{
				delete threadpool();
				threadpool() = 0;
			}
			else
			{
				int c = 0;
				while (0 < threadpool()->size() && c < 10)
				{
					++c;
					threadpool()->remove(-1);
					Thread::sleep(100);
				}
				size = threadpool()->size();

				if (0 == size)
				{
					delete threadpool();
					threadpool() = 0;
				}
				else
				{
					int beforesize = size;
					threadpool()->remove(-1);
					Thread::sleep(2000);
					size = threadpool()->size();

					if (0 == size)
					{
						delete threadpool();
						threadpool() = 0;
					}
					else
					{
						fprintf(stderr, "Thread::destroyPool() : before/after = %d/%d\n", beforesize, size);
						fprintf(stderr, "Thread::destroyPool() : need to abort hosting program now\n");
						exit(-1);
					}
				}
			}
#endif		
		}
	}
	catch (...)
	{}
}

//********************************************************************
// 
// T h r e a d I m p l : : t h r e a d i m p l
// 
//********************************************************************

ThreadImpl::threadimpl::~threadimpl() 
{
	owner = 0;
}

//********************************************************************

ThreadImpl::threadimpl::threadimpl(ThreadImpl* owner) 
: pt::thread(false)
, owner(owner)
{}

//********************************************************************

void ThreadImpl::threadimpl::execute() 
{
	owner->run();
}

//********************************************************************
// 
// T h r e a d I
// 
//********************************************************************

ThreadI::~ThreadI()
{
	// it must be ensured that delegates live longer than ThreadI objects
	// ie. Thread::destroyPool() must always be called after ThreadI objects have been deleted

	if (0 != delegate)
	{
		delegate->pseudoDestruct();
		delegate = 0;
	}
}

//********************************************************************

ThreadI::ThreadI(Runnable runnable) throw (NullPointerException)
: delegate(threadpool()->nextIdle()) 
{
	if (0 == delegate) 
	{
		throw NullPointerException(L"ThreadI::ThreadI() : 0 == delegate (will not run)\nThe ADDTIMEOUTATMAXSIZE or ThreadPool::MAXSIZE should be increased.");
	}
	else
	{
		delegate->pseudoConstruct(runnable);
	}
}

//********************************************************************

ThreadI::ThreadI(Runnable runnable,	const String& name) throw (NullPointerException)
: delegate(threadpool()->nextIdle())
{
	if (0 == delegate) 
	{
		throw NullPointerException(L"ThreadI::ThreadI() : 0 == delegate (will not run)\nThe ADDTIMEOUTATMAXSIZE or ThreadPool::MAXSIZE should be increased.");
	}
	else 
	{
		delegate->pseudoConstruct(runnable, name);
	}
}

//********************************************************************

bool ThreadI::isAlive() const 
{
	if (0 != delegate)
	{
		return delegate->isAlive();
	}
	else
	{	
		return false;
	}
}

//********************************************************************

bool ThreadI::isInterrupted() const 
{
	if (0 != delegate)
	{
		return delegate->isInterrupted();
	}
	else
	{
		return false;
	}
}

//********************************************************************

void ThreadI::interrupt() 
{
	if (0 != delegate) delegate->interrupt(); 
}

//********************************************************************

void ThreadI::join() throw (InterruptedException)
{
	if (0 != delegate) delegate->join();
}

//********************************************************************

void ThreadI::start() 
{
	if (0 != delegate) 
	{
		Thread thread = this;
		delegate->getRunnable()->setCurrentThread(thread);
		delegate->start(); 
	}
}

//********************************************************************

String ThreadI::getName() const 
{
	if (0 != delegate)
	{
		return delegate->getName();
	}
	else
	{
		return L"<delegate is null>";
	}
}

//********************************************************************

void ThreadI::run() 
{
	if (0 != delegate) delegate->run();
}

//********************************************************************
// 
// T h r e a d
// 
//********************************************************************

Thread Thread::currentThread() throw (UnsupportedOperationException) 
{
	throw UnsupportedOperationException(L"Thread::currentThread() is not implemented, please see AbstractRunnable for details.");

	static Thread nonethread;
	return nonethread;
}

//********************************************************************

void Thread::sleep(int millis) throw (InterruptedException)
{
	if (0 <= millis) 
	{
		try 
		{
			pt::psleep(millis);
		}
		catch (std::exception&)
		{
			throw InterruptedException(L"Thread::sleep(int millis) was interrupted.");
		}
		catch (...)
		{
			throw InterruptedException(L"Thread::sleep(int millis) was interrupted.");
		}
	}
}

//********************************************************************

void Thread::yield() 
{
#ifdef WIN32
	pt::psleep(1);
#else
	sched_yield();	
#endif
}

//********************************************************************

void Thread::createPool(int minsize,
						int maxsize,
						int batchsize,
						int addtimeout,
						int addtimeoutatmaxsize,
						int removetimeout,
						int destroytimeout)	throw (NullPointerException)

{
	if (0 >= minsize) minsize = threadpool()->DEFAULT_MINSIZE;
	if (0 >= maxsize) maxsize = threadpool()->DEFAULT_MAXSIZE;
	if (0 >= batchsize) batchsize = threadpool()->DEFAULT_BATCHSIZE;
	if (0 >= addtimeout) addtimeout = threadpool()->DEFAULT_ADDTIMEOUT;
	if (0 >= addtimeoutatmaxsize) addtimeoutatmaxsize = threadpool()->DEFAULT_ADDTIMEOUTATMAXSIZE;
	if (0 >= removetimeout) removetimeout = threadpool()->DEFAULT_REMOVETIMEOUT;
	if (0 >= destroytimeout) destroytimeout = threadpool()->DEFAULT_DESTROYTIMEOUT;

	ThreadImpl::createPool(
		minsize,
		maxsize,
		batchsize,
		addtimeout,
		addtimeoutatmaxsize,
		removetimeout,
		destroytimeout);
}

//********************************************************************

void Thread::destroyPool() 
{
	// a NOOP
}

//********************************************************************

void initializeThreadPool() 
{
	if (0 == threadpool())
	{
		threadpool() = new _threadpool();
	}
}

//********************************************************************

void finalizeThreadPool() 
{
	ThreadImpl::destroyPool();
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
