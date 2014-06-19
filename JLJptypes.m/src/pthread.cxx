/*
*
*  C++ Portable Types Library (PTypes)
*  Version 2.1.1  Released 27-Jun-2007
*
*  Copyright (C) 2001-2007 Hovik Melikyan
*
*  http://www.melikyan.com/ptypes/
*
*/

#ifdef _WIN32
#  include <process.h>
#else
#  include <pthread.h>
#  include <stdlib.h>
#endif

#include "pasync.h"
#include <stdio.h>


PTYPES_BEGIN


thread::thread(bool iautofree)
:
#ifdef _WIN32
id(0),
#endif
handle(0), autofree(iautofree),
running(0), signaled(0), finished(0), freed(0),
reserved(0), relaxsem(0)
{}


thread::~thread()
{
	if (pexchange(&freed, 1) != 0)
		return;
#ifdef _WIN32
	if (autofree)
		// MSDN states this is not necessary, however, without closing
		// the handle debuggers show an obvious handle leak here
		CloseHandle(handle);
#else
	// though we require non-autofree threads to always call waitfor(),
	// the statement below is provided to cleanup thread resources even
	// if waitfor() was not called.
	if (!autofree && running)
		pthread_detach(handle);
#endif
}


void thread::signal()
{
	if (pexchange(&signaled, 1) == 0)
		relaxsem.post();
}


void thread::waitfor()
{
	if (pexchange(&freed, 1) != 0)
		return;
	if (pthrequal(get_id()))
		fatal(CRIT_FIRST + 47, "Can not waitfor() on myself");
	if (autofree)
		fatal(CRIT_FIRST + 48, "Can not waitfor() on an autofree thread");
	// 20090311, Guido Schmidt : INFINITE is not nice in production systems
	// 20090311, Guido Schmidt : if it fails to join the thread after some time it is better to be killed
#ifdef _WIN32
	const int MAXIMUMTIMEOUT = 200;
	int MAXRETRIES = 100;
	while (0 == finished && 0 < --MAXRETRIES)
	{
		if (0 == MAXRETRIES % 10) 
		{
			if (50 > MAXRETRIES)
			{
				fprintf(stderr, "thread::waitfor() : WaitForSingleObject() (retries : %d)\n", MAXRETRIES);
			}
			pt::psleep(MAXIMUMTIMEOUT);
		}
		WaitForSingleObject(handle, MAXIMUMTIMEOUT);
	}
	if (0 == finished && 0 == MAXRETRIES)
	{
		fprintf(stderr, "thread::waitfor() : WaitForSingleObject() failed. Need to exit process.\n");	
		exit(-1);
	}
	CloseHandle(handle);
#else
	int rc = -1;
	const int MAXIMUMTIMEOUT = 200;
	int MAXRETRIES = 100;
	while (0 == finished && 0 < --MAXRETRIES)
	{
		if (-1 == rc) rc = pthread_join(handle, nil);
		if (0 == MAXRETRIES % 10) 
		{
			pt::psleep(MAXIMUMTIMEOUT);
			if (50 > MAXRETRIES)
			{
				fprintf(stderr, "pthread_join() : retries : %d\n", MAXRETRIES);
			}
		}
	}
	if (0 == finished && 0 == MAXRETRIES)
	{
		fprintf(stderr, "thread::waitfor() : pthread_join() failed. Need to exit process.\n");	
		exit(-1);
	}
	//  detaching after 'join' is not required (or even do harm on some systems)
	//  except for HPUX. we don't support HPUX yet.
	if (1 == finished && 0 != rc) 
	{
		rc = pthread_detach(handle);
	}
#endif
	handle = 0;
}

// 20090311, Guido Schmidt : on SMP machines the "this" pointer must be passed into a newly created threadproc safely via a messagequeue
static jobqueue threadqueue;

class synchronizedmessage
	: public message
{
public:
	synchronizedmessage(thread* t) 
		: message(1) 
	{
		scopelock scope(lock);
#if defined(__HP_aCC) || defined(_AIX)
		param = (unsigned long) t;
#else
		param = (pintptr) t;
#endif
	}

	pintptr getParam() 
	{
		scopelock scope(lock);
		return param;
	}

private:
	mutex lock;
};

#ifdef _WIN32
unsigned _stdcall _threadproc(void*)
{
#else
void* _threadproc(void*) 
{
#endif
	const int THREADSTARTTIMEOUT = 30*1000;
	synchronizedmessage* msg = (synchronizedmessage*) threadqueue.getmessage(THREADSTARTTIMEOUT);
	thread* thr = 0;
	if (0 != msg)
	{
		thr = (thread*) msg->getParam();
		delete msg;
	}
	if (0 != thr)
	{
#ifndef _WIN32
		if (thr->autofree)
			// start() does not assign the handle for autofree threads
			thr->handle = pthread_self();
#endif
		try 
		{
			thr->execute();
		}
		catch(...)
		{
			_threadepilog(thr);
			throw;
		}
		_threadepilog(thr);
	}
#ifdef _WIN32
	_endthreadex(0);
#elif defined(__sun__)
	pthread_exit(0);
#endif
	return 0;
}


void _threadepilog(thread* thr)
{
	if (0 == thr) return;
	try
	{
		thr->cleanup();
	}
	catch(exception* e)
	{
		delete e;
	}
	pexchange(&thr->finished, 1);
	if (thr->autofree)
		delete thr;
}


void thread::start()
{
	if (pexchange(&running, 1) == 0)
	{
		threadqueue.post(new synchronizedmessage(this));
#ifdef _WIN32
		handle = (HANDLE)_beginthreadex(nil, 0, _threadproc, nil, 0, &id);
		if (handle == 0)
			fatal(CRIT_FIRST + 40, "CreateThread() failed");
#else
		const int STACKSIZE = 128 * 1024; // important for AIX
		pthread_t temp_handle;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, STACKSIZE);
		pthread_attr_setdetachstate(&attr, 
			autofree ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
		if (pthread_create(autofree ? &temp_handle : &handle,
			&attr, _threadproc, nil) != 0)
			fatal(CRIT_FIRST + 40, "pthread_create() failed");
		pthread_attr_destroy(&attr);
#endif
	}
}


void thread::cleanup()
{
}


PTYPES_END
