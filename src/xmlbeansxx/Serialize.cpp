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


struct NSMap {
	
	std::map<StoreString,std::string> prefixMap;
	std::vector<std::pair<StoreString,std::string> > notPrinted;
	int current;

	NSMap():current(0){};
	
	std::string cprintQName(const QName& n) {
		if(n.first==StoreString("")) return n.second;
		
		VAL(i,prefixMap.find(n.first));
		if(i==prefixMap.end()) {
			std::string prefix=getNextPrefix();
			addNamespace(n.first,prefix);
			return printPrefixName(prefix, n.second);
		}
		return printPrefixName(i->second, n.second);
	};
	
	void addNamespace(StoreString ns,const std::string& prefix) {
		if(prefixMap.find(ns)!=prefixMap.end()) return;
		std::pair<StoreString,std::string> a(ns,prefix);
		prefixMap.insert(a);
		notPrinted.push_back(a);
	}
	
	std::string printNewNS() {
		std::string retu;
		while(!notPrinted.empty()) {
			std::pair<StoreString,std::string> a=notPrinted.back();
			if(a.second.empty())	retu+=std::string(" xmlns=\"") + a.first + "\"";
			else			retu+=std::string(" xmlns:") + a.second  + "=\"" + a.first + "\"";
			
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



void Contents::serialize(bool printXsiType,const QName& elemName,std::ostream &o,NSMap ns ) const {
	TRACER(log,"serialize");
    	SYNC(mutex)

    	o << "<" << ns.cprintQName(elemName);
    	o << ns.printNewNS();


        //it's an object
    	if (printXsiType) {
		if(st->isArray) {
			o << " xsi:array=\"" << ns.cprintQName(st->name) << "\"";
			o << ns.printNewNS();			
		} else {
			o << " xsi:type=\"" << ns.cprintQName(st->name) << "\"";
			o << ns.printNewNS();
		}
        }
    
    	serializeAttrs(o,ns);
	
    	{
        	std::string cnt=TextUtils::exchangeEntities(getSimpleContent());
        	if (cnt==std::string() && !hasElements()) {
            		o<<"/>";
        	} else {
        		o<<">";
            		o<<cnt;
            		serializeElems(o,ns);
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

    NSMap ns;
    ns.addNamespace("http://www.w3.org/2001/XMLSchema-instance","xsi");
    ns.addNamespace("http://www.w3.org/2001/XMLSchema","xs");
    ns.addNamespace(it->name->first,"");
    
    const std::map<QName,SchemaPropertyPtr> *order=&(st->elements);
    std::map<QName,SchemaPropertyPtr>::const_iterator propIt=order->find(it->name);
    SchemaPropertyPtr prop;
    if (propIt!=order->end())
        prop=propIt->second;

    if (prop==NULL) {
        std::string msg=std::string("Serializing document of class: ")+std::string(st->className)+std::string(". It's root element name should not be '")+it->name+std::string("'");
        LOG4CXX_DEBUG(log,msg);
        throw XmlException(msg);
    }

    bool printXsiType=shallPrintXsiType(prop->schemaType,it->value->st);
    //----------
    it->value->serialize(printXsiType,it->name,o,ns);
    o<<"\n";
    
}


void Contents::serializeAttrs(ostream &o,NSMap& ns) const {
	SYNC(mutex)
	FOREACH(it,attrs.contents) {
		o << " " << ns.cprintQName(it->name) << "=\"" << TextUtils::exchangeEntities(xmlbeansxx::Contents::Walker::getSimpleContent(it->value), TextUtils::AttrEscapes) << "\"";
		o << ns.printNewNS();
	}
}


void Contents::serializeElems(ostream &o,NSMap ns) const {
	SYNC(mutex)
	TRACER(log,"serializeElems");


	// order sort
	map<int,std::pair<QName,const SchemaType*> > m;
	set<QName> left;
	FOREACH(it,elems.contents) 
		left.insert(it->name);
	
	FOREACH(it,st->elements){
		m[it->second->order]=std::pair<QName,const SchemaType*>(it->first,it->second->schemaType);
		left.erase(it->first);
	}
	FOREACH(it,left)
		m[-1]=std::pair<QName,const SchemaType*>(*it,XmlObject::type());
	
		
	FOREACH(it,m){
		VAL(v,it->second);
		const QName &elemName = v.first;
		const SchemaType * elemSt = v.second;
		int count = elems.count(elemName);
		for(int i=0;i<count;i++) {
			ContentsPtr value(elems.find(elemName,i));
			if (value!=NULL) {
				bool printXsiType= shallPrintXsiType(elemSt,value->st);
				value->serialize(printXsiType,elemName,o,ns);
			}
		}
	}
}

}
