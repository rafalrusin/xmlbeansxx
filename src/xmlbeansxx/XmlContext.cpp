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

StoreString XmlContext::getLink(const std::string& shortcut) {
    return nsLinks[shortcut];
}

void XmlContext::setLink(const std::string& shortcut, StoreString ns) {
    restoreLinks.push(std::pair<std::string, StoreString>(shortcut, nsLinks[shortcut]));
    nsLinks[shortcut] = ns;
}

void XmlContext::remember() {
    rememberedPositions.push(restoreLinks.size());
}

void XmlContext::restore() {
    int restorePosition = rememberedPositions.top();
    rememberedPositions.pop();
    while (int(restoreLinks.size()) > restorePosition) {
        nsLinks[restoreLinks.top().first] = restoreLinks.top().second;
        restoreLinks.pop();
    }
}

}
