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

template<class T>
struct DictEl {
    //std::string name;
    StoreString name;
    T value;

    DictEl() {}
    DictEl(std::string name,T value):
    name(name),value(value) {}
};

//slownik mapujacy u->v (v jest typu T), pamietajacy kolejnosc elementow
//dziala tak, ze kasujac jakis element nie zwalniamy value, tylko wymazujemy;
//name za to zwalniamy.
//natomiast free() zwalnia name i value.
struct AttrDict {
    typedef std::string T;
    std::vector<DictEl<T> > contents;

public:
    int size() const;
    bool hasEmptyContent() const;
    StringPtr find(std::string u) const;
    int count(std::string u) const;
    /** Duplicates u, but not v */
    void add
        (std::string u,T v);
    void set
        (std::string u,int pos,T v);
    int del(std::string u);
    void free();
};


struct ElemDict {
    typedef XmlObjectPtr T;
    std::vector<DictEl<T> > contents;

public:

    int size() const;
    bool hasEmptyContent() const;
    T find(std::string u,int nr) const;
    int count(std::string u) const;
    /** Duplicates u, but not v */
    void add
        (std::string u,T v);
    void set
        (std::string u,int pos,T v);
    int del(std::string u);
    void removeAt(std::string name,int index);
    void free();
};
}


#endif //XMLBEANSPP_DICT_H_

