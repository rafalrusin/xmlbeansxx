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



#include "Contents.h"
#include "Tracer.h"
#include "defs.h"
#include "XmlTypesGen.h"
#include <set>
#include <string>
#include <iostream>
#include <map>
#include "StoreString.h"

#include "TypeSystem.h"
#include "TextUtils.h"
#include "SchemaProperty.h"
#include "XmlBeans.h"
#include "NSMap.h"

using namespace std;


//------------------------------------------------------------------------------------

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/detail/lock.hpp>
#define SYNC(mutex) boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(mutex);
#else
#define SYNC(mutex)
#endif





namespace xmlbeansxx {

LOGGER_PTR_SET(NSMap::log,"xmlbeansxx.NSMap");

bool NSMap::isSetNamespaceURI(StoreString ns) const {
	FOREACH(i,prefixMap){
		if (i->second == ns) return true;
	}
	return false;
}
bool NSMap::isSetPrefix(const std::string& prefix) const {
	return prefixMap.find(prefix)!=prefixMap.end();
}

bool NSMap::addNamespace(const std::string& prefix, StoreString ns) {
        LOG4CXX_DEBUG(log,"addNamespace: " + prefix + ": " + ns);
	if(isSetPrefix(prefix)) return false;
	std::pair<std::string, StoreString> a(prefix, ns);
	prefixMap.insert(a);
	return true;
}

StoreString NSMap::getNamespaceURI(const std::string& prefix) const {
	VAL(p,prefixMap.find(prefix));
	if(p==prefixMap.end())
		throw BeansException("Namespace prefix: " + prefix + " not set.");
	
	return p->second;
}

std::string NSMap::getPrefix(StoreString ns) const {
	FOREACH(i,prefixMap){
		if (i->second == ns) return i->first;
	}
	throw BeansException(std::string("Namespace: ") + ns + " not set.");
}


QName NSMap::getQName(const std::string& name) const {
	int p=name.find(':');
	if(p<0) {
		try {
			StoreString ns = getNamespaceURI("");
			return QName(ns,name);
		} catch(BeansException &e) {
			return QName("",name);
		}
	}
	return QName(getNamespaceURI(name.substr(0,p)),name.substr(p+1));
}




struct NSMapSerializer : public NSMap {
	
	std::vector<std::pair<std::string, StoreString> > notPrinted;
	int current;

	NSMapSerializer():current(0){};


	virtual bool addNamespace(const std::string& prefix,StoreString ns) {
	    if(!isSetNamespaceURI(ns)) {
	    	NSMap::addNamespace(prefix, ns);
		std::pair<std::string, StoreString> a(prefix, ns);
		notPrinted.push_back(a);
	    	return true;
	    }
	    return false;
	}

	
	std::string cprintQName(const QName& n) {
		if(n.first==StoreString("")) return n.second;
		
		//VAL(i,prefixMap.find(n.first));
		if(!isSetNamespaceURI(n.first)) {
			std::string prefix=getNextPrefix();
			addNamespace(prefix,n.first);
			return printPrefixName(prefix, n.second);
		}
		return printPrefixName(getPrefix(n.first), n.second);
	};
	
	
	std::string printNewNS() {
		std::string retu;
		while(!notPrinted.empty()) {
			std::pair<std::string, StoreString> a=notPrinted.back();
			if(a.first.empty())	retu+=std::string(" xmlns=\"") + a.second + "\"";
			else			retu+=std::string(" xmlns:") + a.first  + "=\"" + a.second + "\"";
			
			notPrinted.pop_back();
		}
		
		return retu;
	}
	
private:
	std::string getNextPrefix() {
		return std::string(1,'a'+current++);
	}
	std::string printPrefixName(const std::string& prefix,const std::string& name) {
		if(prefix.empty()) return name;
		return prefix + ":" + name;
	}
	
};



namespace {
bool shallPrintXsiType(const SchemaType *xsdDefined,const SchemaType * st) {
    static XmlObject src=XmlObject::Factory::newInstance();
    return st->classTypeInfo != src.getSchemaType()->classTypeInfo 
        && st->classTypeInfo != xsdDefined->classTypeInfo;
}
}



void Contents::serialize(bool printXsiType,const QName& elemName,std::ostream &o,NSMapSerializer ns,XmlOptions options) const {
	TRACER(log,"serialize");
    	SYNC(mutex)

    	o << "<" << ns.cprintQName(elemName);
    	o << ns.printNewNS();


        //it's an object
    	if (printXsiType) {
		if( options.getSerializeInnerTypes() || !(st->name.first == XmlBeans::innerType_ns()) ) {
			if(st->isArray) {
				o << " xsi:array=\"" << ns.cprintQName(st->name) << "\"";
				o << ns.printNewNS();			
			} else {
				o << " xsi:type=\"" << ns.cprintQName(st->name) << "\"";
				o << ns.printNewNS();
			}
		}
        }
    
    	serializeAttrs(o,ns,options);
	
    	{
        	std::string cnt=TextUtils::exchangeEntities(getSimpleContent());
        	if (cnt==std::string() && !hasElements()) {
            		o<<"/>";
        	} else {
        		o<<">";
            		o<<cnt;
            		serializeElems(o,ns,options);
            		o<<"</"<<ns.cprintQName(elemName)<<">";
        	}
    	}
}

void Contents::serializeDocument(ostream &o,XmlOptions options) const {
    TRACER(log,"serializeDocument");
    SYNC(mutex)

    if (options.getPrintXmlDeclaration())
        o<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

    VAL(it,elems.contents.begin());
    if (it==elems.contents.end() || it->value==NULL)
        return;

    NSMapSerializer ns;
    ns.addNamespace("xsi","http://www.w3.org/2001/XMLSchema-instance");
    ns.addNamespace("xs","http://www.w3.org/2001/XMLSchema");
//    ns.addNamespace("",it->name->first);
    
    const std::map<QName,SchemaPropertyPtr> *order=&(st->elements);
    std::map<QName,SchemaPropertyPtr>::const_iterator propIt=order->find(it->name);
    SchemaPropertyPtr prop;
    if (propIt!=order->end())
        prop=propIt->second;

    bool printXsiType;
    if (prop==NULL) {
        std::string msg=std::string("Serializing document of class: ")+std::string(st->className)+std::string(". It's root element name should not be '")+it->name+std::string("'");
        LOG4CXX_DEBUG(log,msg);
	printXsiType=false;
//        throw XmlException(msg);
    } else printXsiType=shallPrintXsiType(prop->schemaType,it->value->st);
    //----------
    it->value->serialize(printXsiType,it->name,o,ns,options);
    o<<"\n";
    
}


void Contents::serializeAttrs(ostream &o,NSMapSerializer& ns, XmlOptions options) const {
	SYNC(mutex)
	FOREACH(it,attrs.contents) {
		o << " " << ns.cprintQName(it->name) << "=\"" << TextUtils::exchangeEntities(xmlbeansxx::Contents::Walker::getSimpleContent(it->value), TextUtils::AttrEscapes) << "\"";
		o << ns.printNewNS();
	}
}


void Contents::serializeElems(ostream &o,NSMapSerializer ns, XmlOptions options) const {
	SYNC(mutex)
	TRACER(log,"serializeElems");


	// order sort
	map<int,std::pair<QName,const SchemaType*> > m;
	map<QName,int> left;
	int order=-1;
	FOREACH_BACKWARD(it,elems.contents) 
		left.insert(make_pair(it->name,order--));
	
	FOREACH(it,st->elements){
		m[it->second->order]=std::pair<QName,const SchemaType*>(it->first,it->second->schemaType);
		left.erase(it->first);
	}
	FOREACH(it,left) {
		m[it->second]=std::pair<QName,const SchemaType*>(it->first,XmlObject::type());
	}
	
		
	FOREACH(it,m){
		VAL(v,it->second);
		const QName &elemName = v.first;
		const SchemaType * elemSt = v.second;
		int count = elems.count(elemName);
		for(int i=0;i<count;i++) {
			ContentsPtr value(elems.find(elemName,i));
			if (value!=NULL) {
				bool printXsiType= shallPrintXsiType(elemSt,value->st);
				value->serialize(printXsiType,elemName,o,ns,options);
			}
		}
	}
}

}
