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

#include "ContentsImpl.h"
#include <log4cxx/logger.h>
#include "Tracer.h"
#include "defs.h"
#include "XmlTypesGen.h"
#include <string>
#include <iostream>
#include <set>
#include <map>
#include "TypeSystem.h"
#include "TextUtils.h"
#include "SchemaProperty.h"
using namespace std;

//------------------------------------------------------------------------------------

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/detail/lock.hpp>
#define SYNC(mutex) boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(mutex);
#else
#define SYNC(mutex)
#endif


/*
//Disable XmlObject synchronization
#undef SYNC
#define SYNC(getMutex())
*/

//Disable tracing
#undef TRACER
#define TRACER(logger, msg)

namespace xmlbeansxx {
namespace {
log4cxx::LoggerPtr ContentsImpl::LOG = log4cxx::Logger::getLogger("xmlbeansxx.ContentsImpl");
}

//Contents Impl

boost::recursive_mutex &ContentsImpl::getMutex() const {
    return _mutex;
}

void ContentsImpl::setSimpleContent(const std::string &c) {
    SYNC(getMutex())
    value=c;
}

std::string ContentsImpl::getSimpleContent() const {
    SYNC(getMutex())
    return value;
}

bool ContentsImpl::hasEmptyContent() const {
    return attrs.hasEmptyContent() && elems.hasEmptyContent();
}

void ContentsImpl::copyFrom(const Contents *obj) {
    SYNC(getMutex())
/*
#ifdef BOOST_HAS_THREADS
    boost::detail::thread::scoped_lock<boost::recursive_mutex> lock2(obj.mutex);
#endif
*/
    VAL(objElems, obj->getElems());
    FOREACH(it,objElems) {
        XmlObjectPtr o=it->second;
        if (o!=NULL)
            o=o->clone();
        elems.add(it->first,o);
    }

    VAL(objAttrs, obj->getAttrs());
    FOREACH(it,objAttrs) {
        attrs.add(it->first,it->second);
    }
}

bool ContentsImpl::hasElements() const {
    SYNC(getMutex())
    return elems.size()>0;
}

void ContentsImpl::serializeAttrs(ostream &o) const {
    SYNC(getMutex())
    FOREACH(it,attrs.contents) {
        //if (it->value!=NULL) {
        o<<" "<<it->name<<"='"<<TextUtils::exchangeEntities(it->value)<<"'";
        //}
    }
}

namespace {
template<class T1,class T2,class T3>
struct three {
    T1 first; T2 second; T3 third;
    three(const T1 &a,const T2 &b,const T3 &c): first(a), second(b),third(c) {}
    bool operator <(const three<T1,T2,T3> &other) const {
        if (first==other.first) return second<other.second;
        else return first<other.first;
    }
};
    
bool shallPrintXsiType(const SchemaType *xsdDefined,const XmlObjectPtr &obj) {
    static XmlObjectPtr src=XmlObject::Factory::newInstance();
    return obj->getSchemaType()->classTypeInfo != src->getSchemaType()->classTypeInfo 
        && obj->getSchemaType()->classTypeInfo != xsdDefined->classTypeInfo;
}
}

void ContentsImpl::serializeElems(int emptyNsID,ostream &o,const std::map<std::string,SchemaPropertyPtr> *order) const {
    SYNC(getMutex())
    TRACER(log,"serializeElems")
    //logger.debug("ContentsImpl::serializeElems - order=%p, elements:",order);
    //--
    if (order!=NULL) {
        //sorting
        set<three<int,int,SchemaProperty *> > s;
        int i=0;
        /*
        FOREACH(it,*order) {
        logger.debug("ContentsImpl::serializeElems - %s->%i",it->first.c_str(),it->second);
        }
        logger.debug("ContentsImpl::serializeElems ---");
         */

        FOREACH(it,elems.contents) {
            std::map<std::string,SchemaPropertyPtr>::const_iterator it2=(*order).find(it->name);
            if (it2!=(*order).end()) {
                s.insert(three<int,int,SchemaProperty *>(it2->second->order,i,it2->second.get()));
            } else {
                s.insert(three<int,int,SchemaProperty *>(-1,i,NULL));
            }
            i++;
        }

        /*
        FOREACH(it,s) {
        logger.debug("ContentsImpl::serializeElems - pair<%i,%i> name:%s",it->first,it->second,elems.contents[it->second].name);
        }
        logger.debug("ContentsImpl::serializeElems ---");
         */

        FOREACH(it,s) {
            XmlObjectPtr value(elems.contents[it->second].value);
            if (value!=NULL) {
                bool printXsiType= it->third==NULL || shallPrintXsiType(it->third->schemaType,value);
                value->getContents()->serialize(elems.contents[it->second].name,o,value.get(),emptyNsID,printXsiType);
            }
        }
    } else {
        LOG4CXX_DEBUG(LOG,"Elements order is NULL");
        throw IllegalStateException();
        /*
        FOREACH(it,elems.contents) {
            if (it->value!=NULL) {
                it->value->contents.serialize(it->name,o,it->value.get(),emptyNsID);
            }
        }*/
    }
}

void ContentsImpl::serialize2(int emptyNsID,bool printXsiType,std::string elemName,ostream &o,const XmlObject *obj) const {
    TRACER(log,"serialize2")
    SYNC(getMutex())

    int ans=obj->getSchemaType()->arrayXsdNamespaceID;
    if (ans!=-1) {
        //it's an array
        std::string typeName=obj->getSchemaType()->arrayXsdTypeName;
        std::string def;

        o<<" xsi:array='";
        if (ans==globalTypeSystem()->xs_ns()) {
            o<<"xs:";
        } else if (ans==globalTypeSystem()->xsi_ns()) {
            o<<"xsi:";
        } else if (ans!=emptyNsID) {
            emptyNsID=ans;
            std::string emptyNs=globalTypeSystem()->getNamespaceName(emptyNsID);
            def=std::string(" xmlns='")+globalTypeSystem()->getNamespaceName(emptyNsID)+"'";
        }

        o<<typeName;
        o<<"'";

        o<<def;

    } else {
        //it's an object
        if (printXsiType) {
            int ns=obj->getSchemaType()->xsdNamespaceID;
            if (ns!=-1) {
                std::string typeName=obj->getSchemaType()->xsdTypeName;
                std::string def;

                o<<" xsi:type='";
                if (ns==globalTypeSystem()->xs_ns()) {
                    o<<"xs:";
                } else if (ns==globalTypeSystem()->xsi_ns()) {
                    o<<"xsi:";
                } else if (ns!=emptyNsID) {
                    emptyNsID=ns;
                    std::string emptyNs=globalTypeSystem()->getNamespaceName(emptyNsID);
                    def=std::string(" xmlns='")+globalTypeSystem()->getNamespaceName(emptyNsID)+"'";
                }

                o<<typeName;
                o<<"'";

                o<<def;
            } else {
                //no xsi type printed
                if (printXsiType && obj->documentElementNamespaceID()==-1) {
                    std::string msg=std::string("Inserted element '")+elemName+std::string("' of class: ")+std::string(obj->getSchemaType()->className)+std::string("' is a schema anonymous type and it's type is not equal to schema expected type.");
                    LOG4CXX_DEBUG(LOG,msg);
                    throw XmlException(msg);
                }
                
            }
        }
    }

    //o<<" xsi:class=\""<<obj->className()<<"\"";
    //
    serializeAttrs(o);

    {
        std::string cnt=obj->exchangeEntities(obj->getSimpleContent());
        if (cnt==std::string() && !const_cast<XmlObject *>(obj)->getContents()->hasElements()) {
            o<<"/>";
        } else {
            o<<">";
            o<<cnt;
            serializeElems(emptyNsID,o,&(obj->getSchemaType()->elements));
            o<<"</"<<elemName<<">";
        }
    }
}

void ContentsImpl::serialize(std::string elemName,ostream &o,const XmlObject *obj,int emptyNsID,bool printXsiType) const {
    o<<"<"<<elemName;
    serialize2(emptyNsID,printXsiType,elemName,o,obj);
}

std::string ContentsImpl::digest(XmlObject *parent) {
    std::string r;
    
    std::vector<std::pair<DictNameType,std::string> > as=getAttrs();
    std::vector<std::pair<DictNameType,XmlObjectPtr > > es=getElems();

    sort(as.begin(), as.end());
    FOREACH(it,as) {
        XmlObjectPtr a=getAttrObject2(it->first,it->second,parent);
        r+=" ";
        r+=it->first;
        r+="=\"";
        r+=a->exchangeEntities(a->getCanonicalContent());
        r+="\"";
    }
    r+=">";
    r+=parent->exchangeEntities(parent->getCanonicalContent());
    
    std::map<std::pair<std::string,int>, XmlObjectPtr> m;
    std::map<std::string, int> counters;
    FOREACH(it,es) {
        if (it->second!=NULL) {
            m[std::pair<std::string,int>(it->first,counters[it->first])]=it->second;
            counters[it->first]++;
        }
    }
    FOREACH(it,m) {
        r+="<";
        r+=it->first.first;
        r+=it->second->digest();
        r+="</";
        r+=it->first.first;
        r+=">";
    }
    return r;
}

void ContentsImpl::free() {
    //logger->debug(std::string("ContentsImpl::free() - start"));
    SYNC(getMutex())
    elems.free();
    attrs.free();
    value=std::string();
    //logger->debug(std::string("ContentsImpl::free() - finish"));
}

void ContentsImpl::serializeDocument(ostream &o,const XmlOptionsPtr &options,const XmlObject *obj) const {
    TRACER(log,"serializeDocument")
    //XmlContextPtr xmlContext(new XmlContext());
    SYNC(getMutex())

    if (options->getPrintXmlDeclaration())
        o<<"<?xml version='1.0' encoding='UTF-8'?>\n";

    VAL(it,elems.contents.begin());
    if (it==elems.contents.end())
        return;

    o<<"<"<<it->name;
    LOG4CXX_DEBUG(LOG,"document root element namespace id: "+TextUtils::intToString(it->value->getSchemaType()->xsdNamespaceID));
    int emptyNsID=obj->documentElementNamespaceID();
    std::string docns=globalTypeSystem()->getNamespaceName(emptyNsID);
    if (docns!="") {
        o<<" xmlns='"<<docns<<"'";
    }
    o<<" xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'";
    o<<" xmlns:xs='http://www.w3.org/2001/XMLSchema'";

    //----------
    const std::map<std::string,SchemaPropertyPtr> *order=&(obj->getSchemaType()->elements);
    std::map<std::string,SchemaPropertyPtr>::const_iterator propIt=(*order).find(it->name);
    SchemaPropertyPtr prop;
    if (propIt!=(*order).end())
        prop=propIt->second;

    if (prop==NULL) {
        std::string msg=std::string("Serializing document of class: ")+std::string(obj->getSchemaType()->className)+std::string(". It's root element name should not be '")+it->name+std::string("'");
        LOG4CXX_DEBUG(LOG,msg);
        throw XmlException(msg);
    }

    bool printXsiType=shallPrintXsiType(prop->schemaType,it->value);
    //----------
    it->value->getContents()->serialize2(emptyNsID,printXsiType,it->name,o,it->value.get());
    o<<"\n";
}

StringPtr ContentsImpl::getAttr(const DictNameType &elemName) const {
    SYNC(getMutex())
    return attrs.find(elemName);
    /*
    const char *v=attrs.find(elemName);
    if (v==NULL)
        return constStringPtr();
    else
        return constStringPtr(new std::string(v));
    */
}

XmlObjectPtr ContentsImpl::getAttrObject(const DictNameType &attrName, XmlObject *parent) const {
    SYNC(getMutex())
    StringPtr v=attrs.find(attrName);
    if (v==NULL) return XmlObjectPtr();
    else return getAttrObject2(attrName,*v,parent);
}

XmlObjectPtr ContentsImpl::getAttrObject2(const DictNameType &attrName, const std::string &attrValue, XmlObject *parent) const {
    XmlObjectPtr r;
    SchemaPropertyPtr prop=parent->getSchemaType()->findAttribute(attrName);
    if (prop!=NULL) {
        r=prop->schemaType->createFn();
    } else {
        r=XmlObject::Factory::newInstance();
    }
    r->setSimpleContent(attrValue);
    return r;
}

void ContentsImpl::setAttr(const DictNameType &attrName,const StringPtr &value) {
    SYNC(getMutex())
    attrs.del(attrName);
    //cout<<"\n!adding "<<elemName<<"->"<<value<<"\n";
    if (value!=NULL)
        attrs.add(attrName,*value);
}

XmlObjectPtr ContentsImpl::getElem(const DictNameType &elemName) const {
    SYNC(getMutex())
    return elems.find(elemName,0);
}

XmlObjectPtr ContentsImpl::getElemAt(const DictNameType &elemName,int index) const {
    SYNC(getMutex())
    return elems.find(elemName,index);
}

XmlObjectPtr ContentsImpl::cgetElemAt(const DictNameType &elemName,int index,ObjectCreatorFn createFn,XmlObject *creator) {
    TRACER(log,"cgetElemAt")

    XmlObjectPtr e(getElemAt(elemName,index));
    if (e!=NULL)
        return e;

    if (createFn!=NULL) {
        e=createFn();
        LOG4CXX_DEBUG(LOG,"CreateFn returned: "+TextUtils::ptrToString(e.get()));
    } else {
        LOG4CXX_DEBUG(LOG,"creating subObject on creator");
        e=creator->getSchemaType()->createSubObject(elemName);
    }
    setElemAt(elemName,index,e);
    return e;
}

/*
XmlObjectPtr ContentsImpl::cgetElemAt(std::string elemName,int index,int namespaceID,std::string typeName,XmlObject *creator) {
    TRACER(logger,"ContentsImpl::cgetElemAt()")
    VAL(e,getElemAt(elemName,index));
    if (e!=NULL)
        return e;
    if (namespaceID!=-1) {
        e=globalTypeSystem()->createByName(typeName,namespaceID);
    } else {
        logger->debug("ContentsImpl::cgetElemAt() - creating subObject");
        e=creator->createSubObject(elemName);
    }
    setElemAt(elemName,index,e);
    return e;
}*/

void ContentsImpl::setElem(const DictNameType &elemName,const XmlObjectPtr &value) {
    SYNC(getMutex())
    elems.del(elemName);
    if (value!=NULL)
        elems.add(elemName,value);
}

void ContentsImpl::setElemAt(const DictNameType &elemName,int index,const XmlObjectPtr &value) {
    SYNC(getMutex())
    elems.set(elemName,index,value);
}

void ContentsImpl::removeElemAt(const DictNameType &elemName,int index) {
    SYNC(getMutex())
    elems.removeAt(elemName,index);
    
}


void ContentsImpl::removeElems(const DictNameType &name) {
    SYNC(getMutex())
    elems.del(name);
}

int ContentsImpl::countElems(const DictNameType &name) const {
    SYNC(getMutex())
    return elems.count(name);
}

vector<pair<DictNameType,XmlObjectPtr > > ContentsImpl::getElems() const {
    SYNC(getMutex())
    vector<pair<DictNameType,XmlObjectPtr > > v;
    FOREACH(it,elems.contents) {
        v.push_back(pair<DictNameType,XmlObjectPtr >(it->name,it->value));
    }
    return v;
}

vector<pair<DictNameType,string> > ContentsImpl::getAttrs() const {
    SYNC(getMutex())
    vector<pair<DictNameType,string> > v;
    FOREACH(it,attrs.contents) {
        v.push_back(pair<DictNameType,string>(it->name,it->value));
    }
    return v;
}

void ContentsImpl::appendAttr(const DictNameType &name,const std::string &value) {
    SYNC(getMutex())
    attrs.add(name,value);
}

void ContentsImpl::appendElem(const DictNameType &name,const XmlObjectPtr &value) {
    SYNC(getMutex())
    elems.add(name,value);
}

shared_array<XmlObjectPtr > ContentsImpl::getElemArray(const DictNameType &elemName) const {
    SYNC(getMutex())
    std::vector<XmlObjectPtr > r;
    FOREACH(it,elems.contents) {
        if (elemName==it->name) {
            r.push_back(it->value);
        }
    }
    return toSharedArray(r);
}

void ContentsImpl::setElemArray(const DictNameType &elemName,const shared_array<XmlObjectPtr > &value) {
    SYNC(getMutex())
    elems.del(elemName);
    for (int i=0;i<value.size();i++) {
        elems.add(elemName,value[i]);
    }
}

void ContentsImpl::insertDefaults(const SchemaType *schemaType) {
    SYNC(getMutex())
    {
        std::set<std::string> exist;
        FOREACH(it,attrs.contents) {
            exist.insert(it->name);
        }
        FOREACH(it, schemaType->attributes) {
            StringPtr def = it->second->singletonDefault;
            if (def != NULL && exist.count(it->first)==0) {
                appendAttr(it->first, *def);
            }
        }
    }
    {
        std::set<std::string> exist;
        FOREACH(it,elems.contents) {
            exist.insert(it->name);
        }
        FOREACH(it, schemaType->elements) {
            StringPtr def = it->second->singletonDefault;
            if (def != NULL && exist.count(it->first)==0) {
                XmlObjectPtr obj = it->second->schemaType->createFn();
                obj->setSimpleContent(*def);
                appendElem(it->first, obj);
            }
        }
    }
}

}

