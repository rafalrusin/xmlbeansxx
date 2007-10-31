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
	int start=0;
	if(rememberedPositions.size()>0)  start=rememberedPositions.top();
	for(int i=start;i<restoreLinks.size();i++) {
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


}
