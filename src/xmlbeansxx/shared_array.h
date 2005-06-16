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
#include <boost/shared_array.hpp>
#include <vector>
#include "macros.h"

namespace xmlbeansxx {

    /** This is shared array of some type with constant size. This is similar to boost::shared_array, but is more flexible to use. */
    template<typename T>
    class shared_array: public boost::shared_array<T> {
        int p_size;
        public:
        
        class iterator {
            int p;
            shared_array<T> a;
            public:
            iterator(int p,shared_array<T> a): p(p), a(a) {
            }

            T operator *() {
                return a[p];
            }

            iterator &operator ++() {
                p++;
                return *this;
            }
            iterator &operator --() {
                p--;
                return *this;
            }

            bool operator ==(const iterator &it2) {
                return p==it2.p;
            }

            bool operator !=(const iterator &it2) {
                return p!=it2.p;
            }
        };
        
        /** Creates shared_array of size 0. */
        shared_array(): p_size(0) {}
        
        /** Creates shared_array of size n by allocating n elements. */
        shared_array(int n): boost::shared_array<T>(new T[n]), p_size(n) {} 
        
        /** Copy constructor. */
        template<typename U>
        shared_array(const shared_array<U> &b): boost::shared_array<T>(b), p_size(b.size()) {}

        /** @return size of this shared_array. */
        int size() const { return p_size; }

        /** Converts this array to std::vector by creating new vector. */
        std::vector<T> toVector() const {
            std::vector<T> v(p_size);
            for(int i=0;i<p_size;i++) {
                v.push_back(a[i]);
            }
            return v;
        }
        
        /** Begin iterator. */
        iterator begin() { return iterator(0,*this); }
        
        /** End iterator. */
        iterator end() { return iterator(size(),*this); }
    };

    /** Converts given std::vector to shared_array */
    template<typename T>
    shared_array<T> toSharedArray(const std::vector<T> &v) {
        shared_array<T> a(v.size());
        for(int i=0;i<a.size();i++) a[i]=v[i];
        return a;
    }


    template<typename T, typename U>
    shared_array<boost::shared_ptr<T> > static_array_cast(const shared_array<boost::shared_ptr<U> > &a) {
        shared_array<boost::shared_ptr<T> > b(a.size());
        for(int i=0;i<a.size();i++) b[i]=boost::static_pointer_cast<T>(a[i]);
        return b;
    }

    template<typename T, typename U>
    shared_array<boost::shared_ptr<T> > dynamic_array_cast(const shared_array<boost::shared_ptr<U> > &a) {
        shared_array<boost::shared_ptr<T> > b(a.size());
        for(int i=0;i<a.size();i++) b[i]=boost::dynamic_pointer_cast<T>(a[i]);
        return b;
    }

    template<typename T, typename U>
    shared_array<boost::shared_ptr<T> > java_array_cast(const shared_array<boost::shared_ptr<U> > &a) {
        shared_array<boost::shared_ptr<T> > b(a.size());
        for(int i=0;i<a.size();i++) b[i]=java_cast<T>(a[i]);
        return b;
    }


}

#endif //_XMLBEANSXX_SHARED_ARRAY_H_


