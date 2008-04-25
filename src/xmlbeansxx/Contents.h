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
#include "XmlContext.h"

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#endif



class xmlbeansxx::Contents;


namespace xmlbeansxx {

class SchemaType;
class XmlObject;
class Contents;

typedef boost::shared_ptr<Contents> ContentsPtr;
typedef boost::shared_ptr<const Contents> constContentsPtr;


class Contents : public boost::enable_shared_from_this<Contents> {
private:
	XMLBEANSXX_STATIC_LOGGER_PTR(log);
public:

    	ElemDict attrs;
    	ElemDict elems;
	const xmlbeansxx::SchemaType * st;
	
#ifdef BOOST_HAS_THREADS
	mutable boost::recursive_mutex mutex;
#endif

	Contents(const SchemaType *st):st(st){}
	virtual ~Contents();

	virtual void setSimpleContent(const std::string &c);
	virtual std::string getSimpleContent() const;
	virtual std::string getCanonicalContent() const;


	virtual ContentsPtr getAttr(const QName& attrName) const;
	virtual void setAttr(const QName& attrName,ContentsPtr value);


	virtual ContentsPtr getElem(const QName& elemName,int index=0) const;
	virtual void setElem(const QName& elemName,ContentsPtr value,int index=0);
	virtual bool isSetElem(const QName& elemName,int index=0);
	virtual void removeElems(const QName& elemName);
	virtual void removeElemAt(const QName& elemName,int index);

	virtual int countElems(const QName& elemName) const;
	virtual bool hasElements() const;
	
	virtual std::vector<ContentsPtr> getElemArray(const QName& elemName) const;


    	/** returns true while this contents don't have any attributes and elements */
	virtual bool hasEmptyContent() const;

	virtual std::vector<std::pair<QName,ContentsPtr> > getElems(bool all=false) const;
	virtual std::vector<std::pair<QName,std::string> > getAttrs() const;
	virtual std::vector<std::pair<QName,ContentsPtr> > getAttrs2() const;

	virtual ContentsPtr clone();
        virtual void free();


	virtual void serializeDocument(std::ostream &o,XmlOptions options) const;

	virtual std::string digest() const;
	virtual const SchemaType * getSchemaType() const;

private:
	virtual void serialize(bool printXsiType,const QName &elemName,std::ostream &o,NSMapSerializer &ns,XmlOptions options) const;
	virtual void serializeAttrs(std::ostream &o,NSMapSerializer& ns,XmlOptions options) const;
	virtual void serializeElems(std::ostream &o,NSMapSerializer& ns,XmlOptions options) const;





public:
	class Walker{
	public:
		typedef std::vector<std::pair<QName,ContentsPtr> > AttrObjectsType;
		typedef std::vector<std::pair<QName,std::string> > AttrsType;
		typedef std::vector<std::pair<QName,ContentsPtr> > ElemsType;
		typedef std::vector<ContentsPtr> ContentsPtrArrayType;
		
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
		
		static ElemsType getElems(const XmlObject& obj);
		static AttrsType getAttrs(const XmlObject& obj);		
		static AttrObjectsType getAttrObjects(const XmlObject& obj);		
		static ContentsPtrArrayType getElemArray(const XmlObject& obj,const QName& elemName);
//		static std::vector<XmlObject> getElemArray(const XmlObject& obj,const QName& elemName);
		template <typename T>
		static void setElemArray(XmlObject& obj,const QName& elemName,const std::vector<T>& v) {
			removeElems(obj,elemName);
			for(typename std::vector<T>::const_iterator i=v.begin();i!=v.end();++i)
				appendElem(obj,elemName,i->contents);
		};
		static void setElemArray(XmlObject& obj,const QName& elemName,const ContentsPtrArrayType& v);

		static void serializeDocument(const XmlObject& obj,std::ostream &o,XmlOptions options,const xmlbeansxx::SchemaType * st);

		static void setSimpleContent(XmlObject& obj,const std::string &c);
		static std::string getSimpleContent(const XmlObject& obj);

		    /** returns true while this contents don't have any attributes and elements */
		static bool hasEmptyContent(XmlObject& obj);
		static std::string dump(const ContentsPtr& p);
		
		static XmlObjectPtr OrginalXmlObject(const ContentsPtr &p);
		static std::string digest(const XmlObject& obj);
	};
};


} //namespace xmlbenasxx

	

#endif //XMLBEANSPP_CONTENTS_H_

