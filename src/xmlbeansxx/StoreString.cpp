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

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/detail/lock.hpp>
#define SYNC(mutex) boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(mutex);
#else
#define SYNC(mutex)
#endif



namespace xmlbeansxx {
//------------------------------------------------

namespace {

char *duplicate(const char *str) {

    char *buf;
    int l=strlen(str)+1;
    buf=new char[l];


    if (!((((unsigned long)buf)&1)==0)) {
    	delete[] buf;
        throw IllegalStateException();
    }
    //strcpy(buf,str);
    for(int i=0;i<l;i++) buf[i]=str[i];
    return buf;
}

void freeDuplicate(char *str) {
    delete[] str;
}
}
//------------------------------------------------
SimpleString::SimpleString(const std::string &s) {
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
    add(std::string());
}

void StringStorage::add(const std::string &str) {
    const char *cs=str.c_str();
    return add(cs);
}

void StringStorage::add(const char * cs) {
    if (cs==NULL) cs="";
    if (!isStored(cs)) {
	SYNC(mutex);
        const char *s(duplicate(cs));
        contents.insert(std::pair<const char *,int>(s,stored.size()));
        SSInfo ssi;
        ssi.str=s;
        ssi.hashCode=CStrHashFn()(s);
        stored.push_back(ssi);
    }
}
bool StringStorage::isStored(const char *str) const {
    SYNC(mutex);
    return  contents.find(str)!=contents.end();
}

unsigned long StringStorage::get(const char *str) const {
    SYNC(mutex);    
    StoreMap::const_iterator it=contents.find(str);
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

void StoreString::construct(const std::string &str) {
    free();
    buf=getStorage()->get(str.c_str());
    if (buf==0) {
        buf=(unsigned long)duplicate(str.c_str());
    }
}

StringStorage *StoreString::getStorage() {
    static std::auto_ptr<StringStorage> storage(new StringStorage());
    return storage.get();
}

StoreString::StoreString() {
    buf=0;
    construct(std::string());
}

StoreString::StoreString(const std::string &str) {
    buf=0;
    construct(str);
}
StoreString::StoreString(const char *str) {
    buf=0;
    construct(std::string(str));
}

StoreString::StoreString(const StoreString &from) {
    buf=0;
    copyFrom(from);
}

void StoreString::free()
{
    if (!isStored() && buf!=0) {
        delete[] ((char *)buf);
    }
    buf=0;
}

StoreString::~StoreString() {
    free();
}

std::string StoreString::toString() const {
    return std::string(const_cast<char *>(c_str()));
}

StoreString StoreString::store(const std::string &str) {
    getStorage()->add(str);
    return StoreString(str);
}
StoreString StoreString::store(const char *str) {
    getStorage()->add(str);
    return StoreString(str);
}

}


