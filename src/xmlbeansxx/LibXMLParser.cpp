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

#include "LibXMLParser.h"
#include "XmlTypesGen.h"
#include "BeansException.h"
#include "Tracer.h"
#include "defs.h"
#include <stdio.h>
#include <boost/scoped_array.hpp>
#include <boost/weak_ptr.hpp>

#include <libxml/xmlstring.h>

#include "logger.h"
#include "XmlTypesGen.h"
#include <string>
#include "XmlBeans.h"

#define LOG4CXX_DEBUG2(a,b) LOG4CXX_DEBUG(a, b)
//#define LOG4CXX_DEBUG2(a,b) 

namespace {
    const bool INSERT_INTO_CURSOR = true;
    const bool IMMEDIATE_RETURN = false;
}

namespace xmlbeansxx {

    
// include class static objects in scope (else tests won't link)
bool LibXMLParser::initialized = false;
//boost::mutex LibXMLParser::mutex;

using namespace std;


/**
 * SAX callbacks headers
 */
extern "C" {
    void static startElementNs(void *ctx,
                               const xmlChar *localname,
                               const xmlChar *prefix,
                               const xmlChar *URI,
                               int nb_namespaces,
                               const xmlChar **namespaces,
                               int nb_attributes,
                               int nb_defaulted,
                               const xmlChar **attributes);
    void static endElementNs(void *ctx,
                             const xmlChar *localname,
                             const xmlChar *prefix,
                             const xmlChar *URI);
    void static characters(void *ctx, const xmlChar *ch, int len);
    void static serror(void *ctx, xmlErrorPtr error);

    static xmlSAXHandler xmlbeansxxSAX2HandlerStruct = {
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                characters,
                NULL,
                NULL,
                NULL,
                NULL,//warningDebug,
                NULL,//errorDebug,
                NULL,//fatalErrorDebug,
                NULL,
                NULL,//cdataBlockDebug,
                NULL,
                XML_SAX2_MAGIC,
                NULL,
                startElementNs,
                endElementNs,
                serror
            };

    static xmlSAXHandlerPtr xmlbeansxxSAX2Handler = &xmlbeansxxSAX2HandlerStruct;

}

LOGGER_PTR_SET(LOG,"xmlbeansxx.LibXMLParser");

LibXMLParser::LibXMLParser() {
    init();
}

LibXMLParser::LibXMLParser(const XmlOptions &options) : EmptyParser(options) {
    init();
}

LibXMLParser::~LibXMLParser() {
    unloadGrammars();
}

void LibXMLParser::init() {
    //LOG4CXX_DEBUG(LOG, "init - start");
    /*
    #ifdef BOOST_HAS_THREADS
            {
                boost::detail::thread::scoped_lock<boost::mutex> lock(mutex);
                if (initialized == false) {
                    //LOG4CXX_DEBUG(LOG, "initializing libxml2 parser...");
                    xmlInitParser();
                    initialized = true;
                }
            }
    #endif
    */

    schemaParserCtxt = NULL;
    schema           = NULL;
    validationCtxt   = NULL;
    schemaPlug       = NULL;

///    xsi_ns = XmlBeans::xsi_ns();
    //LOG4CXX_DEBUG(LOG, "init - finish");
}

string LibXMLParser::generateErrorMessage(xmlErrorPtr error) {
    if (error == NULL)
        return string("Can't get xmlError message from libxml2");

    return string(error->message);
}

void LibXMLParser::parse(istream &in, XmlObject &documentRoot) {
    //TODO: improve it!
    string line, doc;
    while (getline(in, line)) {
        doc.append(line);
    }
    parse(doc, documentRoot);
}


struct null_deleter
{
	template<class T> void operator()(T *) const{}
};
	    
	    

void LibXMLParser::parse(const std::string &doc, xmlbeansxx::XmlObject &documentRoot) {

    xmlSAXHandlerPtr handler;

    handler = xmlbeansxxSAX2Handler;

//    cursor = documentRoot->newCursor();
//    xmlContext.setLink("", "");

    // Set substitutes for entities
    xmlSubstituteEntitiesDefault (1);
    
    while (!nodesStack.empty())
	nodesStack.pop();

    documentRoot.createContents();
    XmlObjectPtr root_ptr=boost::shared_ptr<XmlObject>(&documentRoot,null_deleter());
    
    nodesStack.push(StackEl(root_ptr,documentRoot.getSchemaType()->processContents,QName()));
    

    int errNo;
    if ( validationCtxt == NULL )  {
        if ((errNo = xmlSAXUserParseMemory(handler,
                                           (void *) this,
                                           (const char *) doc.c_str(),
                                           doc.length())) != 0) {


            xmlErrorPtr error = xmlGetLastError();
            throw new XmlParseException(generateErrorMessage(error));
        }
    } else {
        xmlParserInputBufferPtr buf = NULL;
        buf = xmlParserInputBufferCreateMem((const char *)doc.c_str(), doc.length(), XML_CHAR_ENCODING_NONE);

        if (( errNo = xmlSchemaValidateStream(validationCtxt,
                                              buf,
                                              XML_CHAR_ENCODING_NONE,
                                              handler,
                                              (void *)this)) != 0 ) {


            xmlErrorPtr error = xmlGetLastError();
            throw new XmlParseException(generateErrorMessage(error));
        }
    }
    

///    cursor = Null();
}

void addSchemaValidityError(void *ctx, const char *msg, ...) {
    char buf[1024];
    va_list args;

    va_start(args, msg);
    int len = vsnprintf(buf, sizeof(buf)/sizeof(buf[0]), msg, args);
    va_end(args);

    LibXMLParser *parser = (LibXMLParser *) ctx;
    if (len == 0)
        parser->addError("Can't create schema validity error!");
    else
        parser->addError(buf);
}

void addSchemaValidityWarning(void *ctx, const char *msg, ...) {
    char buf[1024];
    va_list args;

    va_start(args, msg);
    int len = vsnprintf(buf, sizeof(buf)/sizeof(buf[0]), msg, args);
    va_end(args);

    LibXMLParser *parser = (LibXMLParser *) ctx;
    if (len == 0)
        parser->addWarning("Can't create schema validity warning!");
    else
        parser->addWarning(buf);
}

// TODO: how to load grammars from couple of files?
void LibXMLParser::loadGrammars(const vector<string>& files) {
    //         FOREACH(it, files) {
    //             loadGrammar(*it);
    //         }
}

/*
 * Load schema and create internal representation.
 * Plug schema validation context to SAX parser.
 */
 
void LibXMLParser::loadGrammar(const string &filename) {
    {
        // prevent memory leak from previous load
        if (schemaPlug != NULL)
            unloadGrammars();

        if (!errors.empty())
            errors.clear();
        if (!warnings.empty())
            warnings.clear();
    }

    schemaParserCtxt = xmlSchemaNewParserCtxt(filename.c_str());
    if (schemaParserCtxt == NULL) {
        throw BeansException("Can't create xmlSchemaParserContext");
    }

    xmlSchemaSetParserErrors(schemaParserCtxt,
                             addSchemaValidityError,
                             addSchemaValidityWarning,
                             this);

    schema = xmlSchemaParse(schemaParserCtxt);
    if (schema == NULL) {
        unloadGrammars();
        throw BeansException("Can't precompile schema");
    }

    if ((validationCtxt = xmlSchemaNewValidCtxt(schema)) == NULL) {
        unloadGrammars();
        throw BeansException("Can't create schema validation context");
    }

    xmlSchemaSetValidErrors(validationCtxt,
                            addSchemaValidityError,
                            addSchemaValidityWarning,
                            this);

    /*schemaPlug = xmlSchemaSAXPlug(validationCtxt,
                                  &saxHandlerPtr,
                                  &(this->user_data));
    if (schemaPlug == NULL) {
        // TODO: errors
        LOG4CXX_ERROR(LOG, "Can't plug in schema validation layer");
        unloadGrammars();
        return;
    }
    */
}

// clear after schemas loading
void LibXMLParser::unloadGrammars() {
    if (schemaPlug) {
        xmlSchemaSAXUnplug(schemaPlug);
        schemaPlug = NULL;
    }
    if (validationCtxt) {
        xmlSchemaFreeValidCtxt(validationCtxt);
        validationCtxt = NULL;
    }
    if (schema) {
        xmlSchemaFree(schema);
        schema = NULL;
    }
    if (schemaParserCtxt) {
        xmlSchemaFreeParserCtxt(schemaParserCtxt);
	schemaParserCtxt = NULL;
    }
	
}



//////////////////////////////////////////////////////////////////////////
/////////////////////// SAX callback routines ////////////////////////////
//////////////////////////////////////////////////////////////////////////

// common xmlChar **namespaces indexes
enum { NS_PREFIX = 0, NS_URI = 1 };

// common xmlChar **attributes indexes
const int ATTRTABSIZE = 5;
enum { ATTR_LOCALNAME = 0,
       ATTR_PREFIX    = 1,
       ATTR_URI       = 2,
       ATTR_VALUE     = 3,
       ATTR_END       = 4 };

// create attribute's name consisting of prefix:localname
// if prefix is NULL, only name == localname
inline string getAttrName(const xmlChar **attributes, int current) {
    string an;
    if (attributes[current+ATTR_PREFIX] != NULL) {
        an.append((const char *) attributes[current+ATTR_PREFIX]);
        an.append(":");
    }
    an.append((const char *) attributes[current+ATTR_LOCALNAME]);
    return an;
}

// extract attribute's value from string, contained between start/end
inline string getAttrValue(const xmlChar **attributes) {
    const xmlChar *start = attributes[ATTR_VALUE];
    const xmlChar *end   = attributes[ATTR_END];

    int len = end - start;
    return string((const char *) start, len);
}

// prefix     - when not provided is NULL
// URI        - when not provided is NULL
// namespaces - consists of prefix/namespace pairs
// attributes - attribute's value is between 'value' and 'end'
void startElementNs(void *ctx,
                    const xmlChar *localname,
                    const xmlChar *prefix,
                    const xmlChar *URI,
                    int nb_namespaces,
                    const xmlChar **namespaces,
                    int nb_attributes,
                    int nb_defaulted,
                    const xmlChar **attributes) // (localname/prefix/URI/value/end)
{
    if (IMMEDIATE_RETURN)
        return;
    {
        // LOGS
        LOG4CXX_DEBUG2(LOG, "SAX2 - StartElementNS info")
        LOG4CXX_DEBUG2(LOG, string("localname: ") + (const char *) localname)
        LOG4CXX_DEBUG2(LOG, string("prefix   : ") + (prefix == NULL ? "" : (const char *) prefix))
        LOG4CXX_DEBUG2(LOG, string("URI      : ") + (URI == NULL ? "" : (const char *) URI))
        
        
        LOG4CXX_DEBUG2(LOG, "namespaces...");
        for(int i=0;i<nb_namespaces;i++) {
            LOG4CXX_DEBUG2(LOG, string("namespace: ")
                          + (namespaces[2*i] == NULL ? "" : (const char *) namespaces[2*i])
                          + "="
                          + (const char *) namespaces[2*i+1]);
        }
        LOG4CXX_DEBUG2(LOG, "attributes...");
        for(int i=0;i<nb_attributes;i++) {
            LOG4CXX_DEBUG2(LOG, string("attribute: "));
            LOG4CXX_DEBUG2(LOG, string("localname: ") + (const char *) attributes[5*i]);
            LOG4CXX_DEBUG2(LOG, string("prefix   : ")
                          + (attributes[5*i+1] == NULL
                             ? ""
                             : (const char *) attributes[5*i+1]));
            LOG4CXX_DEBUG2(LOG, string("URI      : ")
                          + (attributes[5*i+2] == NULL
                             ? ""
                             : (const char *) attributes[5*i+2]));
            LOG4CXX_DEBUG2(LOG, string("value    : ") + getAttrValue(attributes + 5*i));
        }
        LOG4CXX_DEBUG2(LOG, "SAX2 - end info")
    }

    /* *************************************************************************** */
    LibXMLParser *parser = (LibXMLParser *) ctx;


    {
    	//NAMESPACES
        LOG4CXX_DEBUG2(LOG, "store namespaces in xmlContext")
        for (int current = 0;
                current < 2 * nb_namespaces;
                current += 2) {

            LOG4CXX_DEBUG2(LOG, "iteration")

            const char * ns0 = (const char *) namespaces[current + NS_URI];
            BOOST_ASSERT(ns0 != NULL);
            StoreString ns(ns0);
            const char *prefix = (const char *) namespaces[current + NS_PREFIX];
            LOG4CXX_DEBUG2(LOG, "prefix: " << (prefix == NULL ? "" : prefix) << " namespace: " << ns)
            parser->xmlContext.addNamespace(prefix == NULL ? "" : prefix, ns);
        }
    }

    
    QName name(parser->getQName((const char *) prefix, (const char *) localname));
    LOG4CXX_DEBUG2(LOG, std::string("begin element: ") + name)
    
    if(parser->nodesStack.empty())
	throw XmlException(string("no XmlObject on LibXMLParser stack"));


    XmlObjectPtr top=parser->nodesStack.top().obj;
    XmlObjectPtr n;
    if(!top) throw XmlException(string("no XmlObjectPtr on LibXMLParser stack"));					   

    if(parser->nodesStack.size()==1){
	//hack for type casting. Runtime defined type
	Contents::Walker::ElemsType e = Contents::Walker::getElems(*top);
       	LOG4CXX_DEBUG2(LOG, std::string("elements in parsed XmlObject:") + TextUtils::intToString(e.size()));
	if(e.size()==1) {
		n=XmlObjectPtr(new XmlObject(ContentsPtr(new Contents(e.front().second->getSchemaType()))));
        	LOG4CXX_DEBUG2(LOG, std::string("predefined type found:") + n->getSchemaType()->name);
		name = e.front().first;
	}
	//this is the ROOT element
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
        for (int current = 0; current < ATTRTABSIZE * nb_attributes; current += ATTRTABSIZE) {
	    QName name(parser->getQName((const char *) attributes[current + ATTR_PREFIX], (const char *) attributes[current + ATTR_LOCALNAME], true));
	    if(name == XmlBeans::xsi_type()) {
		QName value=parser->nsSplit(getAttrValue(attributes + current));
        	LOG4CXX_DEBUG2(LOG, std::string("xsi:type = ") + value)
		n = globalTypeSystem()->createByName(value);
		if (!n) throw XmlException(string("Xsd Type '")+value+string("' not defined in builtin type system"));					   
	    } else if(name == XmlBeans::xsi_array()) {
		QName value=parser->nsSplit(getAttrValue(attributes + current));
        	LOG4CXX_DEBUG2(LOG, std::string("xsi:array = ") + value)
		n = globalTypeSystem()->createArrayByName(value);
		if (!n) throw XmlException(string("Xsd Type '")+value+string("' not defined in builtin type system"));					   
	    
	    }
	}
	
	if(!n) throw XmlException(string("Cannot create subelement '")+name+string("' on object of class "+top->getSchemaType()->className));



    }	

    n->createContents();
    

    {
        LOG4CXX_DEBUG2(LOG, "add attributes")
        for (int current = 0; current < ATTRTABSIZE * nb_attributes; current += ATTRTABSIZE) {
            QName name(parser->getQName((const char *) attributes[current + ATTR_PREFIX], (const char *) attributes[current + ATTR_LOCALNAME], true));
    	    LOG4CXX_DEBUG2(LOG, std::string("attribute name (prefix): (") +  name.prefix + ")" + name)
	    
	    if (name == XmlBeans::xsi_type()) continue;
	    if (name == XmlBeans::xsi_array()) continue;
	    std::string value(getAttrValue(attributes + current));
            xmlbeansxx::Contents::Walker::setAttr(*n,name, value);
        }
    }

    {	//add namespaces as attributes
	    XmlContext::StoredLinks ns=parser->xmlContext.getLastStoredLinks();
	    XMLBEANSXX_FOREACH(XmlContext::StoredLinks::iterator,i,ns) {
	    	QName name(XmlBeans::xmlns(),i->first);
    	    	LOG4CXX_DEBUG2(LOG, std::string("namespace attribute name: ")+ name + " = " + std::string(i->second) )
	    	xmlbeansxx::Contents::Walker::setAttr(*n,name, i->second);
	    }
    }

    parser->xmlContext.remember();
    
    {
	LOG4CXX_DEBUG2(LOG, std::string("append element name (prefix): (") +name.prefix+ ")"+ name  )

	Contents::Walker::appendElem(*(parser->nodesStack.top().obj),name,n->contents);
        parser->nodesStack.push(LibXMLParser::StackEl(n,n->getSchemaType()->processContents,name));
    }
}

void endElementNs(void *ctx,
                  const xmlChar *localname,
                  const xmlChar *prefix,
                  const xmlChar *URI) {

    if (IMMEDIATE_RETURN)
        return;
    LOG4CXX_DEBUG2(LOG, "end element " << (const char *) localname )
    LibXMLParser *parser = (LibXMLParser *) ctx;
///    if (INSERT_INTO_CURSOR)
///        parser->cursor->pop();
///    parser->xmlContext.restore();
    LibXMLParser::StackEl e=parser->nodesStack.top();
    XmlObjectPtr n=e.obj;
    n->setSimpleContent(e.str);
    parser->xmlContext.restore();
    parser->nodesStack.pop();

    LOG4CXX_DEBUG2(LOG, "leaving end element " << (const char *) localname)
}

void characters(void *ctx, const xmlChar *ch, int length) {
    if (IMMEDIATE_RETURN)
        return;
    LibXMLParser *parser = (LibXMLParser *) ctx;

    string s( (const char*) ch, length);
///    if (INSERT_INTO_CURSOR)
///       parser->cursor->insertChars(s);

    parser->nodesStack.top().str+=s;
}

void serror(void *ctx, xmlErrorPtr error) {
	std::stringstream err;
	
	err << "serror:"	<< error->message;
    	err << ", line:"  	<< error->line;
	err << ", column:" 	<< error->int2;
	if(error->str1)
	err << " ,str1:" 	<< error->str1;
	if(error->str2)
	err << " ,str2:" 	<< error->str2;
	if(error->str3)
	err << " ,str3:" 	<< error->str3;
    LOG4CXX_ERROR(LOG,err.str());
}
}
