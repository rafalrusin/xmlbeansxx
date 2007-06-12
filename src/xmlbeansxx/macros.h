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

//Common templeate definitions
/*
typedef boost::shared_ptr<std::string> StringPtr;
typedef boost::shared_ptr<const std::string> constStringPtr;
typedef std::map<std::string,int> Map_String_Int;
*/
/*
#include <boost/shared_ptr.hpp>
 
 
#define DECLARE_PTR(name,name_ptr,const_name_ptr) \
struct name_ptr: boost::shared_ptr<name> { \
    name_ptr(); \
    name_ptr(const name_ptr &b); \
    name_ptr(name *b); \
};\
struct const_name_ptr: boost::shared_ptr<const name> { \
    const_name_ptr(); \
    const_name_ptr(const const_name_ptr &b); \
    const_name_ptr(const name *b); \
};
 
#define DECLARE_PTR_CODE(name,name_ptr,const_name_ptr) \
name_ptr::name_ptr() {} \
name_ptr::name_ptr(const name_ptr &b): boost::shared_ptr<name>(b) {} \
name_ptr::name_ptr(name *b): boost::shared_ptr<name>(b) {} \
\
const_name_ptr::const_name_ptr() {} \
const_name_ptr::const_name_ptr(const const_name_ptr &b): boost::shared_ptr<const name>(b) {} \
const_name_ptr::const_name_ptr(const name *b): boost::shared_ptr<const name>(b) {} \
*/


//#define DECLARE_PTR(name,name_ptr,const_name_ptr) 
//    typedef boost::shared_ptr<name> name_ptr;
//    typedef boost::shared_ptr<const name> const_name_ptr;

#define DECLARE_PTR(name,name_ptr,const_name_ptr) typedef boost::shared_ptr<name> name_ptr;
#define DECLARE_PTR_CODE(name,name_ptr,const_name_ptr)




namespace xmlbeansxx {

/*
template<class T2,class T>
std::vector<boost::shared_ptr<T2> > dynamic_vector_ptr_cast(const std::vector<boost::shared_ptr<T> > &a) {
    std::vector<boost::shared_ptr<T2> > ret;
    typename std::vector<boost::shared_ptr<T> >::const_iterator it=a.begin();
    for(;it!=a.end();it++) {
        ret.push_back(boost::dynamic_pointer_cast<T2>(*it));
    }
    return ret;
}

template<class T2,class T>
std::vector<boost::shared_ptr<T2> > static_vector_ptr_cast(const std::vector<boost::shared_ptr<T> > &a) {
    std::vector<boost::shared_ptr<T2> > ret;
    typename std::vector<boost::shared_ptr<T> >::const_iterator it=a.begin();
    for(;it!=a.end();it++) {
        ret.push_back(boost::static_pointer_cast<T2>(*it));
    }
    return ret;
}*/

/*template<class T2,class T>
boost::shared_ptr<T2> java_cast(const boost::shared_ptr<T> &a) {
    if (a==NULL) return boost::shared_ptr<T2>();
    else {
        boost::shared_ptr<T2> b=boost::dynamic_pointer_cast<T2>(a);
        if (b==NULL) throw ClassCastException(std::string("from ")+std::string(typeid(*a.get()).name())+std::string(" to ")+std::string(typeid(T2).name()));
        return b;
    }
}
*/

template<class T2,class T>
T2 java_cast(const T &a) {
	return T2(a);
}



/*
template<class T2,class T>
std::vector<boost::shared_ptr<T2> > vector_java_cast(const std::vector<boost::shared_ptr<T> > &a) {
    std::vector<boost::shared_ptr<T2> > ret;
    typename std::vector<boost::shared_ptr<T> >::const_iterator it=a.begin();
    for(;it!=a.end();it++) {
        ret.push_back(java_cast<T2>(*it));
    }
    return ret;
}*/

}


#endif//_XMLBEANSXX_MACROS_H_
