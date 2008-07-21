/*
    Copyright 2004-2008 TouK sp. z o.o. s.k.a.
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */



#include "Contents.h"
#include "Tracer.h"
#include "defs.h"
#include "XmlTypesGen.h"
#include <set>
#include <string>
#include <iostream>
#include <map>
#include "XmlBeans.h"
#include "XmlTypesGen.h"

#include "TypeSystem.h"
#include "TextUtils.h"
#include "SchemaProperty.h"
#include "ContentsImpl.h"

using namespace std;


//------------------------------------------------------------------------------------

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#define SYNC(mutex) boost::recursive_mutex::scoped_lock lock(mutex);
#else
#define SYNC(mutex)
#endif



namespace xmlbeansxx {

XMLBEANSXX_LOGGER_PTR_SET(Contents::log,"xmlbeansxx.Contents");

//Contents Impl





namespace {
template<class T1,class T2,class T3>
struct three {
    T1 first; T2 second; T3 third;
    three(const T1 &a,const T2 &b,const T3 &c): first(a), second(b),third(c) {}
    bool operator <(const three<T1,T2,T3> &other) const {
        if (first==other.first) return second<other.second;
        else return first<other.first;
    }
};
    
}

//Contents::Contents() { }

Contents::~Contents() {
    free();
}

void Contents::free() {
    SYNC(mutex)
    elems.free();
    attrs.free();
}



ContentsPtr Contents::clone() 
{
    SYNC(mutex)
    ContentsPtr clone(new Contents(st));
    XMLBEANSXX_FOREACH(ElemDict::ContentsType::iterator,it,elems.contents) {
        ContentsPtr o=it->value;
        if(o) 	clone->elems.add(it->name,o->clone());
	else	clone->elems.add(it->name,ContentsPtr());
    }

    XMLBEANSXX_FOREACH(ElemDict::ContentsType::iterator,it,attrs.contents) {
        ContentsPtr o=it->value;
        if(o)	clone->attrs.add(it->name,o->clone());
	else 	clone->attrs.add(it->name,ContentsPtr());
    }

    return clone;
}


const SchemaType * Contents::getSchemaType() const
{
	return st;
}






void Contents::setSimpleContent(const std::string &c) {
    removeElems(XmlBeans::textElementName());
    if(c.size() > 0)
	setElem(XmlBeans::textElementName(), ContentsPtr (new StringContents(c)));
    
}
std::string Contents::getSimpleContent() const {
    SYNC(mutex)
    std::string r;
    std::vector<ContentsPtr>  txt = Contents::getElemArray(XmlBeans::textElementName());
    XMLBEANSXX_FOREACH(std::vector<ContentsPtr>::const_iterator,it,txt) {
    	r+=(*it)->getSimpleContent();
    }
    return r;

}



ContentsPtr Contents::getAttr(const QName &elemName) const {
    SYNC(mutex)
    return attrs.find(elemName).value;
}

void Contents::setAttr(const QName &elemName,ContentsPtr value) {
    SYNC(mutex)
    attrs.del(elemName);
    attrs.add(elemName,value);
}



ContentsPtr Contents::getElem(const QName& elemName,int index) const {
    SYNC(mutex)
    return elems.find(elemName,index).value;
}

void Contents::setElem(const QName& elemName,ContentsPtr value,int index) {
    SYNC(mutex)
    elems.set(elemName,index,value);
}

bool Contents::isSetElem(const QName& elemName,int index){
    SYNC(mutex)
    return elems.find(elemName,index).value!=NULL;
};

void Contents::removeElems(const QName& name) {
    SYNC(mutex)
    elems.del(name);
}

int Contents::countElems(const QName& name) const {
    SYNC(mutex)
    return elems.count(name);
}


bool Contents::hasElements() const {
    SYNC(mutex)
    return elems.size()>0;
}


std::vector<ContentsPtr> Contents::getElemArray(const QName& elemName) const {
    SYNC(mutex)
    std::vector<ContentsPtr> r;
    XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,elems.contents) {
        if (elemName==it->name) {
            r.push_back(it->value);
        }
    }
    return r;
}


void Contents::removeElemAt(const QName& elemName,int index) {
    SYNC(mutex)
    elems.removeAt(elemName,index);
    
}

bool Contents::hasEmptyContent() const {
    SYNC(mutex)
    return attrs.hasEmptyContent() && elems.hasEmptyContent();
}

vector<pair<QName,ContentsPtr > > Contents::getElems(bool all) const {
    SYNC(mutex)
    vector<pair<QName,ContentsPtr > > v;
    XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,elems.contents) {
    	if(all || (it->name != XmlBeans::textElementName()))
	        v.push_back(pair<QName,ContentsPtr >(it->name,it->value));
    }
    return v;
}

vector<pair<QName,string> > Contents::getAttrs() const {
    SYNC(mutex)
    vector<pair<QName,string> > v;
    XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,attrs.contents) {
        v.push_back(pair<QName,string>(it->name,Walker::getSimpleContent(it->value)));
    }
    return v;
}

vector<pair<QName,ContentsPtr> > Contents::getAttrs2() const {
    SYNC(mutex)
    vector<pair<QName,ContentsPtr> > v;
    XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,attrs.contents) {
        v.push_back(pair<QName,ContentsPtr>(it->name,it->value));
    }
    return v;
}


std::string Contents::digest() const {
    std::string r;
    
    Contents::Walker::ElemsType as=getAttrs2();
    Contents::Walker::ElemsType es=getElems();

//    sort(as.begin(),as.end());
    XMLBEANSXX_FOREACH(Contents::Walker::ElemsType::iterator,it,as) {
    	if(it->second){
        	r+=" ";
	        r+=std::string(it->first);
        	r+="=\"";
	        r+=TextUtils::exchangeEntities(it->second->getCanonicalContent(),TextUtils::AttrEscapes);
        	r+="\"";
	}
    }
    r+=">";
    r+=TextUtils::exchangeEntities(getCanonicalContent());
    
    typedef std::map<std::pair<QName,int>, ContentsPtr> MType;
    MType m;
    std::map<std::string, int> counters;
    {
	SYNC(mutex)
    	XMLBEANSXX_FOREACH(Contents::Walker::ElemsType::iterator,it,es) {
        	if (it->second!=NULL) {
	            m[std::pair<QName,int>(it->first,counters[it->first])]=it->second;
        	    counters[it->first]++;
	        }
	}
    }
    XMLBEANSXX_FOREACH(MType::iterator,it,m) {
        r+="<";
        r+=std::string(it->first.first);
        r+=it->second->digest();
        r+="</";
        r+=std::string(it->first.first);
        r+=">";
    }
    return r;
}


std::string Contents::getCanonicalContent() const {
	if(!st->createFn) return getSimpleContent();
	XmlObjectPtr o = st->createFn();
	if(!o) return getSimpleContent();
	ContentsPtr const c=boost::const_pointer_cast<Contents>(shared_from_this());
	o->swapContents(c);
	return o->getCanonicalContent();

	return getSimpleContent();
}


StringContents::StringContents(const std::string & str):Contents(xmlbeansxx::XmlString::type()),value(str){};



} //namespace xmlbeansxx


