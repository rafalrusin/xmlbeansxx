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


#include <sstream>

#include "XmlTypesGen.h"
#include "geum_types.h"
#include "defs.h"
#include "BoostAssert.h"


namespace xmlbeansxx {

//---------------------------------------------------------------------------------------
// Geum
/**
 * xpath delimiter std::string
 */
static const char DELIM =  '/';

/**
 * CDATA prefix <pre><![CDATA[</pre>
 */
static const std::string CDATA_PREFIX = "<![CDATA[";

/**
 * CDATA suffix <pre>]]></pre>
 
 */
static const std::string CDATA_SUFFIX = "]]>";

using namespace std;
//Xpath operations

XmlObject *XmlObject::walkXpath(const string xpath,string &lastName,bool createElems) {
    int delim=xpath.find('/');
    if (delim==0) {
        return walkXpath(xpath.substr(1),lastName,createElems);
    } else if (delim==-1) {
        lastName=xpath;
        return this;
    } else {
        string t=xpath.substr(0,delim);

        pair<string,int> e=decomposeElem(t);
        XmlObjectPtr sub=contents.getElemAt(e.first,e.second);
        if (sub==NULL) {
            if (createElems) {
                sub=XmlObjectPtr(new XmlObject());
                contents.setElemAt(e.first,e.second,sub);
            } else
                return NULL;
        }
        return sub->walkXpath(xpath.substr(delim+1),lastName,createElems);
    }
}

bool XmlObject::isAttr(const string name) {
    return name[0]=='@';
}
pair<string,int> XmlObject::decomposeElem(const string name) {
    int a,b;
    if ((a=name.find('['))!=-1) {
        int pos=0;
        a++;
        b=name.find(']');
        BOOST_ASSERT(b!=-1);
        string n=name.substr(a,b-a);
        pos=atoi(n.c_str())-1;
        return pair<string,int>(name.substr(0,a-1),pos);
    } else {
        return pair<string,int>(name,0);
    }
}

std::string XmlObject::getXpathValue(const string xpath) {
    string t;
    XmlObject *obj=walkXpath(xpath,t,false);
    if (obj==NULL) {
        throw XpathException("getXpathValue: xpath not found " + std::string(xpath),
                             XpathException::XPATH_NOT_FOUND);
    }
    if (isAttr(t)) {
        VAL(c,obj->contents.getAttr(t.substr(1)));
        if (c==NULL)
            throw XpathException("getXpathValue: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        return *c;
    } else {
        pair<string,int> e=decomposeElem(t);

        XmlObjectPtr sub=obj->contents.getElemAt(e.first,e.second);
        if (sub==NULL) {
            throw XpathException("getXpathValue: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        }
        return sub->getSimpleContent();
    }
}

void XmlObject::setXpathValue(const string xpath,const string value) {
    string t;
    XmlObject *obj=walkXpath(xpath,t,false);
    if (obj==NULL) {
        throw XpathException("setXpathValue: xpath not found " + std::string(xpath),
                             XpathException::XPATH_NOT_FOUND);
    }
    if (isAttr(t)) {
        return obj->contents.setAttr(t.substr(1),StringPtr(new std::string(value)));
    } else {
        pair<string,int> e=decomposeElem(t);

        XmlObjectPtr sub(obj->contents.getElemAt(e.first,e.second));
        if (sub==NULL) {
            throw XpathException("setXpathValue: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        }
        if (sub->contents.hasElements()) {
            throw XpathException("setXpathValue: invalid xpath (this element has complex content) " + std::string(xpath),
                                 XpathException::INVALID_XPATH_FORMAT);
        }
        sub->setSimpleContent(value);
    }
}

void XmlObject::addXpath(const string xpath,const string value) {
    string t;
    XmlObject *obj(walkXpath(xpath,t,true));
    BOOST_ASSERT(obj!=NULL);
    if (isAttr(t)) {
        obj->contents.setAttr(t.substr(1),StringPtr(new string(value)));
    } else {
        pair<string,int> e=decomposeElem(t);

        XmlObjectPtr sub=XmlObjectPtr(new XmlObject());
        sub->setSimpleContent(value);
        obj->contents.setElemAt(e.first,e.second,sub);
    }
}

void XmlObject::addCdataXpath(const std::string xpath, const std::string value) {
    std::string x;
    x.append(CDATA_PREFIX);
    x.append(value);
    x.append(CDATA_SUFFIX);
    addXpath(xpath, x);
}

void XmlObject::removeXpath(const std::string xpath) {
    int delim=xpath.find('/');
    if (delim==0) {
        removeXpath(xpath.substr(1));
        return;
    } else if (delim==-1) {
        const string &t=xpath;
        if (isAttr(t)) {
            contents.setAttr(t.substr(1),StringPtr());
        } else {
            pair<string,int> e=decomposeElem(t);

            XmlObjectPtr sub(contents.getElemAt(e.first,e.second));
            if (sub!=NULL) {
                contents.setElemAt(e.first,e.second,XmlObjectPtr());
            } else {
                throw XpathException("removeXpath: xpath not found " + std::string(xpath),
                                     XpathException::XPATH_NOT_FOUND);
            }
        }
        return;
    } else {
        string t=xpath.substr(0,delim);

        pair<string,int> e=decomposeElem(t);
        XmlObjectPtr sub(contents.getElemAt(e.first,e.second));
        if (sub==NULL) {
            throw XpathException("removeXpath: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        }
        sub->removeXpath(xpath.substr(delim+1));
        if (sub->contents.hasEmptyContent()) {
            contents.setElemAt(e.first,e.second,XmlObjectPtr());
        }
    }
}

vector<std::string>* XmlObject::getXpaths(const std::string xpath) {
    vector<std::string>* v=new vector<std::string>();

    string t;
    XmlObject *obj(walkXpath(xpath,t,false));
    if (obj==NULL) {
        throw XpathException("getXpaths: xpath not found " + std::string(xpath),
                             XpathException::XPATH_NOT_FOUND);
    }
    if (isAttr(t)) {
        v->push_back(xpath);
        return v;
    } else {
        pair<string,int> e=decomposeElem(t);
        BOOST_ASSERT(e.second==0);

        int nr=0;
        vector<pair<string,XmlObjectPtr > > els=contents.getElems();
        FOREACH(it,els) {
            if (e.first==it->first) {
                ostringstream ss;
                ss<<xpath<<"["<<nr<<"]";
                v->push_back(ss.str());
                nr++;
            }
        }
        return v;
    }
}


void XmlObject::getXpathsLikeMapRec(XpathsMap &m,const std::string prefix) {
    vector<pair<string,string> > ats=contents.getAttrs();
    FOREACH(it,ats) {
        ostringstream ss;
        ss<<prefix<<"@"<<it->first;
        m.insert(pair<string,string>(ss.str(),it->second));
    }
    map<string,int> counter;

    vector<pair<string,XmlObjectPtr > > els=contents.getElems();
    FOREACH(it,els) {
        ostringstream ss;
        counter[it->first]++;
        int c=counter[it->first];
        ss<<prefix<<it->first;
        if (c>1) {
            ss<<"["<<c<<"]";
        }
        string p=ss.str();
        if (it->second!=NULL) {
            if (it->second->contents.hasElements()) {
                //cout<<p<<" i:"<<c<<" has empty content\n";
                const string x=it->second->getSimpleContent();
                m.insert(pair<string,string>(p,x));
            }
            it->second->getXpathsLikeMapRec(m,p+"/");
        } else {
            //m.push_back(p,"");
        }
    }
}

vector<std::string>* XmlObject::getXpathsLike(const std::string xpath) {
    XpathsMap *map=getXpathsLikeMap(xpath);
    vector<std::string>* v=new vector<std::string>();
    FOREACH(it,*map) {
        v->push_back(it->first);
    }
    delete map;
    return v;
}

XpathsMap* XmlObject::getXpathsLikeMap(const std::string xpath) {
    string t;
    XmlObject *obj=walkXpath(xpath,t,false);
    if (obj==NULL) {
        throw XpathException("getXpathsLikeMap: xpath not found " + std::string(xpath),
                             XpathException::XPATH_NOT_FOUND);
    }
    if (isAttr(t)) {
        throw XpathException("getXpathsLikeMap: invalid xpath (it's an attribute) " + std::string(xpath),
                             XpathException::INVALID_XPATH_FORMAT);
    } else {
        pair<string,int> e=decomposeElem(t);
        XmlObjectPtr obj2=obj->contents.getElemAt(e.first,e.second);
        if (obj2==NULL) {
            throw XpathException("getXpathsLikeMap: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        }
        XpathsMap* map = new XpathsMap();
        obj2->getXpathsLikeMapRec(*map,xpath+"/");
        return map;
    }
}

}
