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
#ifndef XMLBEANSXX_TYPESTORE_INCLUDED
#define XMLBEANSXX_TYPESTORE_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "QName.h"
#include "TypeStoreUser.h"
#include "Mutex.h"
#include "TypeStoreVisitor.h"
#include "XmlCursor.h"
#include "Array.h"
#include "SchemaType.h"
namespace xmlbeansxx {
class TypeStore_I;
BEGIN_CLASS(TypeStore, TypeStore_I)
END_CLASS()
class TypeStore_I {
public:
    virtual ~TypeStore_I();

    virtual Mutex &mutex() = 0;
    virtual XmlCursor new_cursor() = 0;
    
    virtual void store_text(const String &text) = 0;
    virtual String fetch_text(SchemaType::WS_TYPE whitespaceRule) = 0;

    virtual TypeStoreUser change_type(const SchemaType &schemaType) = 0;

    virtual void copy_contents_from(const TypeStore &source) = 0;
    virtual TypeStoreUser find_attribute(const QName &name) = 0;
    virtual void remove_attribute(const QName &name) = 0;
    virtual void add_attribute(const QName &name, const TypeStoreUser &value) = 0;
    virtual TypeStoreUser add_attribute(const QName &name) = 0;

    /** @return NULL if element at specified index was not found otherwise it's value. */
    virtual TypeStoreUser find_element(const QName &name,int index) = 0;

    virtual void find_all_elements(const QName &name, const Array<TypeStoreUser> &toFill) = 0;

    /** @return number of elements given name. */
    virtual int count_elements(const QName &name) = 0;

    /** Appends specified element to end of list. */
    virtual void add_element(const QName &name, const TypeStoreUser &value) = 0;
    virtual TypeStoreUser add_element(const QName &name) = 0;

    /** Sets element at specific position. */
    virtual void set_element(const QName &name, int index, const TypeStoreUser &value) = 0;

    /**
    * Removes all elements of specified name.
    */
    virtual void remove_all_elements(const QName &name) = 0;

    /**
    * Removes element of given name at specified index (by cutting it off).
    */
    virtual void remove_element(const QName &name,int index) = 0;

    /**
    * Visits all children in specific order.
    */
    virtual void visit_elements(const TypeStoreVisitor &visitor) = 0;

};
}
#endif //XMLBEANSXX_TYPESTORE_INCLUDED
