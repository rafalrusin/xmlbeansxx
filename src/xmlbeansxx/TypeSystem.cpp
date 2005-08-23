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


#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/BinFileInputStream.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <log4cxx/logger.h>
#include <string>

#include "BoostAssert.h"

//#include "XmlTypesGen.h"
#include "TypeSystem.h"
#include "defs.h"
#include "Tracer.h"


using namespace std;
XERCES_CPP_NAMESPACE_USE



namespace xmlbeansxx {

//------------------------------------------------------------------------------------
// Type system
log4cxx::LoggerPtr TypeSystem::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.TypeSystem"));

TypeSystem::TypeSystem(): n_namespaces(0) {
    _xs_ns=addNamespace("http://www.w3.org/2001/XMLSchema");
    _xsi_ns=addNamespace("http://www.w3.org/2001/XMLSchema-instance");
}

XmlObjectPtr TypeSystem::createByName(const std::string &typeName,int namespace_nr) {
    TRACER(log,"createByName")
    CreateObjFn f=typeCreators[pair<int,string>(namespace_nr,typeName)]->createFn;
    if (f==NULL) {
        LOG4CXX_DEBUG(log,std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        LOG4CXX_DEBUG(log,std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}

XmlObjectPtr TypeSystem::createArrayByName(const std::string &typeName,int namespace_nr) {
    TRACER(log,"createArrayByName")
    CreateObjFn f=typeCreators[pair<int,string>(namespace_nr,typeName)]->createArrayFn;
    if (f==NULL) {
        LOG4CXX_DEBUG(log,std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        LOG4CXX_DEBUG(log,std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}

int TypeSystem::addNamespace(const std::string namespaceName) {
    if (namespaces.count(namespaceName)==0) {
        namespaces[namespaceName]=n_namespaces+1;
        n_namespaces++;
        return n_namespaces-1;
    } else {
        // return namespace's number by URI
        return namespaces[namespaceName]-1;
    }
}

int TypeSystem::getNamespace(const std::string namespaceName) {
    return namespaces[namespaceName]-1;
}

int TypeSystem::nrNamespaces() {
    return n_namespaces;
}

int TypeSystem::xs_ns() {
    return _xs_ns;
}
int TypeSystem::xsi_ns() {
    return _xsi_ns;
}

std::string TypeSystem::getNamespaceName(int namespaceID) {
    FOREACH(it,namespaces) {
        if ((it->second-1)==namespaceID)
            return it->first;
    }
    return "";
}

void TypeSystem::addType(const SchemaType *st) {
    BOOST_ASSERT(st->xsdNamespaceID!=-1);
    typeCreators[pair<int,string>(st->xsdNamespaceID,st->xsdTypeName)]=st;
}

//------------------------------------------------------------------------------------
namespace {
bool initialized=false;

void initializeBeans() {
    try {
        XMLPlatformUtils::Initialize("pl_PL.UTF-8");
        //XMLPlatformUtils::Initialize();
    } catch(...) {
        throw XmlException("Error during Xerces-c initialization");
    }


    initialized=true;
}

void terminateBeans() {
    try {
        XMLPlatformUtils::Terminate();
    } catch(...) {
        throw XmlException("Error during Xerces-c termination");
    }
    initialized=false;
}

class BeansExistence {
private:
    log4cxx::LoggerPtr log;
    
public:
    BeansExistence() {
        log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.BeansExistence"));
        LOG4CXX_DEBUG(log,"Initializing xmlbeansxx");
        initializeBeans();
        LOG4CXX_DEBUG(log,"Initialized xmlbeansxx");
    }
    
    ~BeansExistence() {
        LOG4CXX_DEBUG(log,"Terminating xmlbeansxx");
        terminateBeans();
        LOG4CXX_DEBUG(log,"Terminated xmlbeansxx");
    }
};


}

//-------------------------------------------------------------------------
TypeSystem *globalTypeSystem() {
    static BeansExistence exist;
    static TypeSystem *gts=new TypeSystem();
    return gts;
}


}
