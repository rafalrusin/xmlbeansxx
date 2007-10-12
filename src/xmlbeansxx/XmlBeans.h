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

#ifndef XMLBEANSXX_XMLBEANS_INCLUDED
#define XMLBEANSXX_XMLBEANS_INCLUDED
#include "BoostAssert.h"
#include "StoreString.h"
#include "QName.h"

namespace xmlbeansxx {
class XmlBeans {
private:
public:
    
    /** Schema namespace name. */
    static StoreString xs_ns();
    /** Schema instance namespace name. */
    static StoreString xsi_ns();
    /** inner Type namespace name */
    static StoreString innerType_ns();
    /** xsi:type QName */
    static QName xsi_type();

    /** xsi:array QName */    
    static QName xsi_array();
    
   
    static std::string encoding(std::string encoding = std::string());
    
};
}
#endif //XMLBEANSXX_XMLBEANS_INCLUDED
