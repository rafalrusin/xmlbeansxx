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
#include "StoreString.h"
#include "BeansException.h"
#include "XmlTypesGen.h"

namespace xmlbeansxx {


StoreString XmlBeans::xs_ns() {
    static StoreString ns = StoreString::store("http://www.w3.org/2001/XMLSchema");
    return ns;
}

StoreString XmlBeans::xsi_ns() {
    static StoreString ns = StoreString::store("http://www.w3.org/2001/XMLSchema-instance");
    return ns;
}

StoreString XmlBeans::innerType_ns() {
    static StoreString ns = StoreString::store("http://xmlbeansxx.touk.pl/xmlbeansxx/innerType");
    return ns;
}

QName XmlBeans::xsi_type() {
    static QName n = QName::store("http://www.w3.org/2001/XMLSchema-instance","type");
    return n;
}

QName XmlBeans::xsi_array() {
    static QName n = QName::store("http://www.w3.org/2001/XMLSchema-instance","array");
    return n;
}


}

