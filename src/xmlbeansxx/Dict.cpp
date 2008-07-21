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


#include "Dict.h"
#include "defs.h"
//------------------------------------------------------------------------------------

	
namespace xmlbeansxx {


//Dicts Impl


//------------
//ElemDict
int ElemDict::size() const {
    return (int) contents.size();
}

bool ElemDict::hasEmptyContent() const {
    XMLBEANSXX_FOREACH(ContentsType::const_iterator,it,contents) {
        if (it->value!=NULL)
            return false;
    }
    return true;
}

ElemDict::value_type ElemDict::find(const QName& u,int nr) const {
    XMLBEANSXX_FOREACH(ContentsType::const_iterator,it,contents) {
        if (it->name==u) {
            nr--;
            if (nr==-1)
                return *it;
        }
    }
    return value_type();
}

int ElemDict::count(const QName& u) const {
    int ile=0;
    XMLBEANSXX_FOREACH(ContentsType::const_iterator,it,contents) {
        if (it->name==u) {
            ile++;
        }
    }
    return ile;
}

/** Duplicates u, but not v */
void ElemDict::add(const QName& u,ElemDict::data_type v) {
    contents.push_back(value_type(u,v));
}

void ElemDict::set(const QName& u,int pos,ElemDict::data_type v) {
    XMLBEANSXX_FOREACH(ContentsType::iterator,it,contents) {
        if (it->name==u) {
            if (pos==0) {
	    	it->name=u;
                it->value=v;
                return;
            }
            pos--;
        }
    }
    for(int i=0;i<pos;i++) add(u,ElemDict::data_type());
    add(u,v);
    //throw new XmlException("Trying to set value at non existing position");
}

int ElemDict::del(const QName& u) {
    int p=0,p2=0;
    int ile=0;
    while (p<int(contents.size())) {
        if (p!=p2)
            contents[p2]=contents[p];
        if (contents[p].name==u) {
            //delete contents[p].name;
            contents[p].name=QName();
            contents[p].value.reset();
            ile++;
            p++;
        } else {
            p++;
            p2++;
        }
    }
    for(int i=0;i<ile;i++) contents.pop_back();
    return ile;
}

void ElemDict::free() {
    contents.clear();
}

void ElemDict::removeAt(const QName& name,int index) {
    XMLBEANSXX_FOREACH(ContentsType::iterator,it,contents) {
        if (it->name==name) {
            index--;
            if (index==-1) {
                contents.erase(it);
                return;
            }
        }
    }
}


}
