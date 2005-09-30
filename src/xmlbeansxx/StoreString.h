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


#ifndef _XMLBEANSXX_STORESTRING_H_
#define _XMLBEANSXX_STORESTRING_H_

#include "BoostAssert.h"
//#include <set>
#include <string>
#include <cstring>
#include <log4cxx/logger.h>
#include "Map.h"
#include "Vector.h"
#include <boost/scoped_ptr.hpp>
#include "Existence.h"
#include "String.h"


namespace xmlbeansxx {

/** Immutable string with automatic memory deallocation */
class SimpleString {
private:
    char *buf;
    void copyFrom(const SimpleString &from);
public:
    SimpleString(const String &s);
    SimpleString(const SimpleString &from);
    ~SimpleString();
    SimpleString &operator=(const SimpleString &b);

    const char *c_str() const;
};

/* Immutable string without memory deallocation */
/*
class CStr {
private:
const char *buf;
public:
CStr(const char *buf);
const char *get() const;
bool operator<(const CStr &b) const;
};*/

class CStrHashFn {
public:
    unsigned int operator()(const char *str) const {
        unsigned int v=0;
        while (*str!=0) {
            v+=(unsigned int)(*str)*31;
            str++;
        }
        return v;
    }
};

class CStrLessFn {
public:
    bool operator()(const char *a,const char *b) const {
        return strcmp(a,b)<0;
    }
};

class CStrEqFn {
public:
    bool operator()(const char *a,const char *b) const {
        if (a==b) return true;
        else return strcmp(a,b)==0;
    }
};

class StringStorage;

/**
* Immutable string with ability to remember globally some strings. User can use static method 'store' to remember some strings.
* Construction of StoreString(s) with not remembered s causes duplication of s.
*/
class StoreString {
private:
    static log4cxx::LoggerPtr LOG;
private:
    static StringStorage *storage;
private:
    unsigned long buf;
    bool isStored() const;

    void construct(const char *str);
    inline void copyFrom(const StoreString &from);

class MyExistence_I: public Existence_I {
    public:
        MyExistence_I();
        virtual ~MyExistence_I();
    };

public:
    static Existence staticInit();

    StoreString();
    StoreString(const StoreString &from);
    StoreString(const String &str);
    StoreString(const char *str);
    ~StoreString();
    const char *c_str() const;
    unsigned int hashCode() const;

    inline StoreString &operator=(const StoreString &b);
    inline bool operator ==(const StoreString &str) const;
    inline bool operator <(const StoreString &str) const;

    inline operator String() const;

    String toString() const;
    /** Remembers a string in collection, so it won't be allocated several times. */
    static StoreString store(const String &str);

    class Hash {
    public:
        unsigned int operator()(const StoreString &ss) const {
            return ss.hashCode();
        }
    };
};

inline std::ostream &operator<<(std::ostream &out, const StoreString &str);
inline String operator+(const std::string &a,const StoreString &b);
inline String operator+(const StoreString &a,const std::string &b);
inline String operator+(const StoreString &a,const StoreString &b);

inline bool operator==(const String &a,const StoreString &b);
inline bool operator==(const StoreString &a,const String &b);

//Inline functions
inline StoreString &StoreString::operator=(const StoreString &b) {
    //TRACER(log,"operator=");
    copyFrom(b);
    return *this;
}

inline void StoreString::copyFrom(const StoreString &from) {
    if (from.isStored()) {
        buf=from.buf;
    } else {
        construct(from.c_str());
    }
}

inline bool StoreString::operator ==(const StoreString &str) const {
    if (isStored() && str.isStored()) return buf==str.buf;
    else return CStrEqFn()(c_str(),str.c_str());
}

inline bool StoreString::operator <(const StoreString &str) const {
    return strcmp(c_str(), str.c_str()) < 0;
}

inline StoreString::operator String() const {
    return String(const_cast<char *>(c_str()));
}

inline std::ostream &operator<<(std::ostream &out, const StoreString &str) {
    return out<<str.c_str();
}

inline String operator+(const std::string &a,const StoreString &b) {
    return a+b.toString();
}

inline String operator+(const StoreString &a,const std::string &b) {
    return a.toString()+b;
}

inline String operator+(const StoreString &a,const StoreString &b) {
    return a.toString()+b.toString();
}

inline bool operator==(const String &a,const StoreString &b) {
    return CStrEqFn()(a.c_str(),b.c_str());
}

inline bool operator==(const StoreString &a,const String &b) {
    return CStrEqFn()(a.c_str(),b.c_str());
}



}

#endif//_XMLBEANSXX_STORESTRING_H_


