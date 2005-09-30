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
    
#ifndef XMLBEANSXX_SCHEMATYPEIMPL_INCLUDED
#define XMLBEANSXX_SCHEMATYPEIMPL_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "SchemaType.h"
#include "CoreTypes.h"
#include "SchemaPropertyImpl.h"
#include "XmlTypesGen.h"
namespace xmlbeansxx {
class SchemaTypeImpl_I;
BEGIN_CLASS(SchemaTypeImpl, SchemaTypeImpl_I)
END_CLASS()

typedef XmlObject (*ObjectCreatorFn)();

class SchemaTypeImpl_I: public SchemaType_I {
    private:
    static log4cxx::LoggerPtr LOG;
    public:
private:
    /** Initializes SchemaType to default values. */
    void init();
public:
    typedef Map<QName, SchemaPropertyImpl, QName::Hash>::type PropertyMap;

    SchemaType::WS_TYPE whitespaceRule;

    /** Attribute properties map. */
    PropertyMap attributes;

    /** Element properties map. */
    PropertyMap elements;

    /** Functions for creating XmlObject of this SchemaType. */
    ObjectCreatorFn createRawXmlObject;

    /** Name of a class corresponding to this SchemaType. */
    String className;

    /** QName of this SchemaType. */
    QName name;

    bool processContents;
    bool _isDocumentType;

    const std::type_info &classTypeInfo;

    SchemaType::CONTENT_TYPE contentType;

    SchemaTypeImpl_I(const std::type_info &classTypeInfo);

    //schema facets

    /** -1 while not set */
    int fractionDigits;
    
    virtual SchemaProperty getElementProperty(const QName &eltName);
    virtual SchemaProperty getAttributeProperty(const QName &attrName);
    virtual SchemaType::WS_TYPE getWhitespaceRule() { return whitespaceRule; }
    virtual QName getName() { return name; }
    virtual bool isDocumentType() { return _isDocumentType; }
    virtual SchemaType::CONTENT_TYPE getContentType() { return contentType; }
    
    /** @deprecated */
    virtual int getFractionDigits() { return fractionDigits; }
    
};
}
#endif //XMLBEANSXX_SCHEMATYPEIMPL_INCLUDED
