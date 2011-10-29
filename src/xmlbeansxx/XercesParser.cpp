/*
    Copyright 2004-2008 TouK sp. z o.o. s.k.a.

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
//#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include "ContentsImpl.h"

namespace xmlbeansxx {
using namespace std;
//------------------------------------------------------------------------------------
//Xml Parser

#define ENCODING "utf8"
#define TRACER2(a,b)
#define XMLBEANSXX_DEBUG2(a,b) XMLBEANSXX_DEBUG(a,b)


XMLBEANSXX_LOGGER_PTR_SET(XercesParser::log, "xmlbeansxx.XercesParser");
XMLBEANSXX_LOGGER_PTR_SET(MySAX2Handler::log,"xmlbeansxx.MySAX2Handler");

XERCES_CPP_NAMESPACE_USE


MySAX2Handler::MySAX2Handler(XercesParser *p): parser(p) {}

std::string getPrefix(const std::string &qname){
    std::string::size_type p=qname.find(":");
    if(p!=std::string::npos) return qname.substr(0,p);
    return std::string();
}

namespace {
inline bool isPersistPrefix(const std::string& prefix){
	return (0 == prefix.compare(0,XmlBeans::persistentPrefix().length(),XmlBeans::persistentPrefix()));
}
};

void MySAX2Handler::startElement(	const XMLCh* const uri,
					const XMLCh* const localname,
					const XMLCh* const qname,
					const Attributes& attrs)
{
    {
        // LOGS
        XMLBEANSXX_DEBUG2(log, "SAX2 - StartElement info")
        XMLBEANSXX_DEBUG2(log, string("localname: ") + transcode(localname))
        XMLBEANSXX_DEBUG2(log, string("URI      : ") + transcode(uri))
        XMLBEANSXX_DEBUG2(log, string("qname    : ") + transcode(qname))


        XMLBEANSXX_DEBUG2(log, "attributes...");
        for(unsigned int i=0; i<attrs.getLength(); i++) {
            XMLBEANSXX_DEBUG2(log, string("localname: ") + transcode(attrs.getLocalName(i)));
            XMLBEANSXX_DEBUG2(log, string("URI      : ") + transcode(attrs.getURI(i)));
            XMLBEANSXX_DEBUG2(log, string("type     : ") + transcode(attrs.getType(i)));
            XMLBEANSXX_DEBUG2(log, string("value    : ") + transcode(attrs.getValue(i)));
        }
        XMLBEANSXX_DEBUG2(log, "SAX2 - end info")
    }

    std::string prefix = getPrefix(transcode(qname));
    QName name;
    if(isPersistPrefix(prefix))
    	 name = QName(transcode(uri), transcode(localname));
    else name = QName(transcode(uri), transcode(localname), prefix);

    XMLBEANSXX_DEBUG2(log, std::string("begin element: ") + name)
    XMLBEANSXX_DEBUG2(log, std::string("prefix: ") + prefix)

    if(parser->nodesStack.empty())
	throw XmlException(string("no XmlObject on XercesParser stack"));


    XmlObjectPtr top=parser->nodesStack.top().obj;
    XmlObjectPtr n;
    if(!top) throw XmlException(string("no XmlObjectPtr on XercesParser stack"));

    if(parser->nodesStack.size()==1){
	//hack for type casting. Runtime defined type
	Contents::Walker::ElemsType e = Contents::Walker::getElems(*top);
       	XMLBEANSXX_DEBUG2(log, std::string("elements in parsed XmlObject:") + TextUtils::intToString(e.size()));
	if(e.size()==1) {
		n=XmlObjectPtr(new XmlObject(ContentsPtr(new Contents(e.front().second->getSchemaType()))));
        	XMLBEANSXX_DEBUG2(log, std::string("predefined type found:") + n->getSchemaType()->name);
		name = e.front().first;
	}

	//this is the ROOT element
	top->free();
	XmlObjectPtr root = globalTypeSystem()->createDocumentByName(name);
	if (root) {
	    root->createContents();
	    top->setXmlObject(*root);
	}

    }

    top->createContents();

    {
	if(!n) n=top->getSchemaType()->createSubObject(name);

	// test for xsi:type or create default XmlObject
        for (unsigned int i = 0; i < attrs.getLength(); i++) {
	    QName name(transcode(attrs.getURI(i)), transcode(attrs.getLocalName(i)));
	    if(name == XmlBeans::xsi_type()) {
		QName value=parser->nsSplit(transcode(attrs.getValue(i)));
        	XMLBEANSXX_DEBUG2(log, std::string("xsi:type = ") + value)
		n = globalTypeSystem()->createByName(value);
		if (!n) throw XmlException(string("Xsd Type '")+value+string("' not defined in builtin type system"));
	    } else if(name == XmlBeans::xsi_array()) {
		QName value=parser->nsSplit(transcode(attrs.getValue(i)));
        	XMLBEANSXX_DEBUG2(log, std::string("xsi:array = ") + value)
//		n = globalTypeSystem()->createArrayByName(value);
		if (!n) throw XmlException(string("Xsd Type '")+value+string("' not defined in builtin type system"));

	    }
	}

	if(!n) throw XmlException(string("Cannot create subelement '")+name+string("' on object of class "+top->getSchemaType()->className));



    }

    n->createContents();


    {
        XMLBEANSXX_DEBUG2(log, "add attributes")
        for (unsigned int i = 0;i < attrs.getLength(); i++) {
	    QName name(transcode(attrs.getURI(i)), transcode(attrs.getLocalName(i)), getPrefix(transcode(attrs.getQName(i))));
    	    XMLBEANSXX_DEBUG2(log, std::string("attribute name (prefix): (") +  name.prefix + ")" + name)

	    if (name == XmlBeans::xsi_type()) continue;
	    if (name == XmlBeans::xsi_array()) continue;
	    std::string value(transcode(attrs.getValue(i)));
            xmlbeansxx::Contents::Walker::setAttr(*n,name, value);
        }
    }


    {	//add namespaces as attributes
	    XmlContext::StoredLinks ns=parser->xmlContext.getLastStoredLinks();
	    XMLBEANSXX_FOREACH(XmlContext::StoredLinks::iterator,i,ns) {
	    	if(!isPersistPrefix(i->first)) {
	    		QName name(XmlBeans::xmlns(),i->first);
    	    		XMLBEANSXX_DEBUG2(log, std::string("namespace attribute name: ")+ name + " = " + std::string(i->second) )
	    		xmlbeansxx::Contents::Walker::setAttr(*n,name, i->second);
		}
	    }
    }

    parser->xmlContext.remember();

    {
        XMLBEANSXX_DEBUG2(log, std::string("append element name (prefix): (") +name.prefix+ ")"+ name  )

	Contents::Walker::appendElem(*(parser->nodesStack.top().obj),name,n->contents);
        parser->nodesStack.push(EmptyParser::StackEl(n,n->getSchemaType()->processContents,name));
    }

}
void MySAX2Handler::startPrefixMapping(const XMLCh* const _prefix, const XMLCh* const _uri) {
    	//NAMESPACES
	std::string prefix = transcode(_prefix);
	std::string uri = transcode(_uri);
        parser->xmlContext.addNamespace(prefix, uri);
}


#if (XERCES_VERSION_MAJOR == 3)
void MySAX2Handler::characters(const XMLCh* const chars, const XMLSize_t length) {
#else
void MySAX2Handler::characters(const XMLCh* const chars, const unsigned int length) {
#endif

        string s = transcode(chars, length);
//    	parser->nodesStack.top().str+=s;

	std::string str = TextUtils::applyContentTypeRules(s,parser->nodesStack.top().obj->getSchemaType());

	if(str.size()>0)
		Contents::Walker::appendElem(*(parser->nodesStack.top().obj),XmlBeans::textElementName(),xmlbeansxx::ContentsPtr(new StringContents(str)));
    	XMLBEANSXX_DEBUG2(log, std::string("element value: ") + str )
}

void MySAX2Handler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
 {
    XMLBEANSXX_DEBUG2(log, std::string("end element ") + transcode(qname) )
    EmptyParser::StackEl e=parser->nodesStack.top();
    XmlObjectPtr n=e.obj;
//    n->setSimpleContent(e.str);
//    XMLBEANSXX_DEBUG2(log, std::string("element end value: ") + e.str )
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
    XMLBEANSXX_WARN(log,std::string("MySAX2Handler::warning() - Xerces-c warning: ")+getErrorStr(exc));
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
	return parser->transcoder.transcode(chars);
/*	char* message = XMLString::transcode(chars);
	std::string retu(message);
	XMLString::release(&message);
	return retu;
*/
}


//----------------------

XercesParser::XercesParser():transcoder(XmlBeans::encoding().c_str()){
    init();
}

XercesParser::XercesParser(const XmlOptions &opts):EmptyParser(opts),transcoder(XmlBeans::encoding().c_str()) {
    init();
}



void XercesParser::init(bool reinit) {
    XMLBEANSXX_DEBUG(log,"start");
/*    static bool initialized = false;

    if(!initialized) {
    	XMLBEANSXX_DEBUG(log,"initializing xerces-c");
	XMLPlatformUtils::Initialize();
	initialized=true;
    }
*/
    sax2=auto_ptr<SAX2XMLReader>(XMLReaderFactory::createXMLReader());
    handler=auto_ptr<MySAX2Handler>(new MySAX2Handler(this));
    sax2->setContentHandler(handler.get());
    sax2->setErrorHandler(handler.get());

    sax2->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    sax2->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);

    XMLBEANSXX_DEBUG(log,"finish");
}

XercesParser::~XercesParser() {}

void XercesParser::updateOptions() {
	sax2->setFeature(XMLUni::fgSAX2CoreValidation, options.getValidation());
	sax2->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);
	sax2->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
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
    XMLBEANSXX_DEBUG(log,std::string("XmlParser::parse() - start"));

    updateOptions();

    while (!nodesStack.empty())
	nodesStack.pop();

    documentRoot.createContents();
    XmlObjectPtr root_ptr=boost::shared_ptr<XmlObject>(&documentRoot,null_deleter2());

    nodesStack.push(StackEl(root_ptr,documentRoot.getSchemaType()->processContents,QName()));


    StdInputSource s(in);
    XMLBEANSXX_DEBUG(log,"parsing:");
    sax2->parse(s);
    nodesStack.pop();
    BOOST_ASSERT(nodesStack.empty());
    XMLBEANSXX_DEBUG(log,std::string("XmlParser::parse() - finish"));
}




void XercesParser::loadGrammars(const std::vector<std::string> &fileNames) {
    XMLBEANSXX_FOREACH(std::vector<std::string>::const_iterator,it,fileNames) {
        loadGrammar(*it);
    }
}

void XercesParser::loadGrammar(const std::string &fileName) {
    XMLBEANSXX_DEBUG(log,std::string("loadGrammar: ") + fileName);
    sax2->loadGrammar(fileName.c_str(),Grammar::SchemaGrammarType,true);
}
void XercesParser::unloadGrammars() {
    sax2->resetCachedGrammarPool();
}



}

