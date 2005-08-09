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


#ifndef _XMLBEANSXX_QUERY_H_
#define _XMLBEANSXX_QUERY_H_

#include "BoostAssert.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <log4cxx/logger.h>
#include "macros.h"
#include "Dict.h"

/*
 * This file contains classes used for xpath evaluation. 
 * Implementation is not completed. 
 * TODO: Implementation of xpath queries.
 */

	
namespace xmlbeansxx {
class XmlObject;
DECLARE_PTR(XmlObject,XmlObjectPtr,constXmlObjectPtr)
class Operator;
DECLARE_PTR(Operator,OperatorPtr,constOperatorPtr)
class QueryNode;
DECLARE_PTR(QueryNode,QueryNodePtr,constQueryNodePtr)
class QueryExpr;
DECLARE_PTR(QueryExpr,QueryExprPtr,constQueryExprPtr)
class QueryString;
DECLARE_PTR(QueryString,QueryStringPtr,constQueryStringPtr)
class QueryAttribute;
DECLARE_PTR(QueryAttribute,QueryAttributePtr,constQueryAttributePtr)
class QueryElement;
DECLARE_PTR(QueryElement,QueryElementPtr,constQueryElementPtr)

DECLARE_PTR(std::string,StringPtr,constStringPtr)

class Operator {
    public:
    enum {
        AND,OR,EQ,NOT,
        LE,GE,LT,GT
    };
};


class QueryNode {
    static log4cxx::LoggerPtr log;
    public:
    virtual bool getBooleanValue(const XmlObjectPtr &object);
    virtual std::vector<std::string> getValue(const XmlObjectPtr &object);
    virtual ~QueryNode();
};

class QueryExpr: public QueryNode {
    private:
    static log4cxx::LoggerPtr log;
    QueryNodePtr a,b; 
    int oper;
    public:
    QueryExpr(const QueryNodePtr &a,int oper,const QueryNodePtr &b);
    virtual bool getBooleanValue(const XmlObjectPtr &object);
    virtual std::vector<std::string> getValue(const XmlObjectPtr &object);
};

class QueryString: public QueryNode {
    private:
    static log4cxx::LoggerPtr log;
    std::string str;
    
    public:
    QueryString(std::string str);
    virtual std::vector<std::string> getValue(const XmlObjectPtr &object);
};

class QueryAttribute: public QueryNode {
    private:
    static log4cxx::LoggerPtr log;
    DictNameType attrName;
    public:
    QueryAttribute(const DictNameType &attrName);
    virtual std::vector<std::string> getValue(const XmlObjectPtr &object);
};

class QueryElement: public QueryNode {
    private:
    static log4cxx::LoggerPtr log;
    DictNameType elemName;
    QueryNodePtr next;
    public:
    QueryElement(const DictNameType &elemName,const QueryNodePtr &next);
    virtual std::vector<std::string> getValue(const XmlObjectPtr &object);
};

}

#endif//_XMLBEANSXX_QUERY_H_
