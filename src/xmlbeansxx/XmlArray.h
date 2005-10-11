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
#include "cast.h"
#include "Ptr.h"
#include "Array.h"
#include <log4cxx/logger.h>

#include "xmlbeansxx-schema.h"

namespace xmlbeansxx {

template<typename T>
class cast_traits<XmlArray, Array<T>,  
        typename boost::enable_if<
                boost::is_base_and_derived<Ptr, T>
        >::type > {
    public:
    XmlArray operator ()(const Array<T> &from) const {
        XmlArray n(XmlArray::Factory::newInstance());
        n->setEArray(cast<Array<XmlObject> >(from));
        return n;
    }
};

template<typename T>
class cast_traits<Array<T>, XmlArray,  
        typename boost::enable_if<
                boost::is_base_and_derived<Ptr, T>
        >::type > {
    public:
    Array<T> operator ()(const XmlArray &from) const {
        return cast<Array<T> >(from->getEArray());
    }
};

}

#endif//_XMLBEANSXX_XMLARRAY_H_
