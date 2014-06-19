//XmlSerializer.h

//********************************************************************

#ifndef jlj_xml_serialize_XmlSerializer_h
#define jlj_xml_serialize_XmlSerializer_h

//********************************************************************

#include <jlj/xml/serialize/BaseMarkupSerializer.h>
#include <jlj/xml/parsers/DocumentBuilderFactory.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//********************************************************************

class ExportedByJLJxml XmlSerializerI
    : public BaseMarkupSerializerI
{
protected:
    ~XmlSerializerI(); 
    friend class TypedReference<XmlSerializerI>;

    XmlSerializerI(xercesc::DOMImplementation*) throw (jlj::io::IOException);
	friend class ExportedByJLJxml jlj::xml::parsers::DocumentBuilderFactoryI;

public:
	void setOutputFormat(const OutputFormat&);

};

//********************************************************************

StrongReferenceCommonDeclaration(XmlSerializer, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof

