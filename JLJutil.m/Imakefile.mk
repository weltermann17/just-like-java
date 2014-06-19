# COPYRIGHT Dassault Systemes 2007
#======================================================================
# Imakefile for module JLJutil.m
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
JLJptypes \
JLJcore \
JLJlang \
JLJio
# END WIZARD EDITION ZONE

LINK_WITH = $(WIZARD_LINK_MODULES)

# System dependant variables
#
OS = AIX
LOCAL_POST_CCFLAGS = \
-DNDEBUG -D__JLJutil -DU1 -O2 -qarch=auto -qthreaded -qinline -qstaticinline \
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
-DNDEBUG -D__JLJutil -DU1 -GR -EHsc 
LOCAL_LDFLAGS = 
