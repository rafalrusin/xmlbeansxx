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

LOGGER_PTR_SET(QueryExpr::log,"xmlbeansxx.QueryExpr");
QueryExpr::QueryExpr(QueryNodePtr a,int oper,QueryNodePtr b):a(a),b(b),oper(oper) {}

LOGGER_PTR_SET(QueryNode::log,"xmlbeansxx.QueryNode");

LOGGER_PTR_SET(QueryString::log,"xmlbeansxx.QueryString");
QueryString::QueryString(const std::string& str):str(str) {}

LOGGER_PTR_SET(QueryAttribute::log,"xmlbeansxx.QueryAttribute");
QueryAttribute::QueryAttribute(const std::string& attrName): attrName(attrName) {}

LOGGER_PTR_SET(QueryElement::log,"xmlbeansxx.QueryElement");
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
        FOREACH(ita,aval) {
            FOREACH(itb,bval) {
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
		VAL(array,Contents::Walker::getAttrObjects(o));
		FOREACH(i,array){
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
	if(c){
		VAL(array,Contents::Walker::getElems(o));
		FOREACH(i,array){
			QName name = i->first;
			if(name->second==elemName)
				retu.push_back(i->second);
		}
	} 		
	return retu;

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
    FOREACH(it,v) {
        if (*it) {
            if (next==NULL) {
                r.push_back(Contents::Walker::OrginalXmlObject(*it)->getCanonicalContent());
            } else {
                std::vector<std::string> v2=next->getValue(XmlObject(*it));
                FOREACH(it2,v2) {
                    r.push_back(*it2);
                }
            }
        }
    }
    return r;
}

}
