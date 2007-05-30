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


#ifndef _XMLBEANSXX_XMLARRAY_H_
#define _XMLBEANSXX_XMLARRAY_H_

#include "BoostAssert.h"
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "logger.h"

#include "shared_array.h"
#include "XmlTypesGen.h"
#include "macros.h"
#include "BeansException.h"
#include "Tracer.h"
#include "Dict.h"
//#include "XmlTypesGen.h"


	
namespace xmlbeansxx {


#ifdef HAVE_LIBLOG4CXX
extern LOGGER_PTR(XmlArray_log);
#endif

extern QName XmlArray_elemName;

template<class T>
static SchemaType XmlArray_initSchemaType() {
      	xmlbeansxx::SchemaType st(typeid(T));
      	st.createFn=T::Factory::newXmlArrayInstance;
        st.createArrayFn=NULL;
      	st.whitespaceRule=xmlbeansxx::SchemaType::WS_COLLAPSE;
      	T o=T::Factory::newInstance();
      	st.className=o.getSchemaType()->className;
      	st.name=o.getSchemaType()->name;
	
	st.isArray=true;
      	st.processContents=true;
	
	st.elements[QName::store("http://xmlbeans.apache.org/definitions","e")]=xmlbeansxx::SchemaPropertyPtr(new xmlbeansxx::SchemaProperty(1 ,T::type(), xmlbeansxx::StringPtr()));
      	return st;
}
    

template<class T>
class XmlArray:public XmlObject {
    
    public:
    XmlArray() {}
    
    XmlArray(const std::vector<T> &a) {
        setArray(a);
    }
    XmlArray(const std::vector<ContentsPtr> &a) {
        setArray(a);
    }
    XmlArray(const XmlObject &a) {
    	swapContents(a.contents);
    }
  
    virtual const xmlbeansxx::SchemaType *getSchemaType() const {
      static xmlbeansxx::SchemaType schemaType(XmlArray_initSchemaType<T>());
      return &schemaType; 
    }



    
    T getArray(int i) const {
        TRACER(XmlArray_log,"getArray");
        return xmlbeansxx::Contents::Walker::getElem(*this,XmlArray_elemName,i);
    }
    T cgetArray(int i) {
        TRACER(XmlArray_log,"cgetArray");
        return xmlbeansxx::Contents::Walker::cgetElem(*this,XmlArray_elemName,i);
    }
    void setArray(int i,T value) {
        TRACER(XmlArray_log,"setArray");
        xmlbeansxx::Contents::Walker::setElem(*this,XmlArray_elemName,value.contents,i);
    }
    
    T xgetArray(int i) const { return getArray(i); }
    T xcgetArray(int i) { return cgetArray(i); }
    void xsetArray(int i,T value) { return setArray(i,value); }
    
    void append(T value) {
        TRACER(XmlArray_log,"append");
        xmlbeansxx::Contents::Walker::appendElem(*this,XmlArray_elemName,value.contents);
    }
    void unset() {
        TRACER(XmlArray_log,"unset");
        xmlbeansxx::Contents::Walker::removeElems(*this,XmlArray_elemName);
    }
    
    int size() const {
        TRACER(XmlArray_log,"size");
        return xmlbeansxx::Contents::Walker::countElems(*this,XmlArray_elemName);
    }

    XmlInteger length() const {
        return XmlInteger(size());
    }

    void setArray(const std::vector<T> &vec) {
        TRACER(XmlArray_log,"setArray");
	xmlbeansxx::Contents::Walker::setElemArray(*this,XmlArray_elemName,vec);
    }
    void setArray(const std::vector<ContentsPtr> &vec) {
        TRACER(XmlArray_log,"setArray");
	xmlbeansxx::Contents::Walker::setElemArray(*this,XmlArray_elemName,vec);
    }

    std::vector<T> getArray() const {
        TRACER(XmlArray_log,"getArray");
	std::vector<T> a(xmlbeansxx::vector_conv<T>(xmlbeansxx::Contents::Walker::getElemArray(*this,XmlArray_elemName)));
   	return a;
    }
};



template<class T2,class T>
XmlArray<T2>  xmlarray_java_cast(const XmlArray<T> &from) {
    XmlArray<T2> ret;
    std::vector<T> a=from.getArray();
    for(int it=0;it<a.size();it++) {
        ret.append(java_cast<T2>(a[it]));
    }
    return ret;
}

}

#endif//_XMLBEANSXX_XMLARRAY_H_
