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

#include <map>
#include <string>
#include <vector>
#include "BoostAssert.h"
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "Dict.h"
#include "XmlOptions.h"
#include "shared_array.h"
#include "SchemaType.h"

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#endif


	
namespace xmlbeansxx {
typedef XmlObjectPtr (*ObjectCreatorFn)();

class SchemaProperty;
DECLARE_PTR(SchemaProperty,SchemaPropertyPtr,constSchemaPropertyPtr)


/** This struct provides content management for every XmlObject. Access to contents is synchronized. */
struct Contents {
private:
    static log4cxx::LoggerPtr log;
    AttrDict attrs;
    ElemDict elems;
    std::string value;

public:
#ifdef BOOST_HAS_THREADS
	mutable boost::recursive_mutex mutex;
#endif
	
public:
    Contents();
    Contents(const Contents &orig);
    ~Contents();
	
    /** Sets string content. */
	void setSimpleContent(const std::string &c);
    
    /** Gets string content. */
	std::string getSimpleContent() const;

    /** Adds new attribute */
    void appendAttr(const std::string name,const std::string value);
    
    /** Adds new element */
    void appendElem(const std::string name,XmlObjectPtr value);

    /** @return true while this contents don't have any attributes and elements */
    bool hasEmptyContent() const;

    /** @return names and values of all sub elements */
    std::vector<std::pair<std::string,XmlObjectPtr > > getElems() const;

    /** @return names and values of all sub attributes */
    std::vector<std::pair<std::string,std::string> > getAttrs() const;

    /** Copies contents from orig (not deeply). */
    void simpleCopyFrom(const Contents &orig);

    /** Deeply copies all contents from orig. */
    void copyFrom(const Contents &orig);

    /**
     * Chceck whether this contents have some elements (complex content).
     */
    bool hasElements() const;

    void serialize(std::string elemName,std::ostream &o,const XmlObject *obj,int emptyNsID,bool printXsiType) const;
    
    /** Removes elements from this contents. */
    void free();

    /** @deprecated @see xmlbeans::XmlObject::digest */
    std::string digest(XmlObject *parent);

    /**
     * This is called from serialize from XmlObject beeing Document type.
     */
    void serializeDocument(std::ostream &o,XmlOptionsPtr options,const XmlObject *obj) const;

    /**
     * Takes string value of given attribute. 
     * @return NULL while attribute doesn't exist
     */
    StringPtr getAttr(std::string attrName) const;
    
    /**
     * Takes attribute from contents as XmlObject of proper type.
     */
    XmlObjectPtr getAttrObject(const std::string &attrName, XmlObject *parent) const;
    
    /** 
     * Creates only XmlObject of proper type with attrValue string content. 
     */
    XmlObjectPtr getAttrObject2(const std::string &attrName, const std::string &attrValue, XmlObject *parent) const;

    /**
     * Sets specified attribute with value. Setting NULL value removes attribute.
     */
    void setAttr(std::string elemName,StringPtr value);

    /**
     * @return element of specified name (NULL if doesn't exist)
     */
    XmlObjectPtr getElem(std::string elemName) const;

    /**
     * Retrieves element of specified name at specified position.
     * If position is out of bounds, it returns NULL.
     */
    XmlObjectPtr getElemAt(std::string elemName,int index) const;

    /** Gets specified element (creates one if there is NULL) */
    XmlObjectPtr cgetElemAt(std::string elemName,int index,ObjectCreatorFn createFn,XmlObject *creator);
    
    /**
     * Sets element of given name and value. NULL removes element.
     */
    void setElem(std::string elemName,XmlObjectPtr value);

    /**
     * Sets element of given name at specified index. If index is out of bounds, then some NULL values are added. 
     */
    void setElemAt(std::string elemName,int index,XmlObjectPtr value);

    /** Removes element at specified index (by cutting it off). */
    void removeElemAt(std::string elemName,int index);

    /** Removes all elements of given name. */
    void removeElems(std::string name);

    /** Counts number of elements of given name */
    int countElems(std::string name) const;

    /** @return array of elements of given name */
    shared_array<XmlObjectPtr > getElemArray(std::string elemName) const;
    
    /** Sets array of elements of given name. */
    void setElemArray(std::string elemName,const shared_array<XmlObjectPtr > &value);

private:
    void serializeAttrs(std::ostream &o) const;
    void serializeElems(int emptyNsID,std::ostream &o,const std::map<std::string,SchemaPropertyPtr> *order) const;
public:
    /** Used for serialization. */
    void serialize2(int emptyNsID,bool printXsiType,std::string elemName,std::ostream &o,const XmlObject *obj) const;
    
    /** Adds default elements or attributes to this contents if they don't exist. */
    void insertDefaults(const SchemaType *schemaType);
};

}


#endif //XMLBEANSXX_CONTENTS_H_

