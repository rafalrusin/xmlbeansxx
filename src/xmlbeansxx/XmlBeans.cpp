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
    
#include "XmlBeans.h"
#include <log4cxx/logger.h>
#include "StoreString.h"
#include <xercesc/util/PlatformUtils.hpp>
#include "BeansException.h"
#include "SchemaTypeLoaderImpl.h"
#include "XmlTypesGen.h"
XERCES_CPP_NAMESPACE_USE

namespace xmlbeansxx {

namespace {
    Existence existenceVar = XmlBeans::staticInit();
}

log4cxx::LoggerPtr XmlBeans::LOG = XmlBeans::LOG;

XmlBeans::MyExistence_I::MyExistence_I() {
    XmlBeans::LOG = log4cxx::Logger::getLogger("xmlbeansxx.XmlBeans");
    
    LOG4CXX_DEBUG(XmlBeans::LOG,"Initializing xmlbeansxx");
    dependencies.push_back(StoreString::staticInit());
    StoreString::store("");
    XmlBeans::xs_ns(); //internal static init invokation to be thread-safe
    XmlBeans::xsi_ns(); //internal static init invokation to be thread-safe
    XmlBeans::xsi_type(); //internal static init invokation to be thread-safe
    
    //Xerces init
    try {
        XMLPlatformUtils::Initialize("pl_PL.UTF-8");
        //XMLPlatformUtils::Initialize();
    } catch(...) {
        LOG4CXX_ERROR(XmlBeans::LOG, "Error during Xerces-c initialization");
        throw IllegalStateException();
    }
    //~Xerces init
    
    LOG4CXX_DEBUG(XmlBeans::LOG,"Initialized xmlbeansxx");
}

XmlBeans::MyExistence_I::~MyExistence_I() {
    LOG4CXX_DEBUG(XmlBeans::LOG, "Terminating xmlbeansxx");
    //Xerces termination
    try {
        XMLPlatformUtils::Terminate();
    } catch(...) {
        LOG4CXX_ERROR(XmlBeans::LOG, "Error during Xerces-c termination");
        throw IllegalStateException();
    }
    //~Xerces termination
}

Existence XmlBeans::staticInit() {
    static Existence ptr(new MyExistence_I());
    return ptr;
}

StoreString XmlBeans::xs_ns() {
    static StoreString ns = (StoreString::staticInit(), StoreString::store("http://www.w3.org/2001/XMLSchema"));
    return ns;
}

StoreString XmlBeans::xsi_ns() {
    static StoreString ns = (StoreString::staticInit(), StoreString::store("http://www.w3.org/2001/XMLSchema-instance"));
    return ns;
}

QName XmlBeans::xsi_type() {
    static QName n = (StoreString::staticInit(), 
        QName(
            StoreString::store("http://www.w3.org/2001/XMLSchema-instance"),
            StoreString::store("type")
        )
        );
    return n;
}

SchemaTypeLoader XmlBeans::getContextTypeLoader() {
    static SchemaTypeLoader loader(new SchemaTypeLoaderImpl_I());
    return loader;
}

}

