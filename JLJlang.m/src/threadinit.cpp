// threadinit.cpp

//********************************************************************

#ifndef threadinit_cpp
#define threadinit_cpp

//********************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if defined (_AIX)

// GSM : on AIX multiprocessor machines we bind this process to one processor only if the env variable is set to 1

#include <errno.h>
#include <pthread.h>
#include <sys/processor.h>

#else
#include <process.h>
#endif

//********************************************************************

#include <jlj/lang/Thread.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lang)

//********************************************************************

void initializeThread()
{
	srand((unsigned) getpid());

#if defined (_AIX)
	bool bindtosinglecpu = false;

	// kind of obsolete, do not set

	char* env = getenv("JLJBindToSingleCPU");
	if (0 != env)
	{
		int value = atoi(env);
		bindtosinglecpu = 1 == value;
	}

	if (bindtosinglecpu)
	{
		const int CPUCOUNT = sysconf(_SC_NPROCESSORS_ONLN);

		if (1 < CPUCOUNT)
		{
			const int CPU = rand() % CPUCOUNT;

			fprintf(
				stdout, 
				"\n__initbindprocessor() : trying to bind to cpu %d of %d ...",
				CPU,
				CPUCOUNT);

			int result = bindprocessor(BINDPROCESS, getpid(), CPU);

			if (0 == result)
			{
				fprintf(
					stdout,
					"\n__initbindprocessor() : successfully bound, affinity set to cpu %d.",
					CPU + 1);
			}
			else
			{
				fprintf(
					stdout,
					"\n__initbindprocessor() : failed, error = %d.",
					result);
			}

			fflush(stdout);
		}
	}

	env = getenv("AIXTHREAD_SCOPE");
	bool SYSTEMSCOPE = false;
	if (0 != env)
	{
		SYSTEMSCOPE = 0 == strcmp("S", env);
	}

	if (!SYSTEMSCOPE)
	{
		fprintf(stdout, "If jlj::lang::Thread is used it is strongly recommended to 'export AIXTHREAD_SCOPE=S' \nor application behaviour may be undefined.\n");
	}

#elif defined (__sun__)

//********************************************************************

#elif defined (__hpux)

//********************************************************************

#elif defined (_WINDOWS_SOURCE)

//********************************************************************

#endif
}

//********************************************************************

NAMESPACE_END(lang)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
