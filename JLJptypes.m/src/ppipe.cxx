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
#  include <unistd.h>
#endif

#include "pstreams.h"


PTYPES_BEGIN


void infile::pipe(outfile& out)
{
#ifdef _WIN32

    SECURITY_ATTRIBUTES sa;
    HANDLE h[2];

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&h[0], &h[1], &sa, 0))
#else
    int h[2];
    if (::pipe(h) != 0)
#endif
        error(uerrno(), "Couldn't create a local pipe");

    set_syshandle((pintptr) h[0]);
    peerhandle = (pintptr) h[1];
    out.set_syshandle((pintptr) h[1]);
    out.peerhandle = (pintptr) h[0];
    open();
    out.open();
}


PTYPES_END
