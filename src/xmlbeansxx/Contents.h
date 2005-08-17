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


#ifndef _XMLBEANSXX_CONTENTS_H_
#define _XMLBEANSXX_CONTENTS_H_

#include "BoostAssert.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "XmlOptions.h"
#include "shared_array.h"
#include "SchemaType.h"
#include "StoreString.h"
#include "SimpleValue.h"

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#endif


	
namespace xmlbeansxx {
typedef XmlObjectPtr (*ObjectCreatorFn)();

class SchemaProperty;
typedef boost::shared_ptr<SchemaProperty> SchemaPropertyPtr;
typedef StoreString DictNameType;

class Contents;
typedef boost::shared_ptr<Contents> ContentsPtr;

/** This struct provides content management for every XmlObject. Access to contents is synchronized. */
class Contents: public SimpleValue {
public:
    virtual ~Contents() {}

    /** Gets mutex used for contents synchronization */
    virtual boost::recursive_mutex &getMutex() const = 0;
    
    /** Adds new attribute */
    virtual void appendAttr(const DictNameType &name,const std::string &value) = 0;
    
    /** Adds new element */
    virtual void appendElem(const DictNameType &name,const XmlObjectPtr &value) = 0;

    /** @return true while this contents don't have any attributes and elements */
    virtual bool hasEmptyContent() const = 0;

    /** @return names and values of all sub elements */
    virtual std::vector<std::pair<DictNameType,XmlObjectPtr> > getElems() const = 0;

    /** @return names and values of all sub attributes */
    virtual std::vector<std::pair<DictNameType,std::string> > getAttrs() const = 0;

    /** Deeply copies all contents from orig. */
    virtual void copyFrom(const Contents *orig) = 0;

    /**
     * Chceck whether this contents have some elements (complex content).
     */
    virtual bool hasElements() const = 0;

    virtual void serialize(std::string elemName,std::ostream &o,const XmlObject *obj,int emptyNsID,bool printXsiType) const = 0;
    
    /** Removes elements from this contents. */
    virtual void free() = 0;

    /** @deprecated @see xmlbeans::XmlObject::digest */
    virtual std::string digest(XmlObject *parent) = 0;

    /**
     * This is called from serialize from XmlObject beeing Document type.
     */
    virtual void serializeDocument(std::ostream &o,const XmlOptionsPtr &options,const XmlObject *obj) const = 0;

    /**
     * Takes string value of given attribute. 
     * @return NULL while attribute doesn't exist
     */
    virtual StringPtr getAttr(const DictNameType &attrName) const = 0;
    
    /**
     * Takes attribute from contents as XmlObject of proper type.
     */
    virtual XmlObjectPtr getAttrObject(const DictNameType &attrName, XmlObject *parent) const = 0;
    
    /** 
     * Creates only XmlObject of proper type with attrValue string content. 
     */
    virtual XmlObjectPtr getAttrObject2(const DictNameType &attrName, const std::string &attrValue, XmlObject *parent) const = 0;

    /**
     * Sets specified attribute with value. Setting NULL value removes attribute.
     */
    virtual void setAttr(const DictNameType &attrName,const StringPtr &value) = 0;

    /**
     * @return element of specified name (NULL if doesn't exist)
     */
    virtual XmlObjectPtr getElem(const DictNameType &elemName) const = 0;

    /**
     * Retrieves element of specified name at specified position.
     * If position is out of bounds, it returns NULL.
     */
    virtual XmlObjectPtr getElemAt(const DictNameType &elemName,int index) const = 0;

    /** Gets specified element (creates one if there is NULL) */
    virtual XmlObjectPtr cgetElemAt(const DictNameType &elemName,int index,ObjectCreatorFn createFn,XmlObject *creator) = 0;
    
    /**
     * Sets element of given name and value. NULL removes element.
     */
    virtual void setElem(const DictNameType &elemName,const XmlObjectPtr &value) = 0;

    /**
     * Sets element of given name at specified index. If index is out of bounds, then some NULL values are added. 
     */
    virtual void setElemAt(const DictNameType &elemName,int index,const XmlObjectPtr &value) = 0;

    /** Removes element at specified index (by cutting it off). */
    virtual void removeElemAt(const DictNameType &elemName,int index) = 0;

    /** Removes all elements of given name. */
    virtual void removeElems(const DictNameType &name) = 0;

    /** Counts number of elements of given name */
    virtual int countElems(const DictNameType &name) const = 0;

    /** @return array of elements of given name */
    virtual shared_array<XmlObjectPtr > getElemArray(const DictNameType &elemName) const = 0;
    
    /** Sets array of elements of given name. */
    virtual void setElemArray(const DictNameType &elemName,const shared_array<XmlObjectPtr > &value) = 0;

    /** Used for serialization. */
    virtual void serialize2(int emptyNsID,bool printXsiType,std::string elemName,std::ostream &o,const XmlObject *obj) const = 0;
    
    /** Adds default elements or attributes to this contents if they don't exist. */
    virtual void insertDefaults(const SchemaType *schemaType) = 0;
};

}


#endif //XMLBEANSXX_CONTENTS_H_

