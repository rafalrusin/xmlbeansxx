/*
    Copyright 2004-2008 TouK sp. z o.o. s.k.a.

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
#include "QName.h"


namespace xmlbeansxx {

class SchemaType;
DECLARE_PTR(SchemaType,SchemaTypePtr,constSchemaTypePtr)

class XmlObject;
DECLARE_PTR(XmlObject,XmlObjectPtr,constXmlObjectPtr)
typedef XmlObjectPtr (*CreateObjFn)();

struct Map_QName_SchemaPropertyPtr: std::map<QName,SchemaPropertyPtr> {
    Map_QName_SchemaPropertyPtr();
    Map_QName_SchemaPropertyPtr(const Map_QName_SchemaPropertyPtr &b);
};


class SchemaType {
private:
    /** Sets default values */
    void init();
public:
    enum WS_TYPE {WS_PRESERVE,WS_REPLACE,WS_UNSPECIFIED,WS_COLLAPSE};
    WS_TYPE whitespaceRule;
    typedef Map_QName_SchemaPropertyPtr ElementsType;
    ElementsType attributes;
    ElementsType elements;

    CreateObjFn createFn;
//    CreateObjFn createArrayFn;

    std::string className;
    QName name;
    QName documentElementName;
    bool processContents;
    bool isArray;
    /** -1 while not set */
    int fractionDigits;

    const std::type_info &classTypeInfo;

    enum CONTENT_TYPE {EMPTY_CONTENT,SIMPLE_CONTENT,ELEMENT_CONTENT,MIXED_CONTENT,NOT_COMPLEX_TYPE};
    CONTENT_TYPE contentType;

    //methods
    SchemaType(	const std::type_info &classTypeInfo);
    void propertyElem(int elems, const char * elemTable[]);
    void propertyAttr(int attr,  const char * attrTable[]);

    XmlObjectPtr createSubObject(const QName &name) const;

    SchemaPropertyPtr findAttribute(const QName &name) const;
    SchemaPropertyPtr findElement(const QName &name) const;

    CONTENT_TYPE getContentType() const;

    //schema facets

};

}

#endif//_XMLBEANSXX_SCHEMATYPE_H_

