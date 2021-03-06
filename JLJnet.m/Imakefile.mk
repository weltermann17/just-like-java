# COPYRIGHT Dassault Systemes 2007
#======================================================================
# Imakefile for module JLJnet.m
#======================================================================
#
#  Oct 2007  Creation: Code generated by the CAA wizard  u62xz
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
JLJutil
# END WIZARD EDITION ZONE

LINK_WITH = $(WIZARD_LINK_MODULES)

# System dependant variables
#
OS = AIX
LOCAL_POST_CCFLAGS = \
-DNDEBUG -D__JLJnet -DU2 -O2 -qarch=auto -qthreaded -qinline -qstaticinline \
-qnotemplateregistry -qeh=v6 -qrtti
LOCAL_LDFLAGS = -qipa -lpthreads 
#
OS = HP-UX
#
OS = IRIX
#
OS = SunOS
#
OS = Windows_NT
-DNDEBUG -D__JLJnet -DU3 -GR -EHsc
LOCAL_LDFLAGS = 
