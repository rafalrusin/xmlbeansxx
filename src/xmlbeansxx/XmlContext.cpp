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


#include "XmlContext.h"
#include "defs.h"


	
namespace xmlbeansxx {


bool XmlContext::addNamespace(const std::string& shortcut, StoreString ns,bool force) {
	if(isSetPrefix(shortcut)) {	
		if(!force) return false;
    		restoreLinks.push_back(make_pair(shortcut, getNamespaceURI(shortcut)));
	} else	restoreLinks.push_back(make_pair(shortcut, ""));
	return NSMap::addNamespace(shortcut,ns,true);
}

void XmlContext::remember() {
    	rememberedPositions.push(restoreLinks.size());
}

XmlContext::StoredLinks XmlContext::getLastStoredLinks() {
	StoredLinks retu;
	StoredLinks::size_type start=0;
	if(rememberedPositions.size()>0)  start=rememberedPositions.top();
	for(StoredLinks::size_type i=start;i<restoreLinks.size();i++) {
		std::string prefix = restoreLinks[i].first;
		retu.push_back(make_pair(prefix,getNamespaceURI(prefix)));
	}
	return retu;
}


void XmlContext::restore() {
    rememberedPositions.pop();
    int restorePosition = rememberedPositions.top();
    while (int(restoreLinks.size()) > restorePosition) {
        NSMap::addNamespace(restoreLinks.back().first,restoreLinks.back().second,true);
        restoreLinks.pop_back();
    }
}
XmlContext::XmlContext() { 
	remember(); 
	addNamespace("","");
	remember();
}

XmlContext::~XmlContext() {};



//  NSMapSerializer

XMLBEANSXX_LOGGER_PTR_SET(NSMapSerializer::log,"xmlbeansxx.NSMapSerializer");


std::string NSMapSerializer::printNewNS()  {
		std::string retu;
	    	XmlContext::StoredLinks ns=getLastStoredLinks();
		XMLBEANSXX_FOREACH(XmlContext::StoredLinks::iterator,i,ns) {
			if(i->first.empty())	retu+=std::string(" xmlns=\"") + i->second + "\"";
			else			retu+=std::string(" xmlns:") + i->first  + "=\"" + i->second + "\"";
	    	}
		return retu;
	}



std::string NSMapSerializer::cprintQName(const QName& n) {
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
	
	
std::string NSMapSerializer::getNextPrefix() {
		std::string myChar(1,'a'+current++);
		std::string prefix=prefixPrefix + myChar;
		if(isSetPrefix(prefix)) return getNextPrefix();
		else return prefix;
}
std::string NSMapSerializer::printPrefixName(const std::string& prefix,const std::string& name) {
		if(prefix.empty()) return name;
		return prefix + ":" + name;
}







}
