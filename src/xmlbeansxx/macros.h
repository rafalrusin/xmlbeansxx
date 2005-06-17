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


#ifndef _XMLBEANSXX_MACROS_H_
#define _XMLBEANSXX_MACROS_H_

#include "BoostAssert.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <typeinfo>
#include "BeansException.h"


#define DECLARE_PTR(name,name_ptr,const_name_ptr) typedef boost::shared_ptr<name> name_ptr;
#define DECLARE_PTR_CODE(name,name_ptr,const_name_ptr)

namespace xmlbeansxx {

/** 
 * Casting of boost::shared_ptr similar to java casting.
 * Eg. C++ expression:
 * java_cast<ToType>(fromVariable)
 * is similar to Java expression:
 * (ToType) fromVariable
 * It throws xmlbeansxx::ClassCastException when cast fails.
 */
template<class T2,class T>
boost::shared_ptr<T2> java_cast(const boost::shared_ptr<T> &a) {
    if (a==NULL) return boost::shared_ptr<T2>();
    else {
        boost::shared_ptr<T2> b=boost::dynamic_pointer_cast<T2>(a);
        if (b==NULL) throw ClassCastException(std::string("from ")+std::string(typeid(*a.get()).name())+std::string(" to ")+std::string(typeid(T2).name()));
        return b;
    }
}
}


#endif//_XMLBEANSXX_MACROS_H_
