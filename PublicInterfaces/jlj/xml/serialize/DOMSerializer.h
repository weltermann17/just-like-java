//DOMSerializer.h

//********************************************************************

#ifndef jlj_xml_serialize_DOMSerializer_h
#define jlj_xml_serialize_DOMSerializer_h

//********************************************************************

#include <jlj/xml/Typedef.h>
#include <jlj/xml/serialize/OutputFormat.h>

//********************************************************************

#include <jlj/lang/Throwable.h>
#include <jlj/lang/String.h>
#include <jlj/util/List.h>
#include <jlj/io/Exceptions.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//********************************************************************

class ExportedByJLJxml DOMSerializerI
    : public ObjectI
{
public:
    virtual ~DOMSerializerI() = 0;
    DOMSerializerI();	

public:
	virtual void serialize(Node) throw (jlj::io::IOException) = 0;
	virtual void serialize(const jlj::util::List<Node>&) throw (jlj::io::IOException) = 0;	
    
	virtual void setOutputCharStream(const String&) = 0; 
    virtual void setOutputFormat(const OutputFormat&) = 0; 

};

//********************************************************************

StrongReferenceCommonDeclaration(DOMSerializer, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof


