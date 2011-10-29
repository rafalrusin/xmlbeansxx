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
#include <boost/thread/recursive_mutex.hpp>
#define SYNC(mutex) boost::recursive_mutex::scoped_lock lock(mutex);
#else
#define SYNC(mutex)
#endif



namespace xmlbeansxx {


namespace {
bool shallPrintXsiType(const SchemaType *xsdDefined,const SchemaType * st) {
    static XmlObject src=XmlObject::Factory::newInstance();
    return st->classTypeInfo != src.getSchemaType()->classTypeInfo
        && st->classTypeInfo != xsdDefined->classTypeInfo;
}
}


void Contents::serialize(bool printXsiType,const QName& elemName,std::ostream &o,NSMapSerializer &ns,XmlOptions options) const {
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
//    			if(st->isArray) o << " " << ns.cprintQName(XmlBeans::xsi_array()) << "=\"" << ns.cprintQName(st->name) << "\"";
//			else 		o << " " << ns.cprintQName(XmlBeans::xsi_type())  << "=\"" << ns.cprintQName(st->name) << "\"";
	 		o << " " << ns.cprintQName(XmlBeans::xsi_type())  << "=\"" << ns.cprintQName(st->name) << "\"";
		}
        }

    	serializeAttrs(o,ns,options);


      	if ( !hasElements()) {
      		o << "/>";
      	} else {
      		o << ">";
       		serializeElems(o,ns,options);
       		o << "</" << name << ">";
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
    if(options.getSerializePersistent()) ns.prefixPrefix=XmlBeans::persistentPrefix();

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
    } else printXsiType=shallPrintXsiType(globalTypeSystem()->getSchemaType(prop->schemaTypeName),it->value->st);
    //----------
    it->value->serialize(printXsiType,it->name,o,ns,options);
    o<<"\n";

}

void Contents::serializeAttrs(std::ostream &o,NSMapSerializer& ns,XmlOptions options) const {
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
template<class T1,class T2,class T3>
three<T1,T2,T3> make_three(const T1 &a,const T2 &b,const T3 &c) {
 	return three<T1,T2,T3>(a,b,c);
}

}


#define MAX(a,b) ((a>b)?(a):(b))

void Contents::serializeElems(ostream &o,NSMapSerializer &ns, XmlOptions options) const {
	SYNC(mutex)
	TRACER(log,"serializeElems");

	ns.remember();


		// order , element, expected element type
	typedef vector< three < int ,  ElemDict::ContentsType::const_iterator , const SchemaType * > > SortType;

	SortType sort;


	//sort all elements
	int max_order = 0;
	// calculate the element orders
	XMLBEANSXX_FOREACH(ElemDict::ContentsType::const_iterator,it,elems.contents) {
		const SchemaType::ElementsType::const_iterator st_it = st->elements.find(it->name);
		if(st_it != st->elements.end()) {
			sort.push_back(make_three(st_it->second->order,it, globalTypeSystem()->getSchemaType(st_it->second->schemaTypeName)));
			max_order = MAX(max_order,st_it->second->order);
		} else {
			//this element  has no order
			sort.push_back(make_three(max_order,it, XmlObject::type()));
		}
	}
	std::sort(sort.begin(),sort.end());


	//print the elements
	XMLBEANSXX_FOREACH(SortType::const_iterator,it,sort) {
		const ElemDict::ContentsType::const_iterator & el_it = it->second;
		const SchemaType * st = it->third;
		if (el_it->value!=NULL) {
			// print the element
			bool printXsiType= shallPrintXsiType(st,el_it->value->st);
			el_it->value->serialize(printXsiType,el_it->name,o,ns,options);
		}
	}


	ns.restore();
}

}
