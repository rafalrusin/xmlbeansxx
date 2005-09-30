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
    
#ifndef XMLBEANSXX_SCHEMAPROPERTYIMPL_INCLUDED
#define XMLBEANSXX_SCHEMAPROPERTYIMPL_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "SchemaProperty.h"
#include "SchemaType.h"
#include "Ptr.h"
namespace xmlbeansxx {
class SchemaPropertyImpl_I;
BEGIN_CLASS(SchemaPropertyImpl, SchemaPropertyImpl_I)
END_CLASS()
class SchemaPropertyImpl_I: public SchemaProperty_I {
    private:
    static log4cxx::LoggerPtr LOG;
public:
    /** Position of property used in serialization */
    int order;
    /** Pointer to SchemaType of this property */
    SchemaType schemaType;
    /** This is not NULL if this property is a singleton (not array) and has default value */
    String defaultText;
    
    SchemaProperty::NVC_TYPE _hasDefault;
    
    virtual SchemaType getType() { return schemaType; }
    virtual String getDefaultText() { return defaultText; }
    virtual SchemaProperty::NVC_TYPE hasDefault() { return _hasDefault; }
};
}
#endif //XMLBEANSXX_SCHEMAPROPERTYIMPL_INCLUDED
