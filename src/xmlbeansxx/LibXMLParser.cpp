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
#include "TypeSystem.h"
#include "XmlTypesGen.h"
#include "BeansException.h"
#include "Tracer.h"
#include "defs.h"
#include <stdio.h>
#include <boost/scoped_array.hpp>

#include <libxml/xmlstring.h>

#include <log4cxx/logger.h>

namespace xmlbeansxx {

    // include class static objects in scope (else tests won't link)
    bool LibXMLParser::initialized = false;
    //boost::mutex LibXMLParser::mutex;

    using namespace std;

    class XmlObject;

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
    NULL
};

static xmlSAXHandlerPtr xmlbeansxxSAX2Handler = &xmlbeansxxSAX2HandlerStruct;

    }

    log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger(
        string("xmlbeansxx.LibXMLParser") );

    LibXMLParser::LibXMLParser()
        : options(new XmlOptions())
    {
        init();
    }

    LibXMLParser::LibXMLParser(const boost::shared_ptr<XmlOptions> &options)
    {
        this->options = options;
        init();
    }
    
    LibXMLParser::~LibXMLParser()
    {
        unloadGrammars();
    }

    void LibXMLParser::init()
    {
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
        user_data = this;

        schema         = NULL;
        validationCtxt = NULL;
        schemaPlug     = NULL;


        xsi_ns = globalTypeSystem()->
            addNamespace("http://www.w3.org/2001/XMLSchema-instance");

        //LOG4CXX_DEBUG(LOG, "init - finish");
    }

    void LibXMLParser::parse(istream &in, XmlObject *documentRoot)
    {
        //TODO: improve it!
        string line, doc;
        while (getline(in, line)) {
            //TODO:  przemek a to po co ten " " przeciez to moze popsuc dokument xml!
            //line.append(" ");
            doc.append(line);
        }
        parse(doc, documentRoot);
    } 

    void LibXMLParser::parse(std::string &doc, XmlObject *documentRoot)
    {
	
        xmlSAXHandlerPtr handler;

        handler = xmlbeansxxSAX2Handler;

        while (!nodesStack.empty())
            nodesStack.pop();

        nodesStack.push(
            Element(documentRoot,
                    documentRoot->getSchemaType()->processContents,
                    0) );

        int errNo;
        if ( validationCtxt == NULL )  {
            if ((errNo = xmlSAXUserParseMemory(handler,
                                           (void *) this,
                                           (const char *) doc.c_str(),
                                           doc.length())) != 0) {
                // TODO: error handling (xmlParserSetError... ?)
                LOG4CXX_ERROR(LOG, "parse error");
                throw new XmlParseException(string("LibXMLParseException"));
            }
        } 
        else {
            xmlParserInputBufferPtr buf = NULL;
            buf = xmlParserInputBufferCreateMem((const char *)doc.c_str(), doc.length(), XML_CHAR_ENCODING_NONE);

            if (( errNo = xmlSchemaValidateStream(validationCtxt, 
                            buf,
                            XML_CHAR_ENCODING_NONE, 
                            handler, 
                            (void *)this)) != 0 ) {
                
                LOG4CXX_ERROR(LOG, "parse (with validation) error");
                throw new XmlParseException(string("LibXMLParseException"));
             }
        }
        nodesStack.pop();
        BOOST_ASSERT(nodesStack.empty());
        //LOG4CXX_DEBUG(LOG, string("LibXMLParser::parse() - finish"));
    }

    void addSchemaValidityError(void *ctx, const char *msg, ...)
    {
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

    void addSchemaValidityWarning(void *ctx, const char *msg, ...)
    {
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
    void LibXMLParser::loadGrammars(const vector<string>& files)
    {
//         FOREACH(it, files) {
//             loadGrammar(*it);
//         }
    }

    /*
     * Load schema and create internal representation.
     * Plug schema validation context to SAX parser.
     */
    void LibXMLParser::loadGrammar(const string filename)
    {
        {
            // prevent memory leak from previous load
            if (schemaPlug != NULL)
                unloadGrammars();

            if (!errors.empty())
                errors.clear();
            if (!warnings.empty())
                warnings.clear();
        }

        xmlSchemaParserCtxtPtr schemaParserCtxt;
        schemaParserCtxt = xmlSchemaNewParserCtxt(filename.c_str());
        if (schemaParserCtxt == NULL) {
            // TODO: error handling
            LOG4CXX_ERROR(LOG, "Can't create xmlSchemaParserContext");
            return;
        }

        xmlSchemaSetParserErrors(schemaParserCtxt,
                            (xmlSchemaValidityErrorFunc) fprintf,
                                        (xmlSchemaValidityWarningFunc) fprintf,
                                                    stderr);

        //xmlSchemaSetParserErrors(schemaParserCtxt,
         //                        addSchemaValidityError,
          //                       addSchemaValidityWarning,
           //                      this);

        schema = xmlSchemaParse(schemaParserCtxt);
        xmlSchemaFreeParserCtxt(schemaParserCtxt); // not needed anymore
        if (schema == NULL) {
            // TODO: error handling
            LOG4CXX_ERROR(LOG, "Can't precompile schema");
            unloadGrammars();
            return;
        }

        if ((validationCtxt = xmlSchemaNewValidCtxt(schema)) == NULL) {
            // TODO: error handling
            LOG4CXX_ERROR(LOG, "Can't create schema validation context");
            unloadGrammars();
            return;
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
    void LibXMLParser::unloadGrammars()
    {
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
    }

    // return namespace number bound to prefix or default namespace if
    // no prefix was given
    pair<int,string> LibXMLParser::nsSplit(const string ns)
    {
        string::size_type pos = ns.find(':');
        if (pos == string::npos) {
            return pair<int,string> (xmlContext.getLink(""), ns);
        } else {
            return pair<int,string> (
                xmlContext.getLink(ns.substr(0,pos)),
                ns.substr(pos+1) );
        }
    }

    pair<string,string> LibXMLParser::tagSplit(const string tag)
    {
        string::size_type pos = tag.find(':');
        if (pos == string::npos) {
            return pair<string,string>("", tag);
        } else {
            return pair<string,string>( tag.substr(0,pos), tag.substr(pos+1) );
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
    inline string getAttrName(const xmlChar **attributes, int current)
    {
        string an;
        if (attributes[current+ATTR_PREFIX] != NULL) {
            an.append((const char *) attributes[current+ATTR_PREFIX]);
            an.append(":");
        }
        an.append((const char *) attributes[current+ATTR_LOCALNAME]);
        return an;
    }

    // extract attribute's value from string, contained between start/end
    inline string getAttrValue(const xmlChar **attributes, int current)
    {
        const xmlChar *start = attributes[current+ATTR_VALUE];
        const xmlChar *end   = attributes[current+ATTR_END];
        // can be buggy?
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
//         //LOG4CXX_DEBUG(LOG, "SAX2 - StartElementNS");
//         //LOG4CXX_DEBUG(LOG, string("localname: ") + (const char *) localname);
//         //LOG4CXX_DEBUG(LOG, string("prefix   : ") + (prefix == NULL ? "" : (const char *) prefix));
//         //LOG4CXX_DEBUG(LOG, string("URI      : ") + (URI == NULL ? "" : (const char *) URI));
//         //LOG4CXX_DEBUG(LOG, "namespaces...");
//         FOR(i, nb_namespaces) {
//             //LOG4CXX_DEBUG(LOG, string("namespace: ")
//                           + (namespaces[2*i] == NULL ? "" : (const char *) namespaces[2*i])
//                           + "="
//                           + (const char *) namespaces[2*i+1]);
//         }
//         //LOG4CXX_DEBUG(LOG, "attributes...");
//         FOR(i, nb_attributes) {
//             //LOG4CXX_DEBUG(LOG, string("attribute: "));
//             //LOG4CXX_DEBUG(LOG, string("localname: ") + (const char *) attributes[5*i]);
//             //LOG4CXX_DEBUG(LOG, string("prefix   : ")
//                           + (attributes[5*i+1] == NULL
//                              ? "" 
//                              : (const char *) attributes[5*i+1]));
//             //LOG4CXX_DEBUG(LOG, string("URI      : ")
//                           + (attributes[5*i+2] == NULL
//                              ? ""
//                              : (const char *) attributes[5*i+2]));
//             //LOG4CXX_DEBUG(LOG, string("value    : ") + getAttrValue(attributes, 5*i));
//         }

        /* *************************************************************************** */
        LibXMLParser *parser = (LibXMLParser *) ctx;

        string name((const char *) localname);

        //LOG4CXX_INFO(LOG, "star element " << localname);
        //TRACER(LOG, string("startElement(") + string(name) + ")");

        int restorePosition = parser->xmlContext.restorePosition();

        // add a new node without contents processing
        if (parser->nodesStack.top().processContents == false) {
            //LOG4CXX_DEBUG(LOG, "Adding new node without processing contents");
            XmlObjectPtr xmlObjPtr( XmlObject::Factory::newInstance() );

            for (int i = 0, current = 0;
                 i < nb_attributes;
                 i++, current += ATTRTABSIZE) {

                xmlObjPtr->getContents()->appendAttr(
                    getAttrName(attributes, current),
                    getAttrValue(attributes, current));
            }
            parser->nodesStack.top().obj->getContents()->appendElem(name,
                                                                    xmlObjPtr);
            parser->nodesStack.push(
                LibXMLParser::Element(xmlObjPtr.get(), false, restorePosition) );
        }
        // add a new node, but do process contents
        else {

            // map namespaces in parser's context to speed up prefix resolving
            FOR(i,nb_namespaces) {
                int ns = globalTypeSystem()->getNamespace(
                    (const char *) namespaces[2*i+NS_URI]);
                if (ns == -1)
                    throw XmlException(
                        string("Namespace '")
                        + string((const char *) namespaces[2*i+NS_URI])
                        + string("' is not registered in globalTypeSystem."));
                // bind prefix to namespace number
                string pfx;
                if (namespaces[2*i+NS_PREFIX] == NULL)
                    pfx = "";
                else
                    pfx = (const char *) namespaces[2*i+NS_PREFIX];
                parser->xmlContext.setLink(pfx, ns);
            }
            
            // parsed attributes are skipped
            boost::scoped_array<bool> skipped(new bool[nb_attributes]);

            // Creating object for this element
            XmlObjectPtr xmlObjPtr;
            bool isArray = false;

            //LOG4CXX_DEBUG(LOG, string("Parsing xsi attributes"));

            for (int i = 0, current = 0;
                 i < nb_attributes;
                 i++, current += ATTRTABSIZE) {

                skipped[i] = false;

                // get namespace number bound to attribute's prefix
                int ns_num;
                if (attributes[current+ATTR_PREFIX] == NULL)
                    ns_num = parser->xmlContext.getLink("");
                else
                    ns_num = parser->xmlContext.getLink(
                        (const char *) attributes[current+ATTR_PREFIX]);

                // parse xsi attributes
                if (ns_num == parser->xsi_ns) {
                    skipped[i] = true;

                    string attrval(getAttrValue(attributes, current));

                    // parse xsi:type=value, value="prefix:localname"
                    if (xmlStrncmp(attributes[current+ATTR_LOCALNAME],
                                   (const xmlChar *) "type",
                                   4) == 0) {
                        pair<int,string> nsTag = parser->nsSplit(attrval);
                        if (nsTag.first == -1)
                            throw XmlException(string("Undefined namespace link '")
                                               + attrval
                                               + string("' in type name, in element '")
                                               + string(name) + string("'"));

                        if (!isArray) {
                            //LOG4CXX_DEBUG(LOG, string("createByName.."));
                            xmlObjPtr = globalTypeSystem()->
                                createByName(nsTag.second, nsTag.first);
                            //LOG4CXX_DEBUG(LOG, string("createByName..returned"));

                            if (xmlObjPtr == 0)
                                throw XmlException(string("Xsd Type '")
                                                   + attrval
                                                   + string("' not defined in builtin type system"));
                        }
                    } else if (xmlStrncmp(attributes[current+ATTR_LOCALNAME],
                                          (const xmlChar *) "array",
                                          5) == 0) {
                        pair<int,string> nsTag = parser->nsSplit(attrval);
                        if (nsTag.first == -1)
                            throw XmlException(string("Undefined namespace link '")
                                               + attrval
                                               + string("' in type name, in element '")
                                               + string(name) + string("'"));
                        isArray = true;
                        //LOG4CXX_DEBUG(LOG, string("createArrayByName.."));
                        xmlObjPtr = globalTypeSystem()->
                            createArrayByName(nsTag.second, nsTag.first);
                        //LOG4CXX_DEBUG(LOG, string("createArrayByName..returned"));

                        if (xmlObjPtr == 0)
                            throw XmlException(string("Xsd Type '")
                                               + attrval
                                               + string("' not defined in builtin type system"));

                    } else if (xmlStrncmp(attributes[current+ATTR_LOCALNAME],
                                          (const xmlChar *) "schemaLocation",
                                          14) == 0) {
                        // just skip
                    }
                    else {
                        throw XmlException(string("Undefined schema instance usage '")
                                           + attrval
                                           + string("' in element ")
                                           + string(name));
                    }
                }
            } // end for()

            if (xmlObjPtr == 0) {
                //LOG4CXX_DEBUG(LOG, string("createSubObject ") + string(name)
                                   //+ string(" on object: "
                                   //+ parser->nodesStack.top().obj->
                                    //        getSchemaType()->className));

                xmlObjPtr = parser->nodesStack.top().obj->getSchemaType()->
                    createSubObject(name);

                //LOG4CXX_DEBUG(LOG, string("createSubObject returned"));

                if(xmlObjPtr == 0) {
                    //LOG4CXX_DEBUG(LOG, string("createSubObject was NULL"));
                    throw XmlException(string("Cannot create subelement '")
                                       + string(name)
                                       + string("' on object of class "
                                       + parser->nodesStack.top().obj->
                                                getSchemaType()->className));
                } else {
                    //LOG4CXX_DEBUG(LOG, string("createSubObject was not NULL"));
                }
            }

            // Removing default attributes and elements, they will be added
            // after parsing this element
            xmlObjPtr->getContents()->free();

            //LOG4CXX_DEBUG(LOG, string("Adding attributes to object"));

            //adding attributes to new object
            for (int i = 0, current = 0;
                 i < nb_attributes;
                 i++, current += ATTRTABSIZE) {

                if (skipped[i])
                    continue;

                string attrname(getAttrName(attributes, current));
                string attrval(getAttrValue(attributes, current));

                //LOG4CXX_DEBUG(LOG, string("adding attr.name : ") + attrname);
                //LOG4CXX_DEBUG(LOG, string("adding attr.value: ") + attrval);

                xmlObjPtr->getContents()->appendAttr(attrname, attrval);
            }
            parser->nodesStack.top().obj->getContents()->appendElem(name,
                                                                    xmlObjPtr);
            parser->nodesStack.push(
                LibXMLParser::Element(xmlObjPtr.get(),
                                      xmlObjPtr->getSchemaType()->processContents,
                                      restorePosition));
        } // else - adding new node with contents processing
    }

    void endElementNs(void *ctx,
                      const xmlChar *localname,
                      const xmlChar *prefix,
                      const xmlChar *URI)
    {
//         //LOG4CXX_DEBUG(LOG, "SAX2 - END elementNS here!");
//         //LOG4CXX_DEBUG(LOG, string("localname: ") + (const char *) localname);
//         //LOG4CXX_DEBUG(LOG, string("prefix   : ")
//                       + (prefix == NULL ? "" : (const char *) prefix));
//         //LOG4CXX_DEBUG(LOG, string("URI      : ")
//                       + (URI == NULL ? "" : (const char *) URI));
        LibXMLParser *parser = (LibXMLParser *) ctx;

        //TRACER(LOG,"endElementNs");
        LibXMLParser::Element elem = parser->nodesStack.top();
        elem.obj->setSimpleContent(elem.characters);
        elem.obj->getContents()->insertDefaults(elem.obj->getSchemaType());
        parser->xmlContext.restore(parser->nodesStack.top().restorePosition);
        parser->nodesStack.pop();
    }

    void characters(void *ctx, const xmlChar *ch, int length)
    {
        LibXMLParser *parser = (LibXMLParser *) ctx;

        string s( (const char*) ch, length);
        //cout << "got(" << length << ") characters: " << s << endl;
        parser->nodesStack.top().characters += s;
    }

    void serror(void *ctx, xmlErrorPtr error)
    {
        LOG4CXX_ERROR(LOG, string("serror: ") + error->message);
    }
}
