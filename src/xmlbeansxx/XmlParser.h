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


#ifndef _XMLBEANSXX_XMLPARSER_H_
#define _XMLBEANSXX_XMLPARSER_H_

#include "BoostAssert.h"
#include <string>
#include <vector>
#include <list>

#include "XmlOptions.h"
#include "XmlContext.h"
#include "QName.h"
#include "XmlTypesGen.h"

namespace xmlbeansxx {

class XmlObject;

/** This class is used for parsing of xml documents. It uses some xml parser and optionally can use xml schema validator.  */

class XmlParser;
typedef boost::shared_ptr<XmlParser> XmlParserPtr;

class XmlParser {
public:
	virtual ~XmlParser() {}

    	/**
    	 * Parses using some parser an xml document from std::istream to some XmlDocument. 
    	 * If XmlOptions validation is set, then uses schema validator
    	 * (apropriate grammars should be loaded using eg. loadGrammar method).
    	 */
    	virtual void parse(std::istream      &in, XmlObject &documentRoot) = 0;
    	virtual void parse(const std::string &in, XmlObject &documentRoot) = 0;

    	virtual XmlOptions getXmlOptions() const = 0;
    	virtual void setXmlOptions(const XmlOptions &options) = 0;

    	/** Loads grammars into parser from specified file names. */
    	virtual void loadGrammars(const std::vector<std::string> &fileNames) = 0;
    	/** Loads grammar into parser from specified file name. */
    	virtual void loadGrammar(const std::string &fileName) = 0;
    	/** Unloads all grammars from parser. */
    	virtual void unloadGrammars() = 0;
    
	class Factory {
	public:
		static XmlParserPtr newInstance();
		static XmlParserPtr newInstance(const XmlOptions opts);
	};

};

class EmptyParser : public XmlParser {
protected:
        std::list<std::string> errors;           // schema validity errors
        std::list<std::string> warnings;         // schema validity warnings
        XmlOptions options;

    public:
        XmlContext xmlContext;
	
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



        EmptyParser() {};
        EmptyParser(const XmlOptions &options): options(options){};
        virtual ~EmptyParser() {};

        // Overrides


        virtual XmlOptions getXmlOptions() const { return options; }

        virtual void setXmlOptions(const XmlOptions &options) {
            this->options = options;
        }


        /** converts eg. "xs:string" to <nr,"string">, where nr is namespace ID in globalTypeSystem */
        virtual QName nsSplit(const std::string &str, bool isAttr = false);
        virtual QName getQName(const char *prefix, const char *localname, bool isAttr = false);
        /** converts "xs:string" to <"xs","string"> */
        virtual std::pair<std::string, std::string> tagSplit(const std::string &str);

        // add schema validity error message
        virtual void addError(const char *msg) {
            errors.push_back(std::string(msg));
        }

        // get schema validity error messages
        virtual std::list<std::string> getSchemaValidityErrors() const {
            return errors;
        }

        // add schema validity warning message
        virtual void addWarning(const char *msg) {
            warnings.push_back(std::string(msg));
        }

        // get schema validity warning messages
        virtual std::list<std::string> getSchemaValidityWarnings() const {
            return warnings;
        }



};







}

#endif//_XMLBEANSXX_XMLPARSER_H_
