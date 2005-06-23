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


#ifndef _XMLBEANSXX_TYPESYSTEM_H_
#define _XMLBEANSXX_TYPESYSTEM_H_

#include "BoostAssert.h"
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include "macros.h"
#include "SchemaType.h"



namespace xmlbeansxx {

/** 
 * This class is used to remember a mapping of xml schema type names to SchemaType descriptions.
 */
class TypeSystem {
    static log4cxx::LoggerPtr log;
    
    int n_namespaces;
    std::map<std::string,int> namespaces;
    std::map<std::pair<int,std::string>,const SchemaType *> typeCreators;

    int _xs_ns,_xsi_ns;

public:

    TypeSystem();

    XmlObjectPtr createByName(const std::string &typeName,int namespace_nr);
    XmlObjectPtr createArrayByName(const std::string &typeName,int namespace_nr);

    /** 
     * Registers a namespace in a pool if it isn't registered yet.
     * It returns a stored identifier of this namespace.
     */
    int addNamespace(const std::string namespaceName);
    /**
     * Returns an identifier of given namespace or -1 if it isn't registered. 
     */
    int getNamespace(const std::string namespaceName);
    /** Returns count of all namespaces n. Namespace identifiers are between 0 and n-1. */
    int nrNamespaces();
    void addType(const SchemaType *st);

    /** returns "" while not found */
    std::string getNamespaceName(int namespaceID);

    /** Schema namespace ID. */
    int xs_ns();
    /** Schema instance namespace ID. */
    int xsi_ns();
};

/**
 * Every generated class registers itself in this type system during static initialization. 
 */
TypeSystem *globalTypeSystem();

}

#endif//_XMLBEANSXX_TYPESYSTEM_H_
