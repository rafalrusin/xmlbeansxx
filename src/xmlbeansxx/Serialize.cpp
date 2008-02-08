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
#include "XmlContext.h"

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


struct NSMapSerializer : public XmlContext {
	
	std::vector<std::pair<std::string, StoreString> > notPrinted;
	int current;

	NSMapSerializer():current(0){};


/*	virtual bool addNamespace(const std::string& prefix,StoreString ns) {
	    if(true){ //!isSetNamespaceURI(ns)
	    	NSMap::addNamespace(prefix, ns);
		std::pair<std::string, StoreString> a(prefix, ns);
		notPrinted.push_back(a);
	    	return true;
	    }
	    return false;
	}

*/	
	std::string cprintQName(const QName& n) {
		if(n.first==StoreString("")) return n.second;
		
		std::string prefix=n.prefix;
        	XMLBEANSXX_DEBUG(log,std::string("printQName: ") + prefix + "{" + n.first + "}" + n.second);
		try {
			if(getNamespaceURI(prefix) == n.first) 
				return printPrefixName(prefix, n.second);
		} catch(BeansException &e) {
			//Prefix not set (getPrefix)
		}
		if(prefix.empty()) {
			try {
				prefix = getPrefix(n.first);
				return printPrefixName(prefix, n.second);
				
			} catch (BeansException &e) {
				prefix=getNextPrefix();
			}
		}
		addNamespace(prefix,n.first);
		return printPrefixName(prefix, n.second);
	};
	
	
	std::string printNewNS() {
		std::string retu;
	    	XmlContext::StoredLinks ns=getLastStoredLinks();
		XMLBEANSXX_FOREACH(XmlContext::StoredLinks::iterator,i,ns) {
			if(i->first.empty())	retu+=std::string(" xmlns=\"") + i->second + "\"";
			else			retu+=std::string(" xmlns:") + i->first  + "=\"" + i->second + "\"";
	    	}
		return retu;
	}
	
private:
	std::string getNextPrefix() {
		std::string prefix(1,'a'+current++);
		if(isSetPrefix(prefix)) return getNextPrefix();
		else 
			return prefix;
	}
	std::string printPrefixName(const std::string& prefix,const std::string& name) {
		if(prefix.empty()) return name;
		return prefix + ":" + name;
	}
	XMLBEANSXX_STATIC_LOGGER_PTR(log);	
};

XMLBEANSXX_LOGGER_PTR_SET(NSMapSerializer::log,"xmlbeansxx.NSMapSerializer");


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

	ns.remember();
	bool ClearDefaultNamespace = false;
	if(elemName.first==StoreString("")) {
		// if the element has a empty namespace then  it must have a "default namespace" (empty prefix)
		ClearDefaultNamespace = true;
		if(!(ns.getNamespaceURI("") == StoreString(""))) 
			ns.addNamespace("","");
	}
	// add all namespace attribute to namespace map
	XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,attrs.contents) {
		QName name = it->name;
		// add ownly namespaceses
		// check if the namespace is the "default namespace" and it must be empty
		if(name->first == XmlBeans::xmlns()  && (!ClearDefaultNamespace || !(name->second == StoreString("")))) {
		    	ns.addNamespace(name->second,xmlbeansxx::Contents::Walker::getSimpleContent(it->value));
		}
	}

	std::string name = ns.cprintQName(elemName);
    	o << "<" << name;


        //it's an object
    	if (printXsiType && options.getSerializeTypes()) {
		if( options.getSerializeInnerTypes() || !(st->name.first == XmlBeans::innerType_ns()) ) {
    			if(st->isArray) o << " " << ns.cprintQName(XmlBeans::xsi_array()) << "=\"" << ns.cprintQName(st->name) << "\"";
			else 		o << " " << ns.cprintQName(XmlBeans::xsi_type())  << "=\"" << ns.cprintQName(st->name) << "\"";
		}
        }
    
    	serializeAttrs(o,ns,options);
	
    	{
        	std::string cnt=TextUtils::exchangeEntities(getSimpleContent());
        	if (cnt==std::string() && !hasElements()) {
            		o << "/>";
        	} else {
        		o << ">";
            		o << cnt;
            		serializeElems(o,ns,options);
            		o << "</" << name << ">";
        	}
    	}
	
	ns.restore();

}

void Contents::serializeDocument(ostream &o,XmlOptions options) const {
    TRACER(log,"serializeDocument");
    SYNC(mutex)

    if (options.getPrintXmlDeclaration())
        o<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

    ElemDict::ContentsType::const_iterator it=elems.contents.begin();
    if (it==elems.contents.end() || it->value==NULL)
        return;

    NSMapSerializer ns;
    
    const std::map<QName,SchemaPropertyPtr> *order=&(st->elements);
    std::map<QName,SchemaPropertyPtr>::const_iterator propIt=order->find(it->name);
    SchemaPropertyPtr prop;
    if (propIt!=order->end())
        prop=propIt->second;

    bool printXsiType;
    if (prop==NULL) {
        std::string msg=std::string("Serializing document of class: ")+std::string(st->className)+std::string(". It's root element name should not be '")+it->name+std::string("'");
        XMLBEANSXX_DEBUG(log,msg);
	printXsiType=false;
//        throw XmlException(msg);
    } else printXsiType=shallPrintXsiType(prop->schemaType,it->value->st);
    //----------
    it->value->serialize(printXsiType,it->name,o,ns,options);
    o<<"\n";
    
}


void Contents::serializeAttrs(ostream &o,NSMapSerializer& ns, XmlOptions options) const {
	SYNC(mutex)
	XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,attrs.contents) {
		if(!(it->name.first == XmlBeans::xmlns())) {		
			o << " " << ns.cprintQName(it->name) << "=\"" << TextUtils::exchangeEntities(xmlbeansxx::Contents::Walker::getSimpleContent(it->value), TextUtils::AttrEscapes) << "\"";
		}
	}
	{
		o << ns.printNewNS();
	}
}


void Contents::serializeElems(ostream &o,NSMapSerializer ns, XmlOptions options) const {
	SYNC(mutex)
	TRACER(log,"serializeElems");


	// order sort
	typedef map<int,std::pair<QName,const SchemaType*> > MType;
	MType m;
	typedef map<QName,int> LeftType;
	LeftType left;
	int order=-1;
	XMLBEANSXX_FOREACH_BACKWARD(ElemDict::ContentsType::const_reverse_iterator,it,elems.contents) 
		left.insert(make_pair(it->name,order--));
	
	XMLBEANSXX_FOREACH(SchemaType::ElementsType::const_iterator,it,st->elements){
		m[it->second->order]=std::pair<QName,const SchemaType*>(it->first,it->second->schemaType);
		left.erase(it->first);
	}
	XMLBEANSXX_FOREACH(LeftType::const_iterator,it,left) {
		m[it->second]=std::pair<QName,const SchemaType*>(it->first,XmlObject::type());
	}
	
		
	XMLBEANSXX_FOREACH(MType::iterator,it,m){
		std::pair<QName,const SchemaType*> v=it->second;
		const QName &elemName = v.first;
		const SchemaType * elemSt = v.second;
		int count = elems.count(elemName);
		for(int i=0;i<count;i++) {
			ElemDict::value_type value(elems.find(elemName,i));
			if (value.value!=NULL) {
				bool printXsiType= shallPrintXsiType(elemSt,value.value->st);
				value.value->serialize(printXsiType,value.name,o,ns,options);
			}
		}
	}
}

}
