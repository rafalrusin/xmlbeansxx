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


namespace xmlbeansxx {

XMLBEANSXX_LOGGER_PTR_SET(NSMap::log,"xmlbeansxx.NSMap");

bool NSMap::isSetNamespaceURI(StoreString ns) const {
	XMLBEANSXX_FOREACH(PrefixMapType::const_iterator,i,prefixMap){
		if (i->second == ns) return true;
	}
	return false;
}
bool NSMap::isSetPrefix(const std::string& prefix) const {
	return prefixMap.find(prefix)!=prefixMap.end();
}

bool NSMap::addNamespace(const std::string& prefix, StoreString ns, bool force) {
        XMLBEANSXX_DEBUG(log,std::string("addNamespace: ") + prefix + ": " + ns);
	bool retu=true;
	if(isSetPrefix(prefix)) retu=false;
	if(force||retu) prefixMap[prefix]=ns;
	return retu;
}

StoreString NSMap::getNamespaceURI(const std::string& prefix) const {
	PrefixMapType::const_iterator p=prefixMap.find(prefix);
	if(p==prefixMap.end())
		throw BeansException("Namespace prefix: " + prefix + " not set.");

	return p->second;
}

std::string NSMap::getPrefix(StoreString ns) const {
	XMLBEANSXX_FOREACH(PrefixMapType::const_iterator, i, prefixMap){
		if (i->second == ns) return i->first;
	}
	throw BeansException(std::string("Namespace: ") + ns + " not set.");
}

std::string NSMap::toString() const {
	std::string retu;
	XMLBEANSXX_FOREACH(PrefixMapType::const_iterator, i, prefixMap){
		retu += i->first + " => " + i->second;
	}
	return retu;
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
	std::string prefix=name.substr(0,p);
	return QName(getNamespaceURI(prefix),name.substr(p+1),prefix);
}
}
