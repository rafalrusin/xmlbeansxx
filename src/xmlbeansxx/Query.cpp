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


//Turn on tracing
//#define TRACER2 TRACER

//Turn off tracing
#define TRACER2(log,msg) 

namespace xmlbeansxx {

log4cxx::LoggerPtr QueryExpr::log = log4cxx::Logger::getLogger("xmlbeansxx.QueryExpr");
QueryExpr::QueryExpr(const QueryNodePtr &a,int oper,const QueryNodePtr &b):a(a),b(b),oper(oper) {}

log4cxx::LoggerPtr QueryNode::log = log4cxx::Logger::getLogger("xmlbeansxx.QueryNode");

log4cxx::LoggerPtr QueryString::log = log4cxx::Logger::getLogger("xmlbeansxx.QueryString");
QueryString::QueryString(std::string str):str(str) {}

log4cxx::LoggerPtr QueryAttribute::log = log4cxx::Logger::getLogger("xmlbeansxx.QueryAttribute");
QueryAttribute::QueryAttribute(const DictNameType &attrName): attrName(attrName) {}

log4cxx::LoggerPtr QueryElement::log = log4cxx::Logger::getLogger("xmlbeansxx.QueryElement");
QueryElement::QueryElement(const DictNameType &elemName,const QueryNodePtr &next): elemName(elemName),next(next) {}

QueryNode::~QueryNode() {}

namespace {
    std::vector<std::string> singleString(const std::string &s) {
        std::vector<std::string> v;
        v.push_back(s);
        return v;
    }
}

bool QueryNode::getBooleanValue(const XmlObjectPtr &object) {
    TRACER2(log,"getBooleanValue")
    return getValue(object).size()>0;
}

std::vector<std::string> QueryNode::getValue(const XmlObjectPtr &object) {
    TRACER2(log,"getValue")
    //Not possible
    BOOST_ASSERT(false);
    return std::vector<std::string>();
}

bool QueryExpr::getBooleanValue(const XmlObjectPtr &object) {
    TRACER2(log,"getBooleanValue")
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

std::vector<std::string> QueryExpr::getValue(const XmlObjectPtr &object) {
    TRACER2(log,"getValue")
    return singleString(getBooleanValue(object)?"true":"false");
}


std::vector<std::string> QueryString::getValue(const XmlObjectPtr &object) {
    TRACER2(log,"getValue")
    return singleString(str);
}

std::vector<std::string> QueryAttribute::getValue(const XmlObjectPtr &object) {
    TRACER2(log,"getValue")
    XmlObjectPtr a=object->getContents()->getAttrObject(attrName,object.get());

    std::vector<std::string> r;
    if (a!=NULL) {
        r.push_back(a->getCanonicalContent());
    }
    return r;
}

std::vector<std::string> QueryElement::getValue(const XmlObjectPtr &object) {
    std::vector<std::string> r;
    shared_array<XmlObjectPtr> v(object->getContents()->getElemArray(elemName));
    FOREACH(it,v) {
        if (*it!=NULL) {
            if (next==NULL) {
                r.push_back((*it)->getCanonicalContent());
            } else {
                std::vector<std::string> v2=next->getValue(*it);
                FOREACH(it2,v2) {
                    r.push_back(*it2);
                }
            }
        }
    }
    return r;
}

}
