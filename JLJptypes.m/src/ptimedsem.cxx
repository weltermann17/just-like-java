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
#  include <sys/time.h>
#  include <pthread.h>
#  include <errno.h>
#endif

#include "pasync.h"


PTYPES_BEGIN


static void tsem_fail()
{
    fatal(CRIT_FIRST + 41, "Timed semaphore failed");
}


#ifdef _WIN32


timedsem::timedsem(int initvalue)
{
// GSM: why not use the maximum possible?
    handle = CreateSemaphore(nil, initvalue, INT_MAX, nil);
    if (handle == 0)
        tsem_fail();
}


timedsem::~timedsem() 
{
    CloseHandle(handle);
}


bool timedsem::wait(int timeout)
{
    uint r = WaitForSingleObject(handle, timeout);
    if (r == WAIT_FAILED)
        tsem_fail();
    return r != WAIT_TIMEOUT;
}


void timedsem::post()
{
    if (ReleaseSemaphore(handle, 1, nil) == 0)
        tsem_fail();
}


#else


inline void tsem_syscheck(int r)
{
    if (r != 0)
        tsem_fail();
}


timedsem::timedsem(int initvalue)
    : unknown(), count(initvalue)
{
    tsem_syscheck(pthread_mutex_init(&mtx, 0));
    tsem_syscheck(pthread_cond_init(&cond, 0));
}


timedsem::~timedsem()
{
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mtx);
}


bool timedsem::wait(int timeout)
{
    pthread_mutex_lock(&mtx);
    while (count <= 0)
    { 
        if (timeout >= 0)
        {
            timespec abs_ts; 
            timeval cur_tv;
            gettimeofday(&cur_tv, NULL);
            abs_ts.tv_sec = cur_tv.tv_sec + timeout / 1000; 
            abs_ts.tv_nsec = cur_tv.tv_usec * 1000
                + (timeout % 1000) * 1000000;
            /*
             * The nanoseconds may have rolled past one second; correct for
             * this condition.  The overflow of tv_nsec is safe, because
             * its upper limit will be at least 2^31-1.  This is because
             * POSIX defines it as a long, which ISO C guarantees will be
             * at least 32-bits wide.
             */
            abs_ts.tv_sec += abs_ts.tv_nsec / 1000000000;
            abs_ts.tv_nsec %= 1000000000;
            int rc = pthread_cond_timedwait(&cond, &mtx, &abs_ts);
            if (rc == ETIMEDOUT) { 
                pthread_mutex_unlock(&mtx);
                return false;
            }
        }
        else
            pthread_cond_wait(&cond, &mtx);
    } 
    count--;
    pthread_mutex_unlock(&mtx);
    return true;
} 


void timedsem::post()
{
    pthread_mutex_lock(&mtx);
    count++; 
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
}


#endif


PTYPES_END
