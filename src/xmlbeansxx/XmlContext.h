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


#ifndef _XMLBEANSXX_XMLCONTEXT_H_
#define _XMLBEANSXX_XMLCONTEXT_H_

#include "BoostAssert.h"
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <boost/shared_ptr.hpp>


namespace xmlbeansxx {
        
class XmlContext {
private:
    std::map<std::string,int> nsLinks;
    std::stack<std::pair<std::string,int> > restoreLinks;
        
public:
    int getLink(std::string shortcut);
    void setLink(std::string shortcut,int ns);

    int restorePosition();
    void restore(int restorePosition);
};
}

#endif//_XMLBEANSXX_XMLCONTEXT_H_
