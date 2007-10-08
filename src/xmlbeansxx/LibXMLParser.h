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
     /*
class LibXMLParser : public EmptyParser {
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


public:

        LibXMLParser();
        LibXMLParser(const XmlOptions &options);
        virtual ~LibXMLParser();

        // Overrides

        // parse using libxml2 parser
        virtual void parse(std::istream &in, XmlObject &documentRoot);
        virtual void parse(const std::string &in, XmlObject &documentRoot);


        /// Loads grammars into libxml2 parser from specified file names. 
        void loadGrammars(const std::vector<std::string> &fileNames);

        /// Loads grammar into libxml2 parser from specified file name. 
        void loadGrammar(const std::string &fileName);

        /// Unloads all grammars from libxml2 parser. 
        void unloadGrammars();


        xmlSAXHandlerPtr saxHandlerPtr;


private:
        void init();
        std::string generateErrorMessage(xmlErrorPtr);
};

*/

}
#endif//_XMLBEANSXX_LIBXMLPARSER_H_
