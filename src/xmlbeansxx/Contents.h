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
	
	void setSimpleContent(const std::string &c);
	std::string getSimpleContent() const;

    void appendAttr(const std::string name,const std::string value);
    void appendElem(const std::string name,XmlObjectPtr value);

    /** returns true while this contents don't have any attributes and elements */
    bool hasEmptyContent() const;

    /** returns names of all sub elements */
    std::vector<std::pair<std::string,XmlObjectPtr > > getElems() const;
    std::vector<std::pair<std::string,std::string> > getAttrs() const;

    /** Copies only orig contents to this object (not whole tree) */
    void simpleCopyFrom(const Contents &orig);

    /** Copies all contents from original object */
    void copyFrom(const Contents &orig);

    /**
     * Chceck whether this object has elements (complex content)
     */
    bool hasElements() const;

    void serialize(std::string elemName,std::ostream &o,const XmlObject *obj,int emptyNsID,bool printXsiType) const;
    void free();

    std::string digest(XmlObject *parent);

    /**
     * This is called from serialize when object is Document type.
     */
    void serializeDocument(std::ostream &o,XmlOptionsPtr options,const XmlObject *obj) const;

    /**
     * Takes string value of given attribute
     */
    StringPtr getAttr(std::string attrName) const;
    
    /**
     * Takes attribute as XmlObject
     */
    XmlObjectPtr getAttrObject(const std::string &attrName, XmlObject *parent) const;
    
    XmlObjectPtr getAttrObject2(const std::string &attrName, const std::string &attrValue, XmlObject *parent) const;

    /**
     * NULL usuwa atrybut
     */
    void setAttr(std::string elemName,StringPtr value);

    /**
     * retrieves element of specified name (NULL if not set)
     */
    XmlObjectPtr getElem(std::string elemName) const;

    /**
     * retrieves element of specified name at specified position.
     * If position is out of bounds, it returns NULL.
     */
    XmlObjectPtr getElemAt(std::string elemName,int index) const;

    /** gets element (creates one if there is NULL) */
    XmlObjectPtr cgetElemAt(std::string elemName,int index,ObjectCreatorFn createFn,XmlObject *creator);
    
    /**
     * NULL removes element
     */
    void setElem(std::string elemName,XmlObjectPtr value);

    /**
     * NULL removes element
     */
    void setElemAt(std::string elemName,int index,XmlObjectPtr value);

    /** Removes element at specified index (cutting it off) */
    void removeElemAt(std::string elemName,int index);

    /** Removes all elements of given name */
    void removeElems(std::string name);

    /** Counts number of elements of given name */
    int countElems(std::string name) const;

    shared_array<XmlObjectPtr > getElemArray(std::string elemName) const;
    void setElemArray(std::string elemName,const shared_array<XmlObjectPtr > &value);

private:
    void serializeAttrs(std::ostream &o) const;
    void serializeElems(int emptyNsID,std::ostream &o,const std::map<std::string,SchemaPropertyPtr> *order) const;
public:
    void serialize2(int emptyNsID,bool printXsiType,std::string elemName,std::ostream &o,const XmlObject *obj) const;
    
    /** Adds default elements or attributes to this contents if they don't exist yet. */
    void insertDefaults(const SchemaType *schemaType);
};

}


#endif //XMLBEANSPP_CONTENTS_H_

