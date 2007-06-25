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
#ifndef XMLBEANSXX_NSMAP_INCLUDED
#define XMLBEANSXX_NSMAP_INCLUDED

#include "StoreString.h"
#include <string>
#include <map>
#include "QName.h"

namespace xmlbeansxx {

class NSMap {
public:
	
	NSMap(){};
	virtual ~NSMap(){};	
	virtual bool addNamespace(const std::string& prefix, StoreString ns);
	virtual bool isSetNamespace(StoreString ns) const;
	virtual bool isSetPrefix(const std::string& prefix) const;
	virtual StoreString getNamespace(const std::string& prefix) const;
	virtual std::string getPrefix(StoreString ns) const;
	virtual QName getQName(const std::string& name) const;
	
	
private:
	// prefix, namespace
	std::map<std::string,StoreString> prefixMap;

	
};

};



#endif //XMLBEANSXX_NSMAP_INCLUDED
