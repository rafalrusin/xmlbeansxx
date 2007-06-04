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


#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Dict.h"
#include "XmlOptions.h"
#include "SchemaType.h"
#include "XmlTypesGen.h"

#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/recursive_mutex.hpp>
#endif

#include <XmlTypesGen.h>

#define FOREACH(it,c) for(__typeof((c).begin()) it=(c).begin(); it!=(c).end(); ++it)


class xmlbeansxx::Contents;

using namespace xmlbeansxx;

namespace xmlbeansxx {


		ContentsPtr Contents::Walker::getAttr(const XmlObject& obj,const QName & x){
			if(!obj.hasContents()) return ContentsPtr();
			return obj.contents->getAttr(x);
		};
		 std::string Contents::Walker::getAttrString(const XmlObject& obj,const QName & x){
			if(!obj.hasContents()) return std::string();
			return getSimpleContent(XmlObject(obj.contents->getAttr(x)));
		};
		 void Contents::Walker::setAttr(XmlObject& obj,const QName &x,const ContentsPtr& y){
			obj.createContents();
			return obj.contents->setAttr(x,y);
		};
		 void Contents::Walker::setAttr(XmlObject& obj,const QName &x,const std::string& y){
			obj.createContents();
			XmlString v(y);
			return obj.contents->setAttr(x,v.contents);
		};
		
		ContentsPtr Contents::Walker::getElem(const XmlObject& obj,const QName& x,int index){
			if(!obj.hasContents()) return ContentsPtr();
			return obj.contents->getElem(x,index);
		};
		ContentsPtr Contents::Walker::cgetElem(XmlObject& obj,const QName& x,int index){
			obj.createContents();
			ContentsPtr n=obj.contents->getElem(x,index);
			if(!n) {
				XmlObjectPtr o=obj.contents->st->createSubObject(x);
				o->createContents();
				n=o->contents;
				obj.contents->setElem(x,n,index);
			}
			return n;
		};
		 void Contents::Walker::setElem(XmlObject& obj,const QName& x,const ContentsPtr& y,int index){
			obj.createContents();
			obj.contents->setElem(x,y,index);
		};
		 void Contents::Walker::appendElem(XmlObject& obj,const QName& x,const ContentsPtr& y){
			obj.createContents();			
			obj.contents->setElem(x,y,countElems(obj,x));
		};
		 bool Contents::Walker::isSetElem(const XmlObject& obj,const QName& x,int index){
			if(!obj.hasContents()) return false;
			return obj.contents->isSetElem(x,index);
		};
		 void Contents::Walker::removeElems(XmlObject& obj,const QName& x){
			if(!obj.hasContents()) return;
			return obj.contents->removeElems(x);
		};
		 void Contents::Walker::removeElemAt(XmlObject& obj,const QName& x,int index){
			if(!obj.hasContents()) return;
			return obj.contents->removeElemAt(x,index);
		};
		
		 int Contents::Walker::countElems(const XmlObject& obj,const QName& x){
			if(!obj.hasContents()) return 0;
			return obj.contents->countElems(x);
		};
		 bool Contents::Walker::hasElements(const XmlObject& obj){
			if(!obj.hasContents()) return false;
			return obj.contents->hasElements();
		};
		
/*		 ContentsPtr create(){
			return  ContentsPtr(new Contents);
		}
*/
		 std::vector<std::pair<QName,ContentsPtr> > Contents::Walker::getElems(const XmlObject& obj) {
			if(!obj.hasContents()) return std::vector<std::pair<QName,ContentsPtr> >();
			return obj.contents->getElems();
		}
		 std::vector<std::pair<QName,std::string> > Contents::Walker::getAttrs(const XmlObject& obj) {
			if(!obj.hasContents()) return std::vector<std::pair<QName,std::string> >();
			return obj.contents->getAttrs();
		}
		
		 std::vector<ContentsPtr> Contents::Walker::getElemArray(const XmlObject& obj,const QName& elemName) {
			if(!obj.hasContents()) return std::vector<ContentsPtr>();
			return obj.contents->getElemArray(elemName);		
		};

		 void Contents::Walker::setElemArray(XmlObject& obj,const QName& elemName,const std::vector<ContentsPtr>& v) {
			removeElems(obj,elemName);
			FOREACH(i,v) appendElem(obj,elemName,(*i));
		};

/*		 void insertDefaults(ContentsPtr &p,const SchemaType *schemaType){
			if(!p) p = ContentsPtr(new Contents);
			p->insertDefaults(schemaType);
		};
*/
		 void Contents::Walker::serializeDocument(XmlObject& obj,std::ostream &o,XmlOptions options,const xmlbeansxx::SchemaType * st) {
			obj.createContents();
			obj.contents->serializeDocument(o,options);
		};
/*		 void Contents::Walker::serialize2(XmlObject& obj,int emptyNsID,bool printXsiType,const QName& elemName,std::ostream &o,const xmlbeansxx::SchemaType * st) {
			obj.createContents();
			obj.contents->serialize2(emptyNsID,printXsiType,elemName,o,st);
		}
		
*/		 void Contents::Walker::setSimpleContent(XmlObject& obj,const std::string &c) {
			obj.createContents();
			obj.contents->setSimpleContent(c);
		}
		 std::string Contents::Walker::getSimpleContent(const XmlObject& obj) {
			if(!obj.hasContents()) return std::string();
			return obj.contents->getSimpleContent();
		}

		    /** returns true while this contents don't have any attributes and elements */
		 bool Contents::Walker::hasEmptyContent(XmlObject& obj) {
			if(!obj.hasContents()) return true;
			return obj.contents->hasEmptyContent();
		}
		
		std::string Contents::Walker::dump(const ContentsPtr& p){
		    if(!p) return " NULL ";
		    std::string s(" { ");
		    FOREACH(i,p->elems.contents){
//			s+= i->name +" (" + i->name.first.buf ","+ i->name.second.buf+ ") = " + dump(i->value); 
			s+= i->name +" = " + dump(i->value); 
		    };
		    FOREACH(i,p->attrs.contents){
//			s+="@" + i->name +" (" + i->name.first.buf ","+ i->name.second.buf+ ") = " + dump(i->value); 
			s+= "@" +i->name +" = " + dump(i->value); 
		    };
		    s+= p->value;
		    s+= " } ";
		    return s;
		}
		
		std::string Contents::Walker::digest(const XmlObject& obj){
			if(!obj.contents) return std::string("NULL");
			return obj.contents->digest();
		}
		
		XmlObjectPtr Contents::Walker::OrginalXmlObject(const ContentsPtr &p){
			if(!p) return XmlObjectPtr(new XmlObject);
			if(p->st->createFn==NULL) XmlObjectPtr(new XmlObject(p));
			XmlObjectPtr retu = p->st->createFn();
			retu->swapContents(p);
			return retu;
		}
				
};




