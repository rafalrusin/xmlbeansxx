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
    
#include "XmlBeans.h"
#include "StoreString.h"
#include "BeansException.h"
#include "XmlTypesGen.h"

namespace xmlbeansxx {


const StoreString XmlBeans::xs_ns() {
    static StoreString ns = StoreString::store("http://www.w3.org/2001/XMLSchema");
    return ns;
}

const StoreString XmlBeans::xsi_ns() {
    static StoreString ns = StoreString::store("http://www.w3.org/2001/XMLSchema-instance");
    return ns;
}

const StoreString XmlBeans::innerType_ns() {
    static StoreString ns = StoreString::store("http://xmlbeansxx.touk.pl/xmlbeansxx/innerType");
    return ns;
}


const StoreString XmlBeans::xmlns() {
    static StoreString ns = StoreString::store("http://www.w3.org/2000/xmlns/");
    return ns;
}

const QName& XmlBeans::textElementName() {
    static QName n = QName::store("","");
    return n;
}

const QName& XmlBeans::xml_fragment() {
    static QName n = QName::store("http://xmlbeans.apache.org/definitions", "xml-fragment");
    return n;
}


const QName& XmlBeans::xsi_type() {
    static QName n = QName::store("http://www.w3.org/2001/XMLSchema-instance","type");
    return n;
}

const QName& XmlBeans::xsi_array() {
    static QName n = QName::store("http://www.w3.org/2001/XMLSchema-instance","array");
    return n;
}

const std::string& XmlBeans::persistentPrefix() {
	static std::string p = "xbsxxP_";
	return p;
}



std::string XmlBeans::encoding(std::string encoding) {
	static std::string encode="utf8";
	if(encoding.size()>0) encode = encoding;
	return encode;
}



}

