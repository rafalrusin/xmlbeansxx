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


#ifndef _XMLBEANSXX_SHARED_ARRAY_H_
#define _XMLBEANSXX_SHARED_ARRAY_H_

#include "BoostAssert.h"
#include <boost/shared_ptr.hpp>
#include "cast.h"
#include "Vector.h"

namespace xmlbeansxx {

/** This is shared array of some type with variable size. This is similar to boost::shared_ptr<Vector<T>::type >, but is more flexible to use. */
template<typename T>
class Array {
    boost::shared_ptr<typename Vector<T>::type > repr;

    void init(int size) {
        repr = boost::shared_ptr<typename Vector<T>::type >(new typename Vector<T>::type(size));
    }

public:
    /** Creates Array of size 0. */
    Array() {
        init(0);
    }

    /** Creates Array of size n by allocating n elements. */
    Array(int n) {
        init(n);
    }
    
    T &operator [](int position) {
        return (*repr)[position];
    }
    
    const T &operator [](int position) const {
        return (*repr)[position];
    }
    
    /** Copy constructor. */
    Array(const Array<T> &b) {
        repr = b.repr;
    }

    /** @return size of this Array. */
    int size() const {
        return (*repr).size();
    }

    void push_back(const T &what) const {
        (*repr).push_back(what);
    }

    /** Begin iterator. */
    typename Vector<T>::type::iterator begin() const {
        return (*repr).begin();
    }

    /** End iterator. */
    typename Vector<T>::type::iterator end() const {
        return (*repr).end();
    }    
};

template<typename TO, typename FROM>
class cast_traits<Array<TO>, Array<FROM> > {
    public:
    Array<TO> operator ()(const Array<FROM> &from) const {
        Array<TO> n(from.size());
        for(int i=0;i<from.size();i++) {
            n[i] = cast<TO>(from[i]);
        }
        return n;
    }
};


}

#endif //_XMLBEANSXX_SHARED_ARRAY_H_


