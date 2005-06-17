/*
    Copyright 2004-2005 TouK s.c.
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */


#ifndef _XMLBEANSXX_SCHEMATYPE_H_
#define _XMLBEANSXX_SCHEMATYPE_H_

#include <typeinfo>
#include <map>
#include <string>
#include "macros.h"
#include "SchemaProperty.h"



namespace xmlbeansxx {
    
class SchemaType;
DECLARE_PTR(SchemaType,SchemaTypePtr,constSchemaTypePtr)

class XmlObject;
DECLARE_PTR(XmlObject,XmlObjectPtr,constXmlObjectPtr)
typedef XmlObjectPtr (*CreateObjFn)();

struct Map_String_SchemaPropertyPtr: std::map<std::string,SchemaPropertyPtr> {
    Map_String_SchemaPropertyPtr();
    Map_String_SchemaPropertyPtr(const Map_String_SchemaPropertyPtr &b);
};


/** Corresponds to org.apache.xmlbeans.SchemaType. Contains information about some xml schema type. */    
class SchemaType {
private:
    /** Initializes SchemaType to default values. */
    void init();
public:
    enum WS_TYPE {WS_PRESERVE,WS_REPLACE,WS_UNSPECIFIED,WS_COLLAPSE};

    WS_TYPE whitespaceRule;
    
    /** Attribute properties map. */
    Map_String_SchemaPropertyPtr attributes;
    
    /** Element properties map. */
    Map_String_SchemaPropertyPtr elements;

    /** Functions for creating XmlObject of this SchemaType. */
    CreateObjFn createFn,createArrayFn;

    /** Name of a class corresponding to this SchemaType. */
    std::string className;
    
    /** Local part of QName of this SchemaType. TODO: change it to xmlbeansxx::QName. */
    std::string xsdTypeName;
    /** Namespace URI id of QName of this SchemaType. TODO: change it to xmlbeansxx::QName. */
    int xsdNamespaceID;

    /** @deprecated This is used to support xmlbeansxx::XmlArray serialization and deserialization. */
    std::string arrayXsdTypeName;
    /** @deprecated This is used to support xmlbeansxx::XmlArray serialization and deserialization. */
    int arrayXsdNamespaceID;
    
    bool processContents;

    const std::type_info &classTypeInfo;
   
    enum CONTENT_TYPE {EMPTY_CONTENT,SIMPLE_CONTENT,ELEMENT_CONTENT,MIXED_CONTENT,NOT_COMPLEX_TYPE};
    CONTENT_TYPE contentType;
    
    //methods
    SchemaType(const std::type_info &classTypeInfo);
    
    XmlObjectPtr createSubObject(const std::string &name) const;

    SchemaPropertyPtr findAttribute(const std::string &name) const;
    SchemaPropertyPtr findElement(const std::string &name) const;

    CONTENT_TYPE getContentType() const;

    //schema facets

    /** -1 while not set */
    int fractionDigits;
};

}

#endif//_XMLBEANSXX_SCHEMATYPE_H_

