# COPYRIGHT Dassault Systemes 2007
#======================================================================
# Imakefile for module JLJnio.m
#======================================================================
#
#  Feb 2007  Creation: Code generated by the CAA wizard  u62xz
#======================================================================
#
# SHARED LIBRARY 
#
BUILT_OBJECT_TYPE=SHARED LIBRARY 
 
# DO NOT EDIT :: THE CAA2 WIZARDS WILL ADD CODE HERE
WIZARD_LINK_MODULES = \
JS0GROUP \
zlib1 \
xerces-c \
xqilla \
JLJptypes \
JLJcore \
JLJlang \
JLJio \
JLJutil \
JLJnet
# END WIZARD EDITION ZONE

LINK_WITH = $(WIZARD_LINK_MODULES)

# System dependant variables
#
OS = AIX
LOCAL_POST_CCFLAGS = \
-DNDEBUG -D__JLJnio -DU2 -O2 -qarch=auto -qthreaded -qinline -qstaticinline \
-qnotemplateregistry -qeh=v6 -qrtti -qlanglvl=ansifor
LOCAL_LDFLAGS = -qipa -lpthreads 
#
OS = HP-UX
#
OS = IRIX
#
OS = SunOS
#
OS = Windows_NT
LOCAL_POST_CCFLAGS = \
-DNDEBUG -D__JLJnio -DU3 -GR -EHsc
LOCAL_LDFLAGS = 
SYS_LIBS = ws2_32.lib
