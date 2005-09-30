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

#include "BoostAssert.h"
#include "SchemaProperty.h"
#include "QName.h"
#include "Ptr.h"

namespace xmlbeansxx {
class SchemaType_I;
BEGIN_CLASS(SchemaType, SchemaType_I)
    public:
    enum WS_TYPE {WS_PRESERVE,WS_REPLACE,WS_UNSPECIFIED,WS_COLLAPSE};
    enum CONTENT_TYPE {EMPTY_CONTENT,SIMPLE_CONTENT,ELEMENT_CONTENT,MIXED_CONTENT,NOT_COMPLEX_TYPE};
END_CLASS()

/** Corresponds to org.apache.xmlbeans.SchemaType. Contains information about some xml schema type. */
class SchemaType_I {
    public:
    virtual SchemaProperty getElementProperty(const QName &eltName) = 0;
    virtual SchemaProperty getAttributeProperty(const QName &attrName) = 0;
    virtual SchemaType::WS_TYPE getWhitespaceRule() = 0;
    virtual QName getName() = 0;
    virtual bool isDocumentType() = 0;
    virtual SchemaType::CONTENT_TYPE getContentType() = 0;
    
    /** @deprecated */
    virtual int getFractionDigits() = 0;
    
    virtual ~SchemaType_I() {}
};

}

#endif//_XMLBEANSXX_SCHEMATYPE_H_

