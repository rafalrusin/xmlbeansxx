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
#ifndef XMLBEANSXX_QNAME_INCLUDED
#define XMLBEANSXX_QNAME_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "StoreString.h"
namespace xmlbeansxx {
/**
* Objects of this class represent qualified name, which contains namespace URI and localPart. Corresponds to javax.xml.namespace.QName class.
*/
class QName: public std::pair<StoreString, StoreString> {
public:
    QName *operator -> () { return this; }
    const QName *operator -> () const { return this; }
    QName(const StoreString &namespacePrefix, const StoreString& localPart) : std::pair<StoreString, StoreString>(namespacePrefix, localPart) {}
    QName() {
        first = "";
        second = "";
    }

    class Hash {
    public:
        unsigned int operator()(const QName &name) const {
            return name.first.hashCode() * 31 ^ name.second.hashCode();
        }
    };
 
    String toString() const {
        return second + String("@") + first;
    }
};

}
#endif //XMLBEANSXX_QNAME_INCLUDED
