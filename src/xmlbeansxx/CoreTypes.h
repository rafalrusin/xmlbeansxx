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


#ifndef _XMLBEANSXX_CORE_TYPES_H_
#define _XMLBEANSXX_CORE_TYPES_H_

#include "BoostAssert.h"
#include <boost/shared_array.hpp>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <gmp.h>
#include <gmpxx.h>

namespace xmlbeansxx {
    
    /** 
     * Objects of this class represent qualified name, which contains namespace URI and localPart. Corresponds to javax.xml.namespace.QName class.
     * TODO: implementation
     */
    class QName {
    };
    typedef boost::shared_ptr<QName> QNamePtr;

    /**
     * Objects of this class represent some calendar time. Corresponds to java.util.Calendar class.
     * TODO: implementation
     */
    class Calendar {
    };
    typedef boost::shared_ptr<Calendar> CalendarPtr;
    
    /** 
     * Represents xml schema compatible Gregorian Date. Corresponds to org.apache.xmlbeans.GDate class.
     * TODO: implementation
     */
    class GDate {
    };
    typedef boost::shared_ptr<GDate> GDatePtr;

    /**
     * Represents list of Objects. Corresponds to java.util.List class.
     * TODO: implementation
     */
    class List {
    };
    typedef boost::shared_ptr<List> ListPtr;

    /**
     * Represents some object. Corresponds to java.lang.Object class.
     * This is mock class for code generator.
     */
    class Object {
        public:
        virtual std::string toString() const { return ""; }
    };
    typedef boost::shared_ptr<Object> ObjectPtr;
}

#endif //_XMLBEANSXX_CORE_TYPES_H_


