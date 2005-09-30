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


#include "StoreString.h"
#include "BeansException.h"
#include "defs.h"
#include "Tracer.h"
#include "TextUtils.h"

#include <cstring>



namespace xmlbeansxx {
//------------------------------------------------

namespace {


char *duplicate(const char *str) {
    //static log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger("xmlbeansxx.duplicate");

    char *buf;
    int l=strlen(str)+1;
    buf=new char[l];

    //LOG4CXX_DEBUG(logger,"duplicate:"<<str<<" ptr:"<<TextUtils::ptrToString(buf));

    if (!((((unsigned long)buf)&1)==0)) {
        /* 
         * A c_str string is remembered if and only if it's first bit in pointer is set to 1. 
         * This method assumes that 'new' operator always returns even pointer. 
         * This may cause portability problems.
         */
        //LOG4CXX_FATAL(LOG,"Memory allocation using new operator returned odd pointer. This is not supported.");
        std::cerr << "Memory allocation using new operator returned odd pointer. This is not supported." << std::endl;
        throw IllegalStateException();
    }
    //strcpy(buf,str);
    FOR(i,l) buf[i]=str[i];
    return buf;
}

void freeDuplicate(char *str) {
    //static log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger("xmlbeansxx.freeDuplicate");
    //LOG4CXX_DEBUG(logger,"freeDuplicate:"<<str<<" ptr:"<<TextUtils::ptrToString(str));
    delete[] str;
}
}

/** String storage remembers a collection of const char * strings. One can add some strings for further use. */
class StringStorage {
private:
    //std::set<CStr> contents;
    //typedef std::set<const char *,CStrLessFn> StoreSet;
    typedef Map<const char *, int, CStrHashFn, CStrEqFn>::type StoreMap;
    StoreMap contents;
public:
    struct SSInfo {
        const char *str;
        int hashCode;
    };
    
    Vector<SSInfo>::type stored;

    StringStorage();
    /** Remembers a string in global pool */
    void add(const String &str);
    /** Checks whether given string is stored */
    bool isStored(const char *str);

    /** Takes a stored representation of given string or 0 if the string is not stored. */
    unsigned long get(const char *str);
    ~StringStorage();
};

//------------------------------------------------
SimpleString::SimpleString(const String &s) {
    buf=duplicate(s.c_str());
}
SimpleString::SimpleString(const SimpleString &from) {
    copyFrom(from);
}

SimpleString::~SimpleString() {
    freeDuplicate(buf);
}

void SimpleString::copyFrom(const SimpleString &from) {
    buf=duplicate(from.c_str());
}

SimpleString &SimpleString::operator=(const SimpleString &b) {
    copyFrom(b);
    return *this;
}

const char *SimpleString::c_str() const {
    return buf;
}
//------------------------------------------------

/*
CStr::CStr(const char *buf):buf(buf) {}
const char *CStr::get() const {
        return buf;
    }
bool CStr::operator<(const CStr &b) const {
    return strcmp(buf,b.buf)<0;
}
*/

//------------------------------------------------
StringStorage::StringStorage() {
    //add default storage
    add(String());
}

void StringStorage::add(const String &str) {
    const char *cs=str.c_str();
    if (cs==NULL) cs="";
    if (!isStored(cs)) {
        const char *s(duplicate(cs));
        contents.insert(std::pair<const char *,int>(s,stored.size()));
        SSInfo ssi;
        ssi.str=s;
        ssi.hashCode=CStrHashFn()(s);
        stored.push_back(ssi);
    }
}
bool StringStorage::isStored(const char *str) {
    return contents.find(str)!=contents.end();
}

unsigned long StringStorage::get(const char *str) {
    StoreMap::iterator it=contents.find(str);
    if (it!=contents.end()) {
        return (it->second*2+1);
    } else return 0;
}

StringStorage::~StringStorage() {
    while (!contents.empty()) {
        StoreMap::iterator it=contents.begin();
        contents.erase(it);
        freeDuplicate(const_cast<char *>(it->first));
    }

    stored.clear();
}

//------------------------------------------------
log4cxx::LoggerPtr StoreString::LOG = StoreString::LOG ;
StringStorage *StoreString::storage = StoreString::storage;

StoreString::MyExistence_I::MyExistence_I() {
    StoreString::LOG = log4cxx::Logger::getLogger("xmlbeansxx.StoreString");
    storage = new StringStorage();
    LOG4CXX_DEBUG(LOG, "initialized");
}

StoreString::MyExistence_I::~MyExistence_I() {
    delete storage;
    storage = NULL;
    LOG4CXX_DEBUG(LOG, "finalized");
}

Existence StoreString::staticInit() {
    static Existence ptr(new MyExistence_I());
    return ptr;
}

void StoreString::construct(const char *str) {
    buf=storage->get(str);
    if (buf==0) {
        buf=(unsigned long)duplicate(str);
    }
}

StoreString::StoreString() {
    //TRACER(log,"StoreString")
    construct("");
}

StoreString::StoreString(const String &str) {
    //TRACER(log,"StoreString")
    construct(str.c_str());
}

StoreString::StoreString(const char *str) {
    construct(str);
}

StoreString::StoreString(const StoreString &from) {
    //TRACER(log,"StoreString")
    copyFrom(from);
}

StoreString::~StoreString() {
    //TRACER(log,"~StoreString")
    //LOG4CXX_DEBUG(log,"str:"<<c_str());
    if (!isStored()) {
        //LOG4CXX_DEBUG(log,"Deleting "<<TextUtils::ptrToString(buf));
        delete[] ((char *)buf);
    }
    buf=0;
}

String StoreString::toString() const {
    return String(const_cast<char *>(c_str()));
}

StoreString StoreString::store(const String &str) {
    storage->add(str);
    return StoreString(str);
}

const char *StoreString::c_str() const {
    if (isStored()) {
        return storage->stored[buf/2].str;
    } else {
        return (char *)buf;
    }
}

unsigned int StoreString::hashCode() const {
    if (isStored()) {
        return storage->stored[buf/2].hashCode;
    } else return CStrHashFn()((char *)buf);
}

bool StoreString::isStored() const {
    //LOG4CXX_DEBUG(LOG, "here this " << this);
    //LOG4CXX_DEBUG(LOG, "ptr " << buf);
    return ((unsigned long)buf)&1;
}


}


