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


#ifndef _XMLBEANSXX_XMLPARSER_H_
#define _XMLBEANSXX_XMLPARSER_H_

#include "BoostAssert.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "XmlOptions.h"


namespace xmlbeansxx {

class XmlObject;

class XmlParser;
typedef boost::shared_ptr<XmlParser> XmlParserPtr;

/** This class is used for parsing of xml documents. It uses some xml parser and optionally can use xml schema validator.  */
class XmlParser {
public:
    static XmlParserPtr create();
    static XmlParserPtr create(const boost::shared_ptr<XmlOptions> &opts);

    virtual ~XmlParser() {}

    /**
     * Parses using some parser an xml document from std::istream to some XmlDocument. 
     * If XmlOptions validation is set, then uses schema validator
     * (apropriate grammars should be loaded using eg. loadGrammar method).
     */
    virtual void parse(std::istream &in, XmlObject *documentRoot) = 0;
    virtual void parse(std::string &doc, XmlObject *documentRoot) = 0;

    virtual boost::shared_ptr<XmlOptions> getXmlOptions() const = 0;
    virtual void setXmlOptions(const boost::shared_ptr<XmlOptions> &options) = 0;

    /** Loads grammars into parser from specified file names. */
    virtual void loadGrammars(const std::vector<std::string> &fileNames) = 0;
    /** Loads grammar into parser from specified file name. */
    virtual void loadGrammar(const std::string fileName) = 0;
    /** Unloads all grammars from parser. */
    virtual void unloadGrammars() = 0;
};
}

#endif//_XMLBEANSXX_XMLPARSER_H_
