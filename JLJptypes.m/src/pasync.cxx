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
#  include <windows.h>
#else
#  include <stdlib.h>
#  include <unistd.h>
#  include <pthread.h>
#  if defined (__sun__)  || defined (_AIX) || defined (__HP_aCC)
#    include <poll.h>
#  endif
#endif

#include "pasync.h"
#include <stdio.h>

PTYPES_BEGIN


void ptdecl psleep(uint milliseconds)
{
	try
	{
#if defined(_WIN32)
		Sleep(milliseconds);
#elif defined (__sun__)  || defined (_AIX) || defined (__HP_aCC)
		poll(0, 0, milliseconds);		
#else
#error "Your platform does not support poll. "
#error "usleep will suspend the entire process not just the thread."
#error "Maybe your settings are wrong (or you're still on hpux 10.20)."
		usleep(milliseconds * 1000);
#endif
	}
	catch (...)
	{
		fprintf(stderr, "psleep() : InterruptedException\n"); fflush(stderr);
	}
}


pthread_id_t ptdecl pthrself() 
{
#ifdef _WIN32
	return (int) GetCurrentThreadId();
#else
	return pthread_self();
#endif
}


bool ptdecl pthrequal(pthread_id_t id)
{
#ifdef _WIN32
	return GetCurrentThreadId() == (uint)id;
#else
	return pthread_equal(pthread_self(), id);
#endif
}


PTYPES_END
