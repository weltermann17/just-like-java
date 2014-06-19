# COPYRIGHT Dassault Systemes 2008
#======================================================================
# Imakefile for module JMSToolstests.m
#======================================================================
#
#  Feb 2008  Creation: Code generated by the CAA wizard  p2905
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
#
OS = AIX
LOCAL_POST_CCFLAGS = \
-I../../../../JLJ3RD/PublicInterfaces/jlj3rd \
-I../../../../JLJ3RD/PublicInterfaces \
-DNDEBUG -DU3 -O2 -qarch=auto -qthreaded -qinline -qstaticinline \
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
-I../../../../JLJ3RD/PublicInterfaces/jlj3rd \
-I../../../../JLJ3RD/PublicInterfaces \
-DNDEBUG -DU5 -GR -EHsc
LOCAL_LDFLAGS = 
