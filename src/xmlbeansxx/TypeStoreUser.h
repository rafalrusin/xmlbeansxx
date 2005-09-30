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
#ifndef XMLBEANSXX_TYPESTOREUSER_INCLUDED
#define XMLBEANSXX_TYPESTOREUSER_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include <boost/shared_ptr.hpp>
#include "SchemaType.h"
#include "Mutex.h"
#include "Ptr.h"
namespace xmlbeansxx {
class TypeStore_I;
class TypeStore;
class TypeStoreUser_I;
BEGIN_CLASS(TypeStoreUser, TypeStoreUser_I)
END_CLASS()
class TypeStoreUser_I {
public:
    virtual ~TypeStoreUser_I();
    virtual Mutex &mutex() = 0;
    virtual void attach_store(const TypeStore &store) = 0;
    /**
    * @return always not NULL store. May return different
    * stores in several calls.
    */
    virtual TypeStore get_store() = 0;
    virtual SchemaType get_schema_type() = 0;

    virtual String build_text() = 0;

    virtual TypeStoreUser create_element_user(QName eltName, QName xsiType) = 0;

    /**
    * A typestore user can create a new TypeStoreUser instance for
    * a given attribute child, based on the attribute name.
    */
    virtual TypeStoreUser create_attribute_user(QName attrName) = 0;

    /**
    * Return the SchemaType which a child element of this name and xsi:type
    * would be.
    */
    virtual SchemaType get_element_type(QName eltName, QName xsiType) = 0;

    /**
    * Return the SchemaType which an attribute of this name would be.
    */

    virtual SchemaType get_attribute_type(QName attrName) = 0;

    /**
    * Returns the default element text, if it's consistent. If it's
    * not consistent, returns null, and requires a visitor walk.
    */
    //virtual String get_default_element_text(QName eltName) = 0;

    /**
    * Returns the default attribute text for the attribute with
    * the given name.
    */
    //virtual String get_default_attribute_text(QName attrName) = 0;

};
}
#endif //XMLBEANSXX_TYPESTOREUSER_INCLUDED
