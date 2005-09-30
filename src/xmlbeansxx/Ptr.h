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
    
#ifndef XMLBEANSXX_PTR_INCLUDED
#define XMLBEANSXX_PTR_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/mpl/and.hpp>
#include <boost/shared_ptr.hpp>
#include "cast.h"
#include "Null.h"
namespace xmlbeansxx {

class Ptr {};

template <typename TYPE, typename TYPE_I>
class PtrBase: public Ptr {
    public:
    boost::shared_ptr<TYPE_I> ptr;
    typedef TYPE_I value_type; 
    
    TYPE_I *operator ->() const { 
        if (ptr == NULL) {
            std::cerr << "Null Pointer Dereference Error" << std::endl;
        }
        //BOOST_ASSERT(ptr != NULL); 
        return ptr.get(); 
    }
    operator bool() { return (bool) ptr; }
    static TYPE New() { TYPE n; n.ptr = boost::shared_ptr<TYPE_I>(new TYPE_I()); return n; }
    template <typename P1>
    static TYPE New(const P1 &p1) { TYPE n; n.ptr = boost::shared_ptr<TYPE_I>(new TYPE_I(p1)); return n; }
    template <typename P1, typename P2>
    static TYPE New(const P1 &p1, const P2 &p2) { TYPE n; n.ptr = boost::shared_ptr<TYPE_I>(new TYPE_I(p1, p2)); return n; }
};

#define BEGIN_CLASS(TYPE, TYPE_I) \
class TYPE: public xmlbeansxx::PtrBase<TYPE, TYPE_I> {\
    public: \
    TYPE() {} \
    TYPE(const boost::shared_ptr<TYPE_I> &_ptr) { ptr = _ptr; } \
    TYPE(const xmlbeansxx::Null &null) {} \
    template <typename T> \
    TYPE(const T& from, \
            typename boost::enable_if< \
                boost::is_base_and_derived<Ptr, T> \
            >::type *dummy = 0) { ptr = from.ptr; }
    

#define END_CLASS() \
};

template<typename TO, typename FROM>
class cast_traits<TO, FROM, 
        typename boost::enable_if<
            boost::mpl::and_<
                boost::is_base_and_derived<Ptr, FROM>,
                boost::is_base_and_derived<Ptr, TO>
            >
        >::type 
    > {
    public:
    TO operator ()(const FROM &from) const {
        return TO(ptr_cast<typename TO::value_type>(from.ptr));
    }
};

template<typename X, typename Y>
typename boost::enable_if<
            boost::mpl::and_<
                boost::is_base_and_derived<Ptr, X>,
                boost::is_base_and_derived<Ptr, Y>
            >,bool>::type
operator == (const X& x, const Y &y) {
    return x.ptr == y.ptr;
}

template<typename X, typename Y>
typename boost::enable_if<
            boost::mpl::and_<
                boost::is_base_and_derived<Ptr, X>,
                boost::is_base_and_derived<Ptr, Y>
            >,bool>::type
operator != (const X& x, const Y &y) {
    return x.ptr != y.ptr;
}


}
#endif //XMLBEANSXX_PTR_INCLUDED
