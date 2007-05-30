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


#include "Dict.h"
#include "defs.h"
//------------------------------------------------------------------------------------

	
namespace xmlbeansxx {


//Dicts Impl

int AttrDict::size() const {
    return (int) contents.size();
}

bool AttrDict::hasEmptyContent() const {
    FOREACH(it,contents) {
        return false;
        //if (it->value!=NULL)
        //    return false;
    }
    return true;
}

AttrDict::T AttrDict::find(const QName& u) const {
    FOREACH(it,contents) {
        if (it->name==u)  return AttrDict::T(it->value);
    }
    return AttrDict::T();
}

int AttrDict::count(const QName& u) const {
    int ile=0;
    FOREACH(it,contents) {
        if (it->name==u) {
            ile++;
        }
    }
    return ile;
}

/** Duplicates u and v */
void AttrDict::add(const QName& u,AttrDict::T v) {
    contents.push_back(DictEl<AttrDict::T>(u,v));
}

void AttrDict::set(const QName& u,int pos,AttrDict::T v) {
    FOREACH(it,contents) {
        if (it->name==u) {
            if (pos==0) {
                it->value=v;
                return;
            }
            pos--;
        }
    }
    FOR(i,pos) add
        (u,(AttrDict::T)NULL);
    add
        (u,v);
    //throw new XmlException("Trying to set value at non existing position");
}

int AttrDict::del(const QName& u) {
    int p=0,p2=0;
    int ile=0;
    while (p<int(contents.size())) {
        if (p!=p2)
            contents[p2]=contents[p];
        if (contents[p].name==u) {
            contents[p].name=QName(); contents[p].value=std::string();
            ile++;
            p++;
        } else {
            p++;
            p2++;
        }
    }
    FOR(i,ile) contents.pop_back();
    return ile;
}

void AttrDict::free() {
    /*
    FOREACH(it,contents) {
        if (it->value!=NULL) {
            delete it->value;
            it->value=NULL;
        }
        delete it->name;
        it->name=NULL;
    }*/
    contents.clear();
}

//------------
//ElemDict
int ElemDict::size() const {
    return (int) contents.size();
}

bool ElemDict::hasEmptyContent() const {
    FOREACH(it,contents) {
        if (it->value!=NULL)
            return false;
    }
    return true;
}

ElemDict::T ElemDict::find(const QName& u,int nr) const {
    FOREACH(it,contents) {
        if (it->name==u) {
            nr--;
            if (nr==-1)
                return it->value;
        }
    }
    return ElemDict::T();
}

int ElemDict::count(const QName& u) const {
    int ile=0;
    FOREACH(it,contents) {
        if (it->name==u) {
            ile++;
        }
    }
    return ile;
}

/** Duplicates u, but not v */
void ElemDict::add(const QName& u,ElemDict::T v) {
    contents.push_back(DictEl<ElemDict::T>(u,v));
    //logger.debug("ElemDict::add - %s",v->className());
}

void ElemDict::set
    (const QName& u,int pos,ElemDict::T v) {
    FOREACH(it,contents) {
        if (it->name==u) {
            if (pos==0) {
                it->value=v;
                return;
            }
            pos--;
        }
    }
    FOR(i,pos) add
        (u,ElemDict::T());
    add
        (u,v);
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
    FOR(i,ile) contents.pop_back();
    return ile;
}

void ElemDict::free() {
    contents.clear();
}

void ElemDict::removeAt(const QName& name,int index) {
    FOREACH(it,contents) {
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
