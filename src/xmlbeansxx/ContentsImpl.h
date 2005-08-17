#ifndef XMLBEANSXX_CONTENTSIMPL_INCLUDED
#define XMLBEANSXX_CONTENTSIMPL_INCLUDED
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

#include "Contents.h"
#include "Dict.h"
namespace xmlbeansxx {
class ContentsImpl: public Contents {
private:
    static log4cxx::LoggerPtr LOG;
    AttrDict attrs;
    ElemDict elems;
    std::string value;

private:
#ifdef BOOST_HAS_THREADS
	mutable boost::recursive_mutex _mutex;
#endif
	
public:
    virtual ~ContentsImpl() {}
    virtual boost::recursive_mutex &getMutex() const;
    
	virtual void setSimpleContent(const std::string &c);
	virtual std::string getSimpleContent() const;

    virtual void appendAttr(const DictNameType &name,const std::string &value);
    virtual void appendElem(const DictNameType &name,const XmlObjectPtr &value);
    virtual bool hasEmptyContent() const;
    virtual std::vector<std::pair<DictNameType,XmlObjectPtr > > getElems() const;
    virtual std::vector<std::pair<DictNameType,std::string> > getAttrs() const;
    virtual void copyFrom(const Contents *orig);
    virtual bool hasElements() const;
    virtual void serialize(std::string elemName,std::ostream &o,const XmlObject *obj,int emptyNsID,bool printXsiType) const;
    virtual void free();
    virtual std::string digest(XmlObject *parent);
    virtual void serializeDocument(std::ostream &o,const XmlOptionsPtr &options,const XmlObject *obj) const;
    virtual StringPtr getAttr(const DictNameType &attrName) const;
    virtual XmlObjectPtr getAttrObject(const DictNameType &attrName, XmlObject *parent) const;
    virtual XmlObjectPtr getAttrObject2(const DictNameType &attrName, const std::string &attrValue, XmlObject *parent) const;
    virtual void setAttr(const DictNameType &attrName,const StringPtr &value);
    virtual XmlObjectPtr getElem(const DictNameType &elemName) const;
    virtual XmlObjectPtr getElemAt(const DictNameType &elemName,int index) const;
    virtual XmlObjectPtr cgetElemAt(const DictNameType &elemName,int index,ObjectCreatorFn createFn,XmlObject *creator);
    virtual void setElem(const DictNameType &elemName,const XmlObjectPtr &value);
    virtual void setElemAt(const DictNameType &elemName,int index,const XmlObjectPtr &value);
    virtual void removeElemAt(const DictNameType &elemName,int index);
    virtual void removeElems(const DictNameType &name);
    virtual int countElems(const DictNameType &name) const;
    virtual shared_array<XmlObjectPtr > getElemArray(const DictNameType &elemName) const;
    virtual void setElemArray(const DictNameType &elemName,const shared_array<XmlObjectPtr > &value);
    virtual void serialize2(int emptyNsID,bool printXsiType,std::string elemName,std::ostream &o,const XmlObject *obj) const;
    virtual void insertDefaults(const SchemaType *schemaType);

private:
    void serializeAttrs(std::ostream &o) const;
    void serializeElems(int emptyNsID,std::ostream &o,const std::map<std::string,SchemaPropertyPtr> *order) const;
};
}
#endif //XMLBEANSXX_CONTENTSIMPL_INCLUDED

