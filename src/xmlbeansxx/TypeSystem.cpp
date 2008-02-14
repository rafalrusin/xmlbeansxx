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
#include <iostream>

#include "BoostAssert.h"

#include "TypeSystem.h"
#include "defs.h"
#include "logger.h"
#include "Tracer.h"
#include "XmlBeans.h"


#include <xercesc/util/PlatformUtils.hpp>

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/detail/lock.hpp>
#define SYNC(mutex) boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(mutex);
#else
#define SYNC(mutex)
#endif


using namespace std;


XERCES_CPP_NAMESPACE_USE

namespace xmlbeansxx {

//------------------------------------------------------------------------------------
// Type system
//XMLBEANSXX_LOGGER_PTR_SET(TypeSystem::log,"xmlbeansxx.TypeSystem");
#ifdef XMLBEANSXX_LOGGING
static log4cxx::LoggerPtr log()
{
	XMLBEANSXX_STATIC_LOGGER_PTR_SET(slog,"xmlbeansxx.TypeSystem");
	return slog;
	
}
#endif


XmlObjectPtr TypeSystem::createDocumentByName(const QName &typeName) const {
    TRACER(log(),"createByName");
   
    const SchemaType * st=0;
    {
    	SYNC(mutex);
    	TypeCreators_type::const_iterator i = documentTypeCreators.find(typeName);
	if(i != documentTypeCreators.end()) 
		st=i->second;
    }
    if(!st) {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" Document Object not registered ,  returning NULL pointer"));
        return XmlObjectPtr();
    }
    CreateObjFn f=st->createFn;
    if (f==NULL) {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}


XmlObjectPtr TypeSystem::createByName(const QName &typeName) const{
    TRACER(log(),"createByName");
    
    const SchemaType * st=0;
    {
    	SYNC(mutex);
    	TypeCreators_type::const_iterator i = typeCreators.find(typeName);
	if(i != typeCreators.end()) 
		st=i->second;
    }
    if(!st) {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" Object not registered ,  returning NULL pointer"));
        return XmlObjectPtr();
    }
    CreateObjFn f=st->createFn;
    if (f==NULL) {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}

XmlObjectPtr TypeSystem::createArrayByName(const QName &typeName) const {
    TRACER(log(),"createArrayByName");

    const SchemaType * st=0;
    {
    	SYNC(mutex);
    	TypeCreators_type::const_iterator i = typeCreators.find(typeName);
	if(i != typeCreators.end()) 
		st=i->second;
    }
    if(!st) {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" Object not registered ,  returning NULL pointer"));
        return XmlObjectPtr();
    }
    CreateObjFn f=st->createArrayFn;
    if (f==NULL) {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" returning NULL pointer"));
        return XmlObjectPtr();
    } else {
        XMLBEANSXX_DEBUG(log(),std::string("typeName:")+typeName+std::string(" using creator"));
        return f();
    }
}

void TypeSystem::addType(const SchemaType *st) {
    SYNC(mutex);
    typeCreators[st->name]=st;
}

void TypeSystem::addDocumentType(const SchemaType *st) {
    addType(st);
    {
    	SYNC(mutex);
    	documentTypeCreators[st->documentElementName]=st;
    }
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
    	std::cerr << "Locale: " << locale << "\n";
//    	XmlBeans::encoding(locale);
	XMLPlatformUtils::Initialize(locale);
//        XMLBEANSXX_INFO(log(),"Locale: " << locale);
    }
    
    initialized=true;
}

void terminateBeans() {
    initialized=false;
}

class BeansExistence {
private:
//    XMLBEANSXX_STATIC_LOGGER_PTR(log);
    
public:
    BeansExistence() {
//        XMLBEANSXX_DEBUG(log(),"Initializing xmlbeansxx");
        initializeBeans();
//        XMLBEANSXX_DEBUG(log(),"Initialized xmlbeansxx");
    }
    
    ~BeansExistence() {
//        XMLBEANSXX_DEBUG(log(),"Terminating xmlbeansxx");
        terminateBeans();
//        XMLBEANSXX_DEBUG(log(),"Terminated xmlbeansxx");
    }
};
 
//XMLBEANSXX_LOGGER_PTR_SET(BeansExistence::log,"xmlbeansxx.BeansExistence");

}

//-------------------------------------------------------------------------
TypeSystem *globalTypeSystem() {
    static BeansExistence exist;
    static TypeSystem * gts=new TypeSystem();
    return gts;
}


}
