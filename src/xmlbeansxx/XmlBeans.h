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

#ifndef XMLBEANSXX_XMLBEANS_INCLUDED
#define XMLBEANSXX_XMLBEANS_INCLUDED
#include "BoostAssert.h"
#include "StoreString.h"
#include "QName.h"

namespace xmlbeansxx {
class XmlBeans {
private:
public:

    /** Schema namespaceURI name. */
    static const StoreString xs_ns();
    /** Schema instance namespaceURI name. */
    static const StoreString xsi_ns();
    /** inner Type namespaceURI name */
    static const StoreString innerType_ns();
    /** xsi:type QName */
    static const QName& xsi_type();

    /** QName of the text "element" name (the string content of an xml is stored as an element of this name, this element is not visible)*/
    static const QName& textElementName();

    static const QName& xml_fragment();


    /** xsi:array QName */
    static const QName& xsi_array();

    /** xmlns namespaceURI name*/
    static const StoreString xmlns();

    /** a name prefix indicating that the element is temporary (not set in the orginal xml) */
    static const std::string& persistentPrefix();

    static std::string encoding(std::string encoding = std::string());

};
}
#endif //XMLBEANSXX_XMLBEANS_INCLUDED
