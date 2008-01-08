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


#include "Query.h"
#include "XmlTypesGen.h"
#include "Contents.h"
#include "Tracer.h"
#include "defs.h"
#include "BoostAssert.h"



namespace xmlbeansxx {

XMLBEANSXX_LOGGER_PTR_SET(QueryExpr::log,"xmlbeansxx.QueryExpr");
QueryExpr::QueryExpr(QueryNodePtr a,int oper,QueryNodePtr b):a(a),b(b),oper(oper) {}

XMLBEANSXX_LOGGER_PTR_SET(QueryNode::log,"xmlbeansxx.QueryNode");

XMLBEANSXX_LOGGER_PTR_SET(QueryString::log,"xmlbeansxx.QueryString");
QueryString::QueryString(const std::string& str):str(str) {}

XMLBEANSXX_LOGGER_PTR_SET(QueryAttribute::log,"xmlbeansxx.QueryAttribute");
QueryAttribute::QueryAttribute(const std::string& attrName): attrName(attrName) {}

XMLBEANSXX_LOGGER_PTR_SET(QueryElement::log,"xmlbeansxx.QueryElement");
QueryElement::QueryElement(const std::string & elemName,QueryNodePtr next): elemName(elemName),next(next) {}

QueryNode::~QueryNode() {}

namespace {
    std::vector<std::string> singleString(const std::string& s) {
        std::vector<std::string> v;
        v.push_back(s);
        return v;
    }
}

bool QueryNode::getBooleanValue(const XmlObject& object) {
    TRACER(log,"getBooleanValue");
    return getValue(object).size()>0;
}

std::vector<std::string> QueryNode::getValue(const XmlObject& object) {
    TRACER(log,"getValue");
    //Not possible
    BOOST_ASSERT(false);
    return std::vector<std::string>();
}

bool QueryExpr::getBooleanValue(const XmlObject& object) {
    TRACER(log,"getBooleanValue");
    if (oper==Operator::AND) {
        return a->getBooleanValue(object) && b->getBooleanValue(object);
    } else if (oper==Operator::OR) {
        return a->getBooleanValue(object) || b->getBooleanValue(object);
    } else if (oper==Operator::NOT) {
        return ! b->getBooleanValue(object);
    } else {
        std::vector<std::string> aval(a->getValue(object)),bval(b->getValue(object));
        bool res=false;
        XMLBEANSXX_FOREACH(std::vector<std::string>::iterator,ita,aval) {
            XMLBEANSXX_FOREACH(std::vector<std::string>::iterator,itb,bval) {
                if (oper==Operator::EQ) {
                    res|=(*ita)==(*itb);
                } else if (oper==Operator::LE) {
                    res|=(*ita)<=(*itb);
                } else if (oper==Operator::GE) {
                    res|=(*ita)>=(*itb);
                } else if (oper==Operator::LT) {
                    res|=(*ita)<(*itb);
                } else if (oper==Operator::GT) {
                    res|=(*ita)>(*itb);
                } else throw IllegalStateException();
                if (res) return true;
            }
        }
        return false;
    }
}

std::vector<std::string> QueryExpr::getValue(const XmlObject& object) {
    TRACER(log,"getValue");
    return singleString(XmlBoolean(getBooleanValue(object)).getCanonicalContent());
}


std::vector<std::string> QueryString::getValue(const XmlObject& object) {
    TRACER(log,"getValue");
    return singleString(str);
}

static ContentsPtr findAttr(const XmlObject& o,const std::string& attrName)
{
	ContentsPtr c=o.contents;
	if(c){
		Contents::Walker::AttrObjectsType array=Contents::Walker::getAttrObjects(o);
		XMLBEANSXX_FOREACH(Contents::Walker::AttrObjectsType::const_iterator,i,array) {
			QName name = i->first;
			if(name->second==attrName)
				return i->second;
		}
	} 		
	return ContentsPtr();
}

static std::vector<ContentsPtr> findElems(const XmlObject& o,const std::string& elemName)
{
	std::vector<ContentsPtr> retu;
	ContentsPtr c=o.contents;
	if(elemName=="*") {
		Contents::Walker::ElemsType array=Contents::Walker::getElems(o);
		XMLBEANSXX_FOREACH(Contents::Walker::ElemsType::iterator,i,array) {
			retu.push_back(i->second);
		}
	} else if(c){
		Contents::Walker::ElemsType array=Contents::Walker::getElems(o);
		XMLBEANSXX_FOREACH(Contents::Walker::ElemsType::iterator,i,array) {
			QName name = i->first;
			if(name->second==elemName)
				retu.push_back(i->second);
		}
	} 		
	return retu;

}


XmlObject XmlObject::query(const std::string &elementName,QueryNodePtr queryExpr) const {
    TRACER(log,"query");
    std::vector<ContentsPtr> elems=findElems(*this,elementName);
    
    XMLBEANSXX_FOREACH(std::vector<ContentsPtr>::iterator,it,elems) {
        if (*it) {
            if (queryExpr->getBooleanValue(XmlObject(*it))) {
                return XmlObject(*it);
            }
        }
    }
    return XmlObject();
}

XmlObject XmlObject::cquery(const std::string & elementName,QueryNodePtr queryExpr,ObjectCreatorFn createFn) {
    TRACER(log,"cquery");
    
    XmlObject r=query(elementName,queryExpr);
    if (!r.hasContents()) {
	QName correctName=QName("",elementName);
	// find the correct QName
	XMLBEANSXX_FOREACH(SchemaType::ElementsType::const_iterator, i,getSchemaType()->elements) {
	    QName name=i->first;
	    if(name->second==elementName) 
		correctName=name;
	}
    	if(createFn != NULL) {
        	r = createFn();
	} else {
		XmlObjectPtr objP=getSchemaType()->createSubObject(correctName);
		objP->createContents();
		r=*objP;
	}
	
        xmlbeansxx::Contents::Walker::appendElem(*this,correctName,r.contents);
    }
    return r;
}



std::vector<std::string> QueryAttribute::getValue(const XmlObject& object) {
    TRACER(log,"getValue");
    ContentsPtr a=findAttr(object,attrName);

    XmlObjectPtr p=Contents::Walker::OrginalXmlObject(a);
    std::vector<std::string> r;
    r.push_back(p->getCanonicalContent());
    return r;
}

std::vector<std::string> QueryElement::getValue(const XmlObject& object) {
    std::vector<std::string> r;
    std::vector<ContentsPtr> v=findElems(object,elemName);
    XMLBEANSXX_FOREACH(std::vector<ContentsPtr>::iterator,it,v) {
        if (*it) {
            if (next==NULL) {
                r.push_back(Contents::Walker::OrginalXmlObject(*it)->getCanonicalContent());
            } else {
                std::vector<std::string> v2=next->getValue(XmlObject(*it));
                XMLBEANSXX_FOREACH(std::vector<std::string>::iterator, it2, v2) {
                    r.push_back(*it2);
                }
            }
        }
    }
    return r;
}

}
