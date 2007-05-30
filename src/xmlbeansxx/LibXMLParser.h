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


#ifndef _XMLBEANSXX_LIBXMLPARSER_H_
#define _XMLBEANSXX_LIBXMLPARSER_H_

#include <list>
#include <stack>
#include <string>
#include <istream>
#include <boost/thread.hpp>

#include <libxml/SAX2.h>
#include <libxml/xmlschemas.h>


#include "XmlContext.h"
#include "XmlOptions.h"
#include "QName.h"
#include "XmlTypesGen.h"

#include "XmlParser.h"



namespace xmlbeansxx {
class XmlObject;

    /** 
     * Parse XML documents using libxml2 parser.
     */
class LibXMLParser : public XmlParser {
    private:

        // initialize libxml2 parser only once
        static bool initialized;

#ifdef BOOST_HAS_THREADS
        static boost::mutex mutex;
#endif

        xmlSchemaParserCtxtPtr schemaParserCtxt; // schema parser context
        xmlSchemaPtr           schema;           // internal schema representation
        xmlSchemaValidCtxtPtr  validationCtxt;   // based on schema
        xmlSchemaSAXPlugPtr    schemaPlug;       // plugged schema validation layer

        std::list<std::string> errors;           // schema validity errors
        std::list<std::string> warnings;         // schema validity warnings

        XmlOptions options;

        // is set to 'this' pointer
        void *user_data;
	

    public:
	
	struct StackEl {
            xmlbeansxx::XmlObjectPtr obj;
	    std::string str;
    	    bool processContents;
	    xmlbeansxx::QName name;
    	    StackEl(XmlObjectPtr o,bool processContents,const QName& n)
        	: obj(o),processContents(processContents),name(n)
        	{}
	};
	std::stack<StackEl> nodesStack;
	std::string currentString;



        LibXMLParser();
        LibXMLParser(const XmlOptions &options);
        virtual ~LibXMLParser();

        // Overrides

        // parse using libxml2 parser
        virtual void parse(std::istream &in, XmlObject &documentRoot);
        virtual void parse(const std::string &in, XmlObject &documentRoot);



        XmlOptions getXmlOptions() const { return options; }

        void setXmlOptions(const XmlOptions &options) {
            this->options = options;
        }

        /** Loads grammars into libxml2 parser from specified file names. */
        void loadGrammars(const std::vector<std::string> &fileNames);

        /** Loads grammar into libxml2 parser from specified file name. */
        void loadGrammar(const std::string &fileName);

        /** Unloads all grammars from libxml2 parser. */
        void unloadGrammars();

        /** converts eg. "xs:string" to <nr,"string">, where nr is namespace ID in globalTypeSystem */
        QName nsSplit(const std::string &str, bool isAttr = false);
        QName getQName(const char *prefix, const char *localname, bool isAttr = false);
        /** converts "xs:string" to <"xs","string"> */
        std::pair<std::string, std::string> tagSplit(const std::string &str);

        // add schema validity error message
        void addError(const char *msg) {
            errors.push_back(std::string(msg));
        }

        // get schema validity error messages
        std::list<std::string> getSchemaValidityErrors() const {
            return errors;
        }

        // add schema validity warning message
        void addWarning(const char *msg) {
            warnings.push_back(std::string(msg));
        }

        // get schema validity warning messages
        std::list<std::string> getSchemaValidityWarnings() const {
            return warnings;
        }

        xmlSAXHandlerPtr saxHandlerPtr;

        XmlContext xmlContext;
//        XmlCursor cursor;
        StoreString xsi_ns;

    private:
        void init();
        std::string generateErrorMessage(xmlErrorPtr);
    };

}

#endif//_XMLBEANSXX_LIBXMLPARSER_H_
