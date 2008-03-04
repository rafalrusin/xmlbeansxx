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


#ifndef _XMLBEANSXX_CONTENTS_IMPL_H_
#define _XMLBEANSXX_CONTENTS_IMPL_H_

#include "Contents.h"


namespace xmlbeansxx {


class StringContents : public Contents {
private:
	std::string value;	
public:
	StringContents(const SchemaType *st):Contents(st){}
	StringContents(const std::string & str) ; //:Contents(xmlbeansxx::XmlString::type()),value(str){}
	StringContents(const SchemaType *st,const std::string &str):Contents(st),value(str){}
	virtual ~StringContents() {}

	virtual void setSimpleContent(const std::string &c) { value = c; };
	virtual std::string getSimpleContent() const { return value; };
	virtual std::string getCanonicalContent() const { return value; };


	virtual ContentsPtr getAttr(const QName& attrName) const { return ContentsPtr(); };
	virtual void setAttr(const QName& attrName,ContentsPtr value) { throw IllegalStateException("setAttr on StringContents"); };


	virtual ContentsPtr getElem(const QName& elemName,int index=0) const { return ContentsPtr(); };
	virtual void setElem(const QName& elemName,ContentsPtr value,int index=0) { throw IllegalStateException("cgetElem on StringContents"); };
	virtual bool isSetElem(const QName& elemName,int index=0) { return false; };
	virtual void removeElems(const QName& elemName) {};
	virtual void removeElemAt(const QName& elemName,int index) {};

	virtual int countElems(const QName& elemName) const { return 0; }
	virtual bool hasElements() const { return false; }
	
	virtual std::vector<ContentsPtr> getElemArray(const QName& elemName) const {return std::vector<ContentsPtr>(); };


    	/** returns true while this contents don't have any attributes and elements */
	virtual bool hasEmptyContent() const { return true; };

	virtual std::vector<std::pair<QName,ContentsPtr> > getElems(bool ) const { std::vector<std::pair<QName,ContentsPtr> >(); }
	virtual std::vector<std::pair<QName,std::string> > getAttrs() const { std::vector<std::pair<QName,std::string> > (); } 
	virtual std::vector<std::pair<QName,ContentsPtr> > getAttrs2() const { std::vector<std::pair<QName,ContentsPtr> >(); }

	virtual ContentsPtr clone(){  return ContentsPtr(new StringContents(st,value)); }
        virtual void free() { value.clear(); };

	virtual void serializeDocument(std::ostream &o,XmlOptions options) const { throw IllegalStateException("serializeDocument on StringContents"); }
	virtual void serialize(bool printXsiType,const QName &elemName,std::ostream &o,NSMapSerializer &ns,XmlOptions options) const { o << TextUtils::exchangeEntities(value); }


private:
	virtual void serializeAttrs(std::ostream &o,NSMapSerializer& ns,XmlOptions options) const { }
	virtual void serializeElems(std::ostream &o,NSMapSerializer& ns,XmlOptions options) const { }


};


} //namespace xmlbenasxx

	

#endif //XMLBEANSPP_CONTENTS_IMPL_H_

