// SessionDirectory.cpp
//********************************************************************

#ifndef SessionDirectory_cpp
#define SessionDirectory_cpp

//********************************************************************

#include <jlj/util/SessionDirectory.h>
#include <jlj/util/Uuid.h>
#include <jlj/lang/UTF8StringBuffer.h>
#include <stdio.h>
#include <signal.h>
#if defined(_WINDOWS_SOURCE) 
#include <direct.h>
#endif
#include <sys/stat.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

#if defined(_WINDOWS_SOURCE)
void createProcess(const UTF8String& commandline)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char cmdline[8*1024];
	memset(cmdline, 0, 8*1024);
	memcpy(cmdline, constcharptr(commandline), commandline->length()); 

	// Start the child process. 
	if( !CreateProcessA( getenv("COMSPEC"),   // No module name (use command line)
		cmdline,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		fprintf(stdout, "createProcess failed : %d : %s\n", GetLastError(), cmdline);
		return;
	}

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}
#endif

//********************************************************************

static char sessiondirectorypath[2*1024];

//********************************************************************

void removeSessionDirectoryAtExit()
{
	try
	{
		if (0 < strlen(sessiondirectorypath))
		{
			UTF8StringBuffer commandline;
#if defined(_WINDOWS_SOURCE) 
			commandline->append(getenv("COMSPEC"));
			commandline->append(" /c c:\\windows\\sleep 15 & rmdir /q /s \"");
			commandline->append(sessiondirectorypath);
			commandline->append("\"");
			createProcess(commandline->toUTF8String());
#else
			commandline->append("rm -rf \"");
			commandline->append(sessiondirectorypath);
			commandline->append("\" 1> /dev/null 2>@1");
			system(constcharptr(commandline));
#endif
			fprintf(stderr, "sessiondirectory removed: %s\n", constcharptr(commandline)); fflush(stderr);
			strcpy(sessiondirectorypath, "");
		}
	}
	catch (...)
	{}
}

//********************************************************************

SessionDirectoryI::~SessionDirectoryI() 
{
	removeSessionDirectoryAtExit();
}

//********************************************************************

SessionDirectoryI::SessionDirectoryI() throw (Exception)
{
#if defined(_WINDOWS_SOURCE) 
	const String separator = L"\\";
#else
	const String separator = L"/";
#endif

	String tempdirectory;

#if defined(_WINDOWS_SOURCE) 
	const char* tmp = ::getenv("TEMP");
	if (0 != tmp) tempdirectory = String(tmp, strlen(tmp)); else tempdirectory = L".";
#else
	tempdirectory = 0 == access("/tmp/ramdisk", 0) ? L"/tmp/ramdisk" : L"/tmp";
#endif

	path = tempdirectory + separator + Uuid::randomUuid()->toString()->toMD5() + L".session" + separator;
#if defined(_WINDOWS_SOURCE) 
	if (0 != _mkdir(constcharptr(path->toUTF8String())))
#else
	if (0 != mkdir(constcharptr(path->toUTF8String()), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#endif
	{
		throw Exception(WITHDETAILS(L"Cannot create session directory : " + path));
	}
	strcpy(sessiondirectorypath, constcharptr(path->toUTF8String()));
	atexit(removeSessionDirectoryAtExit);
}

//********************************************************************

String SessionDirectoryI::getPath() const
{
	return path;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
