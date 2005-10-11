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
#include <boost/enable_shared_from_this.hpp>
#include "cast.h"
#include "Null.h"
namespace xmlbeansxx {

//based on shared_ptr

class Ptr {
    protected:
    static log4cxx::LoggerPtr LOG();
    public:
    template <typename TYPE, typename TYPE_I>
    static inline TYPE ptrFromThis(const boost::shared_ptr<TYPE_I> &from) {
        TYPE n;
        n.ptr = from;
        return n;
    }
};


template <typename TYPE, typename TYPE_I>
class PtrBase: public Ptr {
    friend class PtrFromThis;
    private:
    static inline TYPE create(TYPE_I *ptr) {
        TYPE n;
        n.ptr = boost::shared_ptr<TYPE_I>(ptr);
        return n;
    }
    
    public:
    boost::shared_ptr<TYPE_I> ptr;

    public:
    typedef TYPE_I value_type; 
    
    inline TYPE_I *operator ->() const {
        //if (ptr == NULL) {
        //    LOG4CXX_ERROR(LOG(), "Null Pointer Dereference Error");
        //}
        //BOOST_ASSERT(ptr != NULL); 
        return &(*ptr); 
    }
    //inline TYPE_I &operator *() const {
    //    return *ptr;
    //}
    operator bool() { return (bool) ptr; }
    static inline TYPE New() { TYPE_I *ptr = new TYPE_I(); return create(ptr); }
    template <typename P1>
    static inline TYPE New(const P1 &p1) { TYPE_I *ptr = new TYPE_I(p1); return create(ptr); }
    template <typename P1, typename P2>
    static inline TYPE New(const P1 &p1, const P2 &p2) { TYPE_I *ptr = new TYPE_I(p1, p2); return create(ptr); }
};

template <typename T>
class EnablePtrFromThis: public boost::enable_shared_from_this<T>  {
    protected:
    template <typename T2>
    inline T2 ptrFromThis() { T2 r; r.ptr = shared_from_this(); return r; }
};

/*
//based on *
class Ptr {
    protected:
    static log4cxx::LoggerPtr LOG();
};


template <typename TYPE, typename TYPE_I>
class PtrBase: public Ptr {
    friend class PtrFromThis;
    private:
    static inline TYPE create(TYPE_I *ptr) {
        TYPE n;
        n.ptr = ptr;
        return n;
    }
    
    public:
    TYPE_I *ptr;
    PtrBase(): ptr(NULL) {}

    public:
    typedef TYPE_I value_type; 
    
    inline TYPE_I *operator ->() const {
        //if (ptr == NULL) {
        //    LOG4CXX_ERROR(LOG(), "Null Pointer Dereference Error");
        //}
        //BOOST_ASSERT(ptr != NULL); 
        return ptr; 
    }
    //inline TYPE_I &operator *() const {
    //    return *ptr;
    //}
    operator bool() { return (bool) ptr; }
    static inline TYPE New() { TYPE_I *ptr = new TYPE_I(); return create(ptr); }
    template <typename P1>
    static inline TYPE New(const P1 &p1) { TYPE_I *ptr = new TYPE_I(p1); return create(ptr); }
    template <typename P1, typename P2>
    static inline TYPE New(const P1 &p1, const P2 &p2) { TYPE_I *ptr = new TYPE_I(p1, p2); return create(ptr); }
};

template <typename T>
class EnablePtrFromThis {
    protected:
    template <typename T2>
    inline T2 ptrFromThis() { T2 r; r.ptr = (T *) this; return r; }
};
*/

#define BEGIN_CLASS(TYPE, TYPE_I) \
class TYPE: public xmlbeansxx::PtrBase<TYPE, TYPE_I> {\
    public: \
    TYPE() {} \
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
        TO r;
        r.ptr = ptr_cast<typename TO::value_type>(from.ptr);
        return r;
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
