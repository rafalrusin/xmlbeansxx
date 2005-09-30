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


#ifndef _XMLBEANSXX_CAST_H_
#define _XMLBEANSXX_CAST_H_

#include "BoostAssert.h"
#include <boost/shared_ptr.hpp>
#include <typeinfo>
#include "BeansException.h"


namespace xmlbeansxx {

/**
 * Casting of boost::shared_ptr similar to java casting.
 * Eg. C++ expression:
 * throwing_pointer_cast<ToType>(fromVariable)
 * is similar to Java expression:
 * (ToType) fromVariable
 * It throws xmlbeansxx::ClassCastException while cast fails.
 */
template<class T2,class T>
boost::shared_ptr<T2> throwing_pointer_cast(const boost::shared_ptr<T> &a) {
    if (a==NULL) return boost::shared_ptr<T2>();
    else {
        boost::shared_ptr<T2> b=boost::dynamic_pointer_cast<T2>(a);
        if (b==NULL) throw ClassCastException(String("from ")+std::string(typeid(*a.get()).name())+std::string(" to ")+std::string(typeid(T2).name()));
        return b;
    }
}

template<class T2,class T>
boost::shared_ptr<T2> ptr_cast(const boost::shared_ptr<T> &a) {
    return throwing_pointer_cast<T2>(a);
}



template<typename TO, typename FROM, typename Enable = void>
class cast_traits {
    public:
    TO operator ()(const FROM &from) const {
        return (TO)(from);
    }
};

template<typename TO, typename FROM>
class cast_traits<boost::shared_ptr<TO>, boost::shared_ptr<FROM> > {
    public:
    boost::shared_ptr<TO> operator ()(const boost::shared_ptr<FROM> &from) const {
        return ptr_cast<TO>(from);
    }
};

template<typename TO, typename FROM>
TO cast(const FROM &from) {
    return cast_traits<TO, FROM>()(from);
}


template <typename Q>
class cast_box_class {
    const Q &item;
    public:
    cast_box_class(const Q &item): item(item) {}
    template <typename T> 
    operator T() {
        return cast<T>(item);
    }
};

template <typename T>
cast_box_class<T> cast_box(const T &item) {
    return cast_box_class<T>(item);
}
}


#endif//_XMLBEANSXX_CAST_H_
