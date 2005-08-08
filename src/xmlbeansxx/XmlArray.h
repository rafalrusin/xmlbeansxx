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
#include <log4cxx/logger.h>

#include "shared_array.h"
#include "XmlTypesGen.h"
#include "macros.h"
#include "BeansException.h"
#include "Tracer.h"
#include "Dict.h"
//#include "XmlTypesGen.h"


	
namespace xmlbeansxx {

extern log4cxx::LoggerPtr XmlArray_log;
extern std::string XmlArray_elemName;

template<class T>
static SchemaType XmlArray_initSchemaType() {
      xmlbeansxx::SchemaType st(typeid(T));
      st.createFn=T::Factory::newXmlArrayInstance;
      st.createArrayFn=NULL;
      st.whitespaceRule=xmlbeansxx::SchemaType::WS_COLLAPSE;
      XmlObjectPtr o=T::Factory::newInstance();
      st.className=o->getSchemaType()->className;
      st.xsdTypeName=o->getSchemaType()->xsdTypeName;
      st.xsdNamespaceID=o->getSchemaType()->xsdNamespaceID;
      st.arrayXsdTypeName=st.xsdTypeName;
      st.arrayXsdNamespaceID=st.xsdNamespaceID;
      st.processContents=true;
      return st;
}


/** 
 * @deprecated
 * This is an array of XmlObjects, which is an XmlObject that one can serialize to xml and parse from xml. 
 */
template<class T>
class XmlArray:public XmlObject {
    T elemObj;
    
    public:
    XmlArray() {}
    
    XmlArray(const shared_array<XmlObjectPtr> &a) {
        setArray(a);
    }
  
    virtual const xmlbeansxx::SchemaType *getSchemaType() const {
      static xmlbeansxx::SchemaType schemaType(XmlArray_initSchemaType<T>());
      return &schemaType; 
    }

    /*
    //virtual methods
    virtual xmlbeansxx::XmlObjectPtr newSameType() const {
        return boost::shared_ptr<XmlArray<T> >(new XmlArray<T>());
    }
    virtual std::string arrayXsdTypeName() const {
        return elemObj.xsdTypeName();
    }
    virtual int arrayXsdNamespace() const {
        return elemObj.xsdNamespace();
    }
    
    virtual bool processContents() const {
        return true;
    }
    //~virtual methods
    */
    
    boost::shared_ptr<T> getArray(int i) const {
        TRACER(XmlArray_log,"getArray");
        return java_cast<T>(contents.getElemAt(XmlArray_elemName,i));
    }
    boost::shared_ptr<T> cgetArray(int i,xmlbeansxx::ObjectCreatorFn createFn=NULL) {
        TRACER(XmlArray_log,"cgetArray");
		if (createFn==NULL) createFn=T::Factory::newInstanceXmlObject;
        return java_cast<T>(contents.cgetElemAt(XmlArray_elemName,i,createFn,this));
    }
    void setArray(int i,const boost::shared_ptr<T> &value) {
        TRACER(XmlArray_log,"setArray");
        contents.setElemAt(XmlArray_elemName,i,value);
    }
    
    boost::shared_ptr<T> xgetArray(int i) const { return getArray(i); }
    boost::shared_ptr<T> xcgetArray(int i,xmlbeansxx::ObjectCreatorFn createFn=NULL) { return cgetArray(i,createFn); }
    void xsetArray(int i,const boost::shared_ptr<T> &value) { return setArray(i,value); }
    
    void append(const boost::shared_ptr<T> &value) {
        TRACER(XmlArray_log,"append");
        contents.appendElem(XmlArray_elemName,value);
    }
    void unset() {
        TRACER(XmlArray_log,"unset");
        contents.removeElems(XmlArray_elemName);
    }
    
    int size() const {
        TRACER(XmlArray_log,"size");
        return contents.countElems(XmlArray_elemName);
    }

    XmlInteger length() const {
        return XmlInteger(size());
    }

    void setArray(const shared_array<XmlObjectPtr> &vec) {
        TRACER(XmlArray_log,"setArray");
        contents.setElemArray(XmlArray_elemName,vec);
    }

    shared_array<XmlObjectPtr> getArray() const {
        TRACER(XmlArray_log,"getArray");
        return contents.getElemArray(XmlArray_elemName);
    }
};


template<class T2,class T>
boost::shared_ptr<XmlArray<T2> > xmlarray_java_cast(const boost::shared_ptr<XmlArray<T> > &from) {
    boost::shared_ptr<XmlArray<T2> > ret(new XmlArray<T2>());
    shared_array<XmlObjectPtr> a=from->getArray();
    for(int it=0;it<a.size();it++) {
        ret->append(java_cast<T2>(a[it]));
    }
    return ret;
}

}

#endif//_XMLBEANSXX_XMLARRAY_H_
