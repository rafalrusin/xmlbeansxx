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


#ifndef _XMLBEANSXX_DICT_H_
#define _XMLBEANSXX_DICT_H_

#include "BoostAssert.h"
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "macros.h"
#include "StoreString.h"


	
namespace xmlbeansxx {

DECLARE_PTR(std::string,StringPtr,constStringPtr)
class XmlObject;
DECLARE_PTR(XmlObject,XmlObjectPtr,constXmlObjectPtr)

/**
 * Represents dictionary element.
 */
template<class T>
struct DictEl {
    StoreString name;
    T value;

    DictEl() {}
    DictEl(std::string name,T value):
    name(name),value(value) {}
};

/**
 * This is xml attributes dictionary used by Contents.
 */
struct AttrDict {
    typedef std::string T;
    std::vector<DictEl<T> > contents;

public:
    /** @return number of elements in dictionary. */
    int size() const;
    /** @return true if dictionary has no content. */
    bool hasEmptyContent() const;
    /** @return NULL if element was not found otherwise it's value. */
    StringPtr find(std::string u) const;
    /** @return number of elements given name. */
    int count(std::string u) const;
    /** Appends specified element. */
    void add(std::string u,T v);
    /** 
     * Sets specified element at given position. 
     * For attribute dictionary only position 0 is possible.
     */
    void set(std::string u,int pos,T v);
    /**
     * Removes specified element.
     */
    int del(std::string u);
    /**
     * Clears contents of dictionary.
     */
    void free();
};

/**
 * This is xml elements dicrionary used by Contents.
 */
struct ElemDict {
    typedef XmlObjectPtr T;
    std::vector<DictEl<T> > contents;

public:

    /** @return number of elements in dictionary. */
    int size() const;
    /** @return true if dictionary has no content. */
    bool hasEmptyContent() const;
    /** @return NULL if element at specified index was not found otherwise it's value. */
    T find(std::string u,int index) const;
    /** @return number of elements given name. */
    int count(std::string u) const;
    /** Appends specified element to end of list. */
    void add(std::string u,T v);
    /** 
     * Sets specified element at given position. If position is out of bounds, then adds some NULL elements.
     */
    void set(std::string u,int pos,T v);
    /**
     * Removes all elements of specified name.
     */
    int del(std::string u);
    /**
     * Removes element of given name at specified index (by cutting it off).
     */
    void removeAt(std::string name,int index);
    /**
     * Clears contents of dictionary.
     */
    void free();
};
}


#endif //XMLBEANSXX_DICT_H_

