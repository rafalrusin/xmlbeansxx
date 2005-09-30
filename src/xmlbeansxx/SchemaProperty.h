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


#ifndef _XMLBEANSXX_SCHEMAPROPERTY_H_
#define _XMLBEANSXX_SCHEMAPROPERTY_H_

#include "BoostAssert.h"
#include "String.h"
#include <boost/shared_ptr.hpp>
#include "Ptr.h"
namespace xmlbeansxx {

class SchemaType_I;
class SchemaType;
class SchemaProperty_I;

BEGIN_CLASS(SchemaProperty, SchemaProperty_I)
    public:
    enum NVC_TYPE {NEVER, VARIABLE, CONSISTENTLY};
END_CLASS()

/**
* Corresponds to org.apache.xmlbeans.SchemaProperty Java class.
* Contains information about a property of some SchemaType.
*/
class SchemaProperty_I {
public:
    virtual SchemaType getType() = 0;
    virtual String getDefaultText() = 0;
    virtual SchemaProperty::NVC_TYPE hasDefault() = 0;
    
    virtual ~SchemaProperty_I() {}
};

}

#endif//_XMLBEANSXX_SCHEMAPROPERTY_H_

