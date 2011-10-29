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


#ifndef _XMLBEANSXX_XMLCONTEXT_H_
#define _XMLBEANSXX_XMLCONTEXT_H_

#include "BoostAssert.h"
#include <string>
#include <stack>
#include <map>
#include "StoreString.h"
#include "NSMap.h"
#include "BeansException.h"
#include <vector>

namespace xmlbeansxx {

/**
 * Provides context of xml namespace links.
 */

class XmlContext : public NSMap {
public:
    typedef std::vector<std::pair<std::string, StoreString> > StoredLinks;
private:
    StoredLinks restoreLinks;
    std::stack<int> rememberedPositions;

public:
    XmlContext();
    virtual ~XmlContext();
    virtual bool addNamespace(const std::string& prefix, StoreString ns,bool force = true);

    /** Restores state of links to last remembered posittion. */
    void restore();

    //get a list of last stored links
    StoredLinks getLastStoredLinks();

    /** Remembers state of links. */
    void remember();
};


struct NSMapSerializer : public XmlContext {

	std::vector<std::pair<std::string, StoreString> > notPrinted;
	//prefix for prefixing the namespaces prefix :-)
	std::string prefixPrefix;
	int current;

	NSMapSerializer():current(0){};

	std::string cprintQName(const QName& n);
	std::string printNewNS() ;

private:
	std::string getNextPrefix();
	std::string printPrefixName(const std::string& prefix,const std::string& name);

	XMLBEANSXX_STATIC_LOGGER_PTR(log);
};


}

#endif//_XMLBEANSXX_XMLCONTEXT_H_
