// jmstest.h

//********************************************************************

#ifndef jmstools_jmstest_h
#define jmstools_jmstest_h

//********************************************************************

#include <JMSToolstests.h>
#include <jlj/lang/Object.h>
#include <jlj/util/logging/Logger.h>

using namespace jlj::util::logging;
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

class  ExportedByJMSToolstests JMSTestI
	: public ObjectI

{
public:
	~JMSTestI();
	JMSTestI();
	JMSTestI(
		const String& type, 
		const String& path, 
		int outerloop, 
		int submitloop, 
		int innerloop, 
		int min_blocksize, 
		int max_blocksize);

	void RequestConnection();
	void ReplyConnection();

private:

	void RequestSimpleConnection();

	Logger logger;
	String type;
	String path;
	int outerloop;
	int submitloop;
	int innerloop;
	int min_blocksize;
	int max_blocksize;

};

//********************************************************************

StrongReferenceCommonDeclaration(JMSTest, ExportedByJMSToolstests);

//********************************************************************

NAMESPACE_END(jmstools)

#endif
// eof