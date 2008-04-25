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


#ifndef _XMLBEANSXX_XMLOPTIONS_H_
#define _XMLBEANSXX_XMLOPTIONS_H_

#include "BoostAssert.h"
#include <boost/shared_ptr.hpp>



namespace xmlbeansxx {

class XmlOptions {
    bool v,d,tp,sit,st,pers;
public:
    XmlOptions():v(false),d(true),tp(true),st(false),sit(false),pers(false) {}

    void setValidation(bool v) {
        XmlOptions::v=v;
    }
    bool getValidation() {
        return v;
    }
    void setPrintXmlDeclaration(bool d) {
        XmlOptions::d=d;
    }
    bool getPrintXmlDeclaration() {
        return d;
    }

    bool getTypedParsing() {
        return tp;
    }
    void setTypedParsing(bool v) {
        tp=v;
    }
    void setSerializeInnerTypes(bool v) {
        if(v) setSerializeTypes(v);
        sit=v;
    }
    bool getSerializeInnerTypes() {
        return getSerializeTypes() && sit;
    }

    void setSerializeTypes(bool v) {
        st=v;
    }
    bool getSerializeTypes() {
        return st;
    }

    bool getSerializePersistent() {
        return pers;
    }
    void setSerializePersistent(bool v) {
    	if(v) setSerializeInnerTypes(true);
	pers=v;
    }
    
    static XmlOptions serializeInnerTypes() { 
    	XmlOptions o;
	o.setSerializeInnerTypes(true);
	return o;
    }
    static XmlOptions serializePersistent() { 
    	XmlOptions o;
	o.setSerializePersistent(true);
	return o;
    }
    static XmlOptions serializeTypes() { 
    	XmlOptions o;
	o.setSerializeTypes(true);
	return o;
    }
};

}
#endif//_XMLBEANSXX_XMLOPTIONS_H_
