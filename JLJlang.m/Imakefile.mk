# COPYRIGHT Dassault Systemes 2007
#======================================================================
# Imakefile for module JLJlang.m
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
JLJptypes \
JLJcore
# END WIZARD EDITION ZONE

LINK_WITH = $(WIZARD_LINK_MODULES)

# System dependant variables
#
OS = AIX
LOCAL_POST_CCFLAGS = \
-DNDEBUG -D__JLJlang -DU3 -O5 -qarch=auto -qthreaded -qinline -qstaticinline \
-qnotemplateregistry -qeh=v6 -qrtti
LOCAL_LDFLAGS = -qipa -lpthreads -liconv
#
OS = HP-UX
#
OS = IRIX
#
OS = SunOS
#
OS = Windows_NT
LOCAL_POST_CCFLAGS = \
-DNDEBUG -D__JLJlang -DU3 -GR -EHsc 
LOCAL_LDFLAGS = 
