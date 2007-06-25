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
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Dict.h"
#include "XmlOptions.h"
#include "SchemaType.h"
#include "QName.h"

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#endif

#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin(); it!=(c).end(); ++it)


class xmlbeansxx::Contents;

/** intrusive_ptr
namespace boost
{
    void intrusive_ptr_add_ref(xmlbeansxx::Contents * p);
    void intrusive_ptr_release(xmlbeansxx::Contents * p);
};
*/

namespace xmlbeansxx {

class SchemaType;
class XmlObject;
class NSMapSerializer;

class Contents;
typedef boost::shared_ptr<Contents> ContentsPtr;
typedef boost::shared_ptr<const Contents> constContentsPtr;
typedef boost::shared_ptr<std::string> StringPtr;
//typedef boost::intrusive_ptr<Contents> ContentsPtr;
//typedef Contents * ContentsPtr;



class Contents : public boost::enable_shared_from_this<Contents> {
private:
	STATIC_LOGGER_PTR(log);
public:

    	ElemDict attrs;
    	ElemDict elems;
    	std::string value;
	const xmlbeansxx::SchemaType * st;
	
#ifdef BOOST_HAS_THREADS
	mutable boost::recursive_mutex mutex;
#endif

	Contents(const SchemaType *st):st(st){}
	virtual ~Contents();

	void setSimpleContent(const std::string &c);
	std::string getSimpleContent() const;
	std::string getCanonicalContent() const;


	ContentsPtr getAttr(const QName& attrName) const;
	void setAttr(const QName& attrName,ContentsPtr value);


	ContentsPtr getElem(const QName& elemName,int index=0) const;
	ContentsPtr cgetElem(const QName& elemName,int index=0) const;
	void setElem(const QName& elemName,ContentsPtr value,int index=0);
	bool isSetElem(const QName& elemName,int index=0);
	void removeElems(const QName& elemName);
	void removeElemAt(const QName& elemName,int index);

	int countElems(const QName& elemName) const;
	bool hasElements() const;
	
	std::vector<ContentsPtr> getElemArray(const QName& elemName) const;


    	/** returns true while this contents don't have any attributes and elements */
	bool hasEmptyContent() const;

	std::vector<std::pair<QName,ContentsPtr> > getElems() const;
	std::vector<std::pair<QName,std::string> > getAttrs() const;
	std::vector<std::pair<QName,ContentsPtr> > getAttrs2() const;

	/** Copies all contents from original object */
//	void copyFrom(const ContentsPtr &orig);
	ContentsPtr clone();
        void free();


//	void insertDefaults(const SchemaType *schemaType);

//	void serialize2(int emptyNsID,bool printXsiType,const QName& elemName,std::ostream &o,const xmlbeansxx::SchemaType * st) const;
	void serializeDocument(std::ostream &o,XmlOptions options) const;
	void serialize(bool printXsiType,const QName &elemName,std::ostream &o,NSMapSerializer ns) const;

	std::string digest() const;
	const SchemaType * getSchemaType() const;

private:
	void serializeAttrs(std::ostream &o,NSMapSerializer& ns) const;
	void serializeElems(std::ostream &o,NSMapSerializer ns) const;





public:
	struct Walker{

		static ContentsPtr getAttr(const XmlObject& obj,const QName& x);
		static std::string getAttrString(const XmlObject& obj,const QName& x);
		static void setAttr(XmlObject& obj,const QName &x,const ContentsPtr& y);
		static void setAttr(XmlObject& obj,const QName &x,const std::string& y);		
		static ContentsPtr getElem(const XmlObject& obj,const QName& x,int index = 0);
		static ContentsPtr cgetElem(XmlObject& obj,const QName& x,int index = 0);
		static void setElem(XmlObject& obj,const QName& x,const ContentsPtr& y,int index = 0);
		static void appendElem(XmlObject& obj,const QName& x,const ContentsPtr& y);
		static bool isSetElem(const XmlObject& obj,const QName& x,int index = 0);
		static void removeElems(XmlObject& obj,const QName& x);
		static void removeElemAt(XmlObject& obj,const QName& x,int index);
		static int countElems(const XmlObject& obj,const QName& x);
		static bool hasElements(const XmlObject& obj);
		
		static std::vector<std::pair<QName,ContentsPtr> > getElems(const XmlObject& obj);
		static std::vector<std::pair<QName,std::string> > getAttrs(const XmlObject& obj);		
		static std::vector<ContentsPtr> getElemArray(const XmlObject& obj,const QName& elemName);
//		static std::vector<XmlObject> getElemArray(const XmlObject& obj,const QName& elemName);
		template <class T>
		static void setElemArray(XmlObject& obj,const QName& elemName,const std::vector<T>& v) {
			removeElems(obj,elemName);
			FOREACH(i,v) appendElem(obj,elemName,i->contents);
		};
		static void setElemArray(XmlObject& obj,const QName& elemName,const std::vector<ContentsPtr>& v);

		static void serializeDocument(XmlObject& obj,std::ostream &o,XmlOptions options,const xmlbeansxx::SchemaType * st);
//		static void serialize2(XmlObject& obj,int emptyNsID,bool printXsiType,const QName& elemName,std::ostream &o,const xmlbeansxx::SchemaType * st);

		static void setSimpleContent(XmlObject& obj,const std::string &c);
		static std::string getSimpleContent(const XmlObject& obj);

		    /** returns true while this contents don't have any attributes and elements */
		static bool hasEmptyContent(XmlObject& obj);
		static std::string dump(const ContentsPtr& p);
		
		static XmlObjectPtr OrginalXmlObject(const ContentsPtr &p);
		static std::string digest(const XmlObject& obj);
	};
};

	
///typedef XmlObject (*ObjectCreatorFn)();

///class SchemaProperty;
///DECLARE_PTR(SchemaProperty,SchemaPropertyPtr,constSchemaPropertyPtr)


/*
struct Contents {

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
*/
///    void appendAttr(const std::string name,const std::string value);
///    void appendElem(const std::string name,XmlObject value);

    /** returns true while this contents don't have any attributes and elements */
///    bool hasEmptyContent() const;

    /** returns names of all sub elements */
///    std::vector<std::pair<std::string,XmlObject> > getElems() const;
///    std::vector<std::pair<std::string,std::string> > getAttrs() const;

    /** Copies only orig contents to this object (not whole tree) */
///    void simpleCopyFrom(const Contents &orig);

    /** Copies all contents from original object */
///    void copyFrom(const Contents &orig);

    /**
     * Chceck whether this object has elements (complex content)
     */
///    bool hasElements() const;

///    void serialize(std::string elemName,std::ostream &o,const XmlObject obj,int emptyNsID,bool printXsiType) const;
///    void free();

///    std::string digest(XmlObject *parent);

    /**
     * This is called from serialize when object is Document type.
     */
///    void serializeDocument(std::ostream &o,XmlOptionsPtr options,const XmlObject obj) const;

    /**
     * Takes string value of given attribute
     */
///    StringPtr getAttr(std::string attrName) const;
    
    /**
     * Takes attribute as XmlObject
     */
///    XmlObject getAttrObject(const std::string &attrName, XmlObject *parent) const;
    
///    XmlObject getAttrObject2(const std::string &attrName, const std::string &attrValue, XmlObject *parent) const;

    /**
     * NULL usuwa atrybut
     */
///    void setAttr(std::string elemName,StringPtr value);

    /**
     * retrieves element of specified name (NULL if not set)
     */
///    ContentsPtr getElem(std::string elemName) const;

    /**
     * retrieves element of specified name at specified position.
     * If position is out of bounds, it returns NULL.
     */
///    XmlObject getElemAt(std::string elemName,int index) const;

    /** gets element (creates one if there is NULL) */
///    XmlObject cgetElemAt(std::string elemName,int index,ObjectCreatorFn createFn,XmlObject *creator);
    
    /**
     * NULL removes element
     */
///    void setElem(std::string elemName,XmlObject value);

    /**
     * NULL removes element
     */
///    void setElemAt(std::string elemName,int index,XmlObject value);

    /** Removes element at specified index (cutting it off) */
///    void removeElemAt(std::string elemName,int index);

    /** Removes all elements of given name */
///    void removeElems(std::string name);

    /** Counts number of elements of given name */
///    int countElems(std::string name) const;
///    shared_array<XmlObject > getElemArray(std::string elemName) const;
///    void setElemArray(std::string elemName,const shared_array<XmlObject > &value);

///private:
///    void serializeAttrs(std::ostream &o) const;
///    void serializeElems(int emptyNsID,std::ostream &o,const std::map<std::string,SchemaPropertyPtr> *order) const;
///public:
    
    /** Adds default elements or attributes to this contents if they don't exist yet. */


///};

}


#endif //XMLBEANSPP_CONTENTS_H_

