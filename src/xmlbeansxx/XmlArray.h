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


#ifdef XMLBEANSXX_LOGGING
extern XMLBEANSXX_LOGGER_PTR(XmlArray_log);
#endif

class AnyXmlArray: public XmlObject {
public:
    class Names {
    public:
    	static const QName elementName;
    };
    class Factory {
    public:
	static AnyXmlArray newInstance();
	static xmlbeansxx::XmlObjectPtr newInstanceXmlObject();
    };

    static SchemaType initSchemaType();
    virtual const xmlbeansxx::SchemaType *getOrginSchemaType() const;
    static const xmlbeansxx::SchemaType *type() ;

};

template<class T>
class XmlArray:public AnyXmlArray {
public:
    class Factory {
    public:
	static XmlArray<T> newInstance() {
	    XmlArray<T> obj;
	    obj.createContents();
	    return obj;
	}
	static xmlbeansxx::XmlObjectPtr newInstanceXmlObject(){
	    return xmlbeansxx::XmlObjectPtr(new XmlArray<T>());
	}

    };


    public:
    XmlArray() {}

    XmlArray(const std::vector<T> &a) {
    	createContents();
        setArray(a);
    }
    XmlArray(const std::vector<ContentsPtr> &a) {
    	createContents();
        setArray(a);
    }
    XmlArray(const XmlObject &p) {
    	if(!p.getSchemaType()->isArray) {
		throw xmlbeansxx::ClassCastException(p.getSchemaType()->className + " to XmlArray");
	}
	if(!p.hasContents()) {
		return ;
	}

	swapContents(p.contents);
    }

    template<class T2>
    XmlArray(const XmlArray<T2> &p) {
    	if(!_cast_test<T>(T2::Factory::newInstance())) {
		throw xmlbeansxx::ClassCastException("xmlbeansxx::XmlArray<" + T2::type()->className + ">" + " to xmlbeansxx::XmlArray<" + T::type()->className + ">");
	}
	if(!p.hasContents()) {
		return ;
	}
	swapContents(p.contents);
   }

    T getArray(int i) const {
        TRACER(XmlArray_log,"getArrayAt");
        return xmlbeansxx::Contents::Walker::getElem(*this,XmlArray::Names::elementName,i);
    }
    T cgetArray(int i) {
        TRACER(XmlArray_log,"cgetArrayAt");
        return xmlbeansxx::Contents::Walker::cgetElem(*this,XmlArray::Names::elementName,i);
    }
    XmlArray<T>& setArray(int i,const T &value) {
        TRACER(XmlArray_log,"setArrayAt");
        xmlbeansxx::Contents::Walker::setElem(*this,XmlArray::Names::elementName,value.contents,i);
	return *this;
    }

    T xgetArray(int i) const { return getArray(i); }
    T xcgetArray(int i) { return cgetArray(i); }
    XmlArray<T>& xsetArray(int i,const T &value) { return setArray(i,value); }

    XmlArray<T>& append(const T &value) {
        TRACER(XmlArray_log,"append");
        xmlbeansxx::Contents::Walker::appendElem(*this,XmlArray::Names::elementName,value.contents);
	return *this;
    }
    XmlArray<T>&  unset() {
        TRACER(XmlArray_log,"unset");
        xmlbeansxx::Contents::Walker::removeElems(*this,XmlArray::Names::elementName);
	return *this;
    }

    int size() const {
        TRACER(XmlArray_log,"size");
        return xmlbeansxx::Contents::Walker::countElems(*this,XmlArray::Names::elementName);
    }

    XmlInteger length() const {
        return XmlInteger(size());
    }

    XmlArray<T>& setArray(const std::vector<T> &vec) {
        TRACER(XmlArray_log,"setArrayT");
	xmlbeansxx::Contents::Walker::setElemArray(*this,XmlArray::Names::elementName,vec);
	return *this;
    }
    XmlArray<T>&  setArray(const std::vector<ContentsPtr> &vec) {
        TRACER(XmlArray_log,"setArray");
	xmlbeansxx::Contents::Walker::setElemArray(*this,XmlArray::Names::elementName,vec);
	return *this;
    }

    std::vector<T> getArray() const {
        TRACER(XmlArray_log,"getArray");
	std::vector<T> a(xmlbeansxx::vector_conv<T>(xmlbeansxx::Contents::Walker::getElemArray(*this,XmlArray::Names::elementName)));
   	return a;
    }
};



template<class T2,class T>
XmlArray<T2>  xmlarray_java_cast(const XmlArray<T> &from) {
    XmlArray<T2> ret=XmlArray<T2>::Factory::newInstance();
    std::vector<T> a=from.getArray();
    for(typename std::vector<T>::size_type it=0;it<a.size();it++) {
        ret.append(java_cast<T2>(a[it]));
    }
    return ret;
}

}

#endif//_XMLBEANSXX_XMLARRAY_H_
