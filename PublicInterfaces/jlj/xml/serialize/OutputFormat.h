//OutputFormat.h

//********************************************************************

#ifndef jlj_xml_serialize_OutputFormat_h
#define jlj_xml_serialize_OutputFormat_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/String.h>
using jlj::lang::ObjectI;
using jlj::lang::String;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(xml)
NAMESPACE_BEGIN(serialize)

//********************************************************************

class ExportedByJLJxml OutputFormatI
    : public ObjectI
{

protected:
    ~OutputFormatI();       
    friend class TypedReference<OutputFormatI>; 

public:       
    OutputFormatI();

public:
    String getEncoding() const;
    String getLineSeparator() const;
    bool getPrettyPrint() const;
    void setEncoding(const String&);
    void setLineSeparator(const String&);

private:
    String encoding;
    String lineSeparator;
    bool prettyPrint;

};

//********************************************************************

StrongReferenceCommonDeclaration(OutputFormat, ExportedByJLJxml);

//********************************************************************

NAMESPACE_END(serialize)
NAMESPACE_END(xml)
NAMESPACE_END(jlj)

//********************************************************************

#endif
