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

int XmlContext::getLink(std::string shortcut) {
    return nsLinks[shortcut]-1;
}

void XmlContext::setLink(std::string shortcut,int ns) {
    restoreLinks.push(std::pair<std::string,int>(shortcut,nsLinks[shortcut]));
    nsLinks[shortcut]=ns+1;
}

int XmlContext::restorePosition() {
    return restoreLinks.size();
}

void XmlContext::restore(int restorePosition) {
    while (int(restoreLinks.size())>restorePosition) {
        nsLinks[restoreLinks.top().first]=restoreLinks.top().second;
        restoreLinks.pop();
    }
}

}
