//BaseMarkupSerializer.h

//********************************************************************

#ifndef jlj_xml_serialize_BaseMarkupSerializer_h
#define jlj_xml_serialize_BaseMarkupSerializer_h

//********************************************************************

#include <jlj/xml/serialize/DOMSerializer.h>
#include <jlj/xml/serialize/OutputFormat.h>
#include <xercesc/dom/DOMLSOutput.hpp>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//********************************************************************

class ExportedByJLJxml BaseMarkupSerializerI
	: public DOMSerializerI
{
protected:
	~BaseMarkupSerializerI() = 0;
	friend class TypedReference<BaseMarkupSerializerI>; 

public:
	void serialize(Node) throw (jlj::io::IOException);
	void serialize(const jlj::util::List<Node>&) throw (jlj::io::IOException);
	void setOutputCharStream(const String& filename) throw (jlj::io::IOException);

protected:
	BaseMarkupSerializerI(xercesc::DOMImplementation*) throw (jlj::io::IOException);

protected:
	AutoRelease<xercesc::DOMLSSerializer> serializer;
	AutoRelease<xercesc::DOMLSOutput> output;
	OutputFormat outputformat;

private:	
	xercesc::XMLFormatTarget* target;

};

//********************************************************************

StrongReferenceCommonDeclaration(BaseMarkupSerializer, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof


