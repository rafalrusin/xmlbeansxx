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


#include <string>

#include "BoostAssert.h"

#include "TypeSystem.h"
#include "defs.h"
#include "logger.h"
#include "Tracer.h"


using namespace std;



namespace xmlbeansxx {

//------------------------------------------------------------------------------------
// Type system
//LOGGER_PTR_SET(TypeSystem::log,"xmlbeansxx.TypeSystem");
static log4cxx::LoggerPtr log()
{
	STATIC_LOGGER_PTR_SET(slog,"xmlbeansxx.TypeSystem");
	return slog;
	
}


XmlObjectPtr TypeSystem::createDocumentByName(const QName &typeName) {
    TRACER(log(),"createByName");
    
    
    const SchemaType * st = documentTypeCreators[typeName];
    if(!st) {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" Document Object not registered ,  returning NULL pointer"));
        return XmlObjectPtr();
    }
    CreateObjFn f=st->createFn;
    if (f==NULL) {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}


XmlObjectPtr TypeSystem::createByName(const QName &typeName) {
    TRACER(log(),"createByName");
    
    
    const SchemaType * st = typeCreators[typeName];
    if(!st) {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" Object not registered ,  returning NULL pointer"));
        return XmlObjectPtr();
    }
    CreateObjFn f=st->createFn;
    if (f==NULL) {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}

XmlObjectPtr TypeSystem::createArrayByName(const QName &typeName) {
    TRACER(log(),"createArrayByName");

    const SchemaType * st = typeCreators[typeName];
    if(!st) {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" Object not registered ,  returning NULL pointer"));
        return XmlObjectPtr();
    }
    CreateObjFn f=st->createArrayFn;
    if (f==NULL) {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        LOG4CXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}

void TypeSystem::addType(const SchemaType *st) {
    typeCreators[st->name]=st;
}

void TypeSystem::addDocumentType(const SchemaType *st) {
    documentTypeCreators[st->name]=st;
}

//------------------------------------------------------------------------------------
namespace {
bool initialized=false;

void initializeBeans() {

    if(initialized) return;
    
    const char *locale=setlocale(LC_CTYPE, "");
    if (!locale) {
        throw XmlException( "Locale not specified. Check LANG, LC_CTYPE, LC_ALL." );
    } else {
//        LOG4CXX_INFO(log(),"Locale: " << locale);
    }
    
    initialized=true;
}

void terminateBeans() {
    initialized=false;
}

class BeansExistence {
private:
//    STATIC_LOGGER_PTR(log);
    
public:
    BeansExistence() {
//        LOG4CXX_DEBUG(log(),"Initializing xmlbeansxx");
        initializeBeans();
//        LOG4CXX_DEBUG(log(),"Initialized xmlbeansxx");
    }
    
    ~BeansExistence() {
//        LOG4CXX_DEBUG(log(),"Terminating xmlbeansxx");
        terminateBeans();
//        LOG4CXX_DEBUG(log(),"Terminated xmlbeansxx");
    }
};
 
//LOGGER_PTR_SET(BeansExistence::log,"xmlbeansxx.BeansExistence");

}

//-------------------------------------------------------------------------
TypeSystem *globalTypeSystem() {
    static BeansExistence exist;
    static TypeSystem *gts=new TypeSystem();
    return gts;
}


}
