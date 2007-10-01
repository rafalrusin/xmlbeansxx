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


#include "XercesParser.h"
#include "defs.h"
#include "TypeSystem.h"
#include "XmlException.h"
#include "XmlTypesGen.h"
#include "Tracer.h"
#include "XercesUtils.h"
#include "logger.h"

#include "BoostAssert.h"
#include "XmlBeans.h"
#include <boost/scoped_array.hpp>

#include <boost/config.hpp>
#include <set>

#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

namespace xmlbeansxx {
using namespace std;
//------------------------------------------------------------------------------------
//Xml Parser

#define ENCODING "utf8"
#define TRACER2(a,b) 
#define LOG4CXX_DEBUG2(a,b) LOG4CXX_DEBUG(a,b)


log4cxx::LoggerPtr XercesParser::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.XercesParser"));
log4cxx::LoggerPtr MySAX2Handler::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.MySAX2Handler"));

XERCES_CPP_NAMESPACE_USE


MySAX2Handler::MySAX2Handler(XercesParser *p): parser(p) {}

void MySAX2Handler::startElement(	const XMLCh* const uri, 
					const XMLCh* const localname, 
					const XMLCh* const qname, 
					const Attributes& attrs)
{
    {
        // LOGS
        LOG4CXX_DEBUG2(log, "SAX2 - StartElement info")
        LOG4CXX_DEBUG2(log, string("localname: ") + transcode(localname))
        LOG4CXX_DEBUG2(log, string("URI      : ") + transcode(uri))
        LOG4CXX_DEBUG2(log, string("qname    : ") + transcode(qname))
        
        
        LOG4CXX_DEBUG2(log, "attributes...");
        for(int i=0; i<attrs.getLength(); i++) {
            LOG4CXX_DEBUG2(log, string("localname: ") + transcode(attrs.getLocalName(i)));
            LOG4CXX_DEBUG2(log, string("URI      : ") + transcode(attrs.getURI(i)));
            LOG4CXX_DEBUG2(log, string("type     : ") + transcode(attrs.getType(i)));
            LOG4CXX_DEBUG2(log, string("value    : ") + transcode(attrs.getValue(i)));
        }
        LOG4CXX_DEBUG2(log, "SAX2 - end info")
    }

    parser->xmlContext.remember();
    
    QName name(transcode(uri), transcode(localname));
    LOG4CXX_DEBUG2(log, "begin element: " + name)
    
    if(parser->nodesStack.empty())
	throw XmlException(string("no XmlObject on XercesParser stack"));


    XmlObjectPtr top=parser->nodesStack.top().obj;
    XmlObjectPtr n;
    if(!top) throw XmlException(string("no XmlObjectPtr on XercesParser stack"));					   

    if(parser->nodesStack.size()==1){
	//this is the ROOT element
	XmlObjectPtr root = globalTypeSystem()->createDocumentByName(name);
	if (root) {
	    root->createContents();
	    top->setXmlObject(*root);
	}
	
    }
	
	        
    {
	// test for xsi:type or create default XmlObject
        for (int i = 0; i < attrs.getLength(); i++) {
	    QName name(transcode(attrs.getURI(i)), transcode(attrs.getLocalName(i)));
	    if(name == XmlBeans::xsi_type()) {
		QName value=parser->nsSplit(transcode(attrs.getValue(i)));
        	LOG4CXX_DEBUG2(log, "xsi:type = " + value)
		n = globalTypeSystem()->createByName(value);
		if (!n) throw XmlException(string("Xsd Type '")+value+string("' not defined in builtin type system"));					   
	    } else if(name == XmlBeans::xsi_array()) {
		QName value=parser->nsSplit(transcode(attrs.getValue(i)));
        	LOG4CXX_DEBUG2(log, "xsi:array = " + value)
		n = globalTypeSystem()->createArrayByName(value);
		if (!n) throw XmlException(string("Xsd Type '")+value+string("' not defined in builtin type system"));					   
	    
	    }
	}
	
	if(!n) n=top->getSchemaType()->createSubObject(name);
	if(!n) throw XmlException(string("Cannot create subelement '")+name+string("' on object of class "+top->getSchemaType()->className));



    }	

    n->createContents();
    

    {
        LOG4CXX_DEBUG2(log, "add attributes")
        for (int i = 0;i < attrs.getLength(); i++) {
	    QName name(transcode(attrs.getURI(i)), transcode(attrs.getLocalName(i)));
    	    LOG4CXX_DEBUG2(log, "attribute name: "+ name)
	    
	    if (name == XmlBeans::xsi_type()) continue;
	    if (name == XmlBeans::xsi_array()) continue;
	    std::string value(transcode(attrs.getValue(i)));
            xmlbeansxx::Contents::Walker::setAttr(*n,name, value);
        }
    }
    
    {
        LOG4CXX_DEBUG2(log, "append element name:" + name  )

	Contents::Walker::appendElem(*(parser->nodesStack.top().obj),name,n->contents);
        parser->nodesStack.push(EmptyParser::StackEl(n,n->getSchemaType()->processContents,name));
	

    }

}
void MySAX2Handler::startPrefixMapping(const XMLCh* const _prefix, const XMLCh* const _uri) {
    	//NAMESPACES
	std::string prefix = transcode(_prefix);
	std::string uri = transcode(_uri);
	LOG4CXX_DEBUG2(log, "store namespace in xmlContext:" + prefix + " = " + uri)
        parser->xmlContext.setLink(prefix, uri); 
}



void MySAX2Handler::characters(const XMLCh* const chars, const unsigned int length) {
        string s = transcode(chars, length);
    	parser->nodesStack.top().str+=s;
}

void MySAX2Handler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
 {
    LOG4CXX_DEBUG2(log, "end element " << transcode(qname) )
    EmptyParser::StackEl e=parser->nodesStack.top();
    XmlObjectPtr n=e.obj;
    n->setSimpleContent(e.str);
    parser->xmlContext.restore();
    parser->nodesStack.pop();
}


//---
string MySAX2Handler::getErrorStr(const SAXParseException& e) {
    ostringstream ss;
    ss << "At line " << e.getLineNumber()
    << ", char " << e.getColumnNumber()
    << ", " << transcode(e.getMessage());
    if(e.getSystemId()) ss << ", " << transcode(e.getSystemId());
    if(e.getPublicId()) ss << ", " << transcode(e.getPublicId());
    return ss.str();
}

void MySAX2Handler::warning(const SAXParseException& exc) {
    LOG4CXX_WARN(log,std::string("MySAX2Handler::warning() - Xerces-c warning: ")+getErrorStr(exc));
//    throw XmlParseException(string("Xerces-c warning: ")+getErrorStr(exc));
}
void MySAX2Handler::error(const SAXParseException& exc) {
    throw XmlParseException(string("Xerces-c error: ")+getErrorStr(exc));
}
void MySAX2Handler::fatalError(const SAXParseException& exc) {
    throw XmlParseException(string("Xerces-c fatal error: ")+getErrorStr(exc));
}

std::string MySAX2Handler::transcode(const XMLCh* const chars,const int length)
{
	char* message = XMLString::transcode(chars);
	std::string retu(message);
	XMLString::release(&message);
	return retu;
}


//----------------------

XercesParser::XercesParser(){
    init();
}

XercesParser::XercesParser(const XmlOptions &opts):EmptyParser(opts) {
    init();
}



void XercesParser::init(bool reinit) {
    LOG4CXX_DEBUG(log,"start");
    static bool initialized = false;
    
    if(!initialized) {
    	LOG4CXX_DEBUG(log,"initializing xerces-c");
	XMLPlatformUtils::Initialize(); 
	initialized=true;
    }

    sax2=auto_ptr<SAX2XMLReader>(XMLReaderFactory::createXMLReader());
    handler=auto_ptr<MySAX2Handler>(new MySAX2Handler(this));
    sax2->setContentHandler(handler.get());
    sax2->setErrorHandler(handler.get());

    sax2->setFeature(XMLUni::fgSAX2CoreNameSpaces, true); 
    sax2->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);

    LOG4CXX_DEBUG(log,"finish");
}

XercesParser::~XercesParser() {}

void XercesParser::updateOptions() {
	sax2->setFeature(XMLUni::fgSAX2CoreValidation, options.getValidation());
	sax2->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);
//	sax2->setFeature(XMLUni::fgXercesDynamic, true);
}

struct null_deleter2
{
	template<class T> void operator()(T *) const{}
};

void XercesParser::parse(const std::string &str, XmlObject &documentRoot){
	std::istringstream in(str);
	parse(in,documentRoot);
}


void XercesParser::parse(istream &in,XmlObject &documentRoot) {
    TRACER(log,"parse");
    LOG4CXX_INFO(log,std::string("XmlParser::parse() - start"));

    updateOptions();

    while (!nodesStack.empty())
	nodesStack.pop();

    documentRoot.createContents();
    XmlObjectPtr root_ptr=boost::shared_ptr<XmlObject>(&documentRoot,null_deleter2());
    
    nodesStack.push(StackEl(root_ptr,documentRoot.getSchemaType()->processContents,QName()));


    StdInputSource s(in);
    LOG4CXX_INFO(log,"parsing:");
    sax2->parse(s);
    nodesStack.pop();
    BOOST_ASSERT(nodesStack.empty());
    LOG4CXX_DEBUG(log,std::string("XmlParser::parse() - finish"));
}




void XercesParser::loadGrammars(const std::vector<std::string> &fileNames) {
    XMLBEANSXX_FOREACH(std::vector<std::string>::const_iterator,it,fileNames) {
        loadGrammar(*it);
    }
}

void XercesParser::loadGrammar(const std::string &fileName) {
    LOG4CXX_DEBUG(log,std::string("loadGrammar: "+ fileName));
    sax2->loadGrammar(fileName.c_str(),Grammar::SchemaGrammarType,true);
}
void XercesParser::unloadGrammars() {
    sax2->resetCachedGrammarPool();
}



}

