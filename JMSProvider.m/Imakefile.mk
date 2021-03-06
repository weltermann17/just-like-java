# COPYRIGHT Dassault Systemes 2007
#======================================================================
# Imakefile for module JMSProvider.m
#======================================================================
#
#  Nov 2007  Creation: Code generated by the CAA wizard  b5631
#======================================================================
#
# LOAD MODULE 
#
BUILT_OBJECT_TYPE=LOAD MODULE 
 
# DO NOT EDIT :: THE CAA2 WIZARDS WILL ADD CODE HERE
WIZARD_LINK_MODULES = \
JS0GROUP \
zlib1 \
xerces-c \
xqilla \
cppunit \
JLJptypes \
JLJcore \
JLJlang \
JLJio \
JLJnio \
JLJnet \
JLJutil \
JLJxml \
JLJjms \
JLJnaming \
JLJlifetime \
JMSreactorimplementation
# END WIZARD EDITION ZONE

LINK_WITH = $(WIZARD_LINK_MODULES)

# System dependant variables
## -O5 leads to crash in atomic operations
OS = AIX
LOCAL_POST_CCFLAGS = \
-DNDEBUG -DU3 -O3 -qarch=auto -qthreaded -qinline -qstaticinline \
-qnotemplateregistry -qeh=v6 -qrtti -qlanglvl=ansifor 
LOCAL_LDFLAGS = -qipa -lpthreads -bhalt:8 -qsuppress=1586-233:1586-231
#
OS = HP-UX
#
OS = IRIX
#
OS = SunOS
#
OS = Windows_NT
LOCAL_POST_CCFLAGS = \
-DNDEBUG -DU1 -GR -EHsc
LOCAL_LDFLAGS =

