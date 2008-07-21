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


#ifndef _XMLBEANSXX_TYPESYSTEM_H_
#define _XMLBEANSXX_TYPESYSTEM_H_

#include "BoostAssert.h"
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include "macros.h"
#include "SchemaType.h"


#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#endif


namespace xmlbeansxx {

class TypeSystem {

    typedef std::map<QName,const SchemaType *> TypeCreators_type;
#ifdef BOOST_HAS_THREADS
    mutable boost::recursive_mutex mutex;
#endif    
    TypeCreators_type typeCreators;
    TypeCreators_type documentTypeCreators;


public:

    TypeSystem(){};

    XmlObjectPtr createDocumentByName(const QName &typeName) const;
    XmlObjectPtr createByName(const QName &typeName) const;
    const SchemaType * getSchemaType(const QName &typeName) const;
    
//    XmlObjectPtr createArrayByName(const QName &typeName) const;
    
    void addDocumentType(const SchemaType *st);
    void addType(const SchemaType *st);
};

TypeSystem *globalTypeSystem();

}

#endif//_XMLBEANSXX_TYPESYSTEM_H_
