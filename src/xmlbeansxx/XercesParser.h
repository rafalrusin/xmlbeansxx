#ifndef XMLBEANSXX_XERCESPARSER_INCLUDED
#define XMLBEANSXX_XERCESPARSER_INCLUDED
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

#include "BoostAssert.h"
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <memory>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>

#include "XmlOptions.h"
#include "XmlContext.h"

#include <log4cxx/logger.h>
#include "XmlParser.h"


namespace xmlbeansxx {

class Transcoder;

class MyEntityResolver: public XERCES_CPP_NAMESPACE::EntityResolver {
private:
    static log4cxx::LoggerPtr LOG;
    
    XERCES_CPP_NAMESPACE::InputSource *resolveEntity(const XMLCh *publicId,const XMLCh *systemId);
};

class XmlObject;
class MyHandler;

/** This class is used for parsing of xml documents. It uses xerces xml parser and optionally can use xerces xml schema validator.  */
class XercesParser: public XmlParser {
private:
    static log4cxx::LoggerPtr LOG;
    
    std::auto_ptr<MyEntityResolver> entityResolver;
    std::auto_ptr<XERCES_CPP_NAMESPACE::SAXParser> sax;
    friend class MyHandler;
    std::auto_ptr<MyHandler> handler;
    std::auto_ptr<xercesc::XMLGrammarPool> grammarPool;

    /** Used to convert from xerces unicode to UTF8 */
    std::auto_ptr<Transcoder> tr;

    struct StackEl {
        XmlObject *obj;
        std::string str;
        bool processContents;
        int restorePosition;
        StackEl(XmlObject *o,bool processContents,int restorePosition)
            : obj(o),processContents(processContents),restorePosition(restorePosition)
            {}
    };

    XmlContext xmlContext;
    std::stack<StackEl> nodesStack;

    int xsi_ns;
    //char mbuf[1024]; //buffer for various things
    std::string currentString;

    boost::shared_ptr<XmlOptions> opts;

public:
    XercesParser();
    XercesParser(const boost::shared_ptr<XmlOptions> &opts);
    virtual ~XercesParser();

    /**
     * Parses using xercesc parser an xml document from std::istream to some XmlDocument. 
     * If XmlOptions validation is set, then uses xercesc schema validator
     * (apropriate grammars should be loaded using eg. loadGrammar method).
     */
    virtual void parse(std::istream &in, XmlObject *documentRoot);

    virtual boost::shared_ptr<XmlOptions> getXmlOptions() const {
        return opts;
    }
    virtual void setXmlOptions(const boost::shared_ptr<XmlOptions> &options) {
        opts=options;
    }

    /** Loads grammars into xercesc parser from specified file names. */
    virtual void loadGrammars(const std::vector<std::string> &fileNames);
    /** Loads grammar into xercesc parser from specified file name. */
    virtual void loadGrammar(const std::string fileName);
    /** Unloads all grammars from xercesc parser. */
    virtual void unloadGrammars();

private:
    void init(bool reinit=false);

    void updateOptions();

    /** converts "xs:string" to <"string",nr>, where nr is namespace name in globalTypeSystem */
    std::pair<int,std::string> nsSplit(const std::string str);
    /** converts "xs:string" to <"xs","string"> */
    std::pair<std::string,std::string> tagSplit(const std::string str);

};

class MyHandler: public XERCES_CPP_NAMESPACE::HandlerBase {
private:
    static log4cxx::LoggerPtr LOG;
    XercesParser *p;
public:
    MyHandler(XercesParser *p);
    void resetDocument();
    void startElement(const XMLCh* const name, XERCES_CPP_NAMESPACE::AttributeList& attributes);
    void characters(const XMLCh* const chars, const unsigned int length);
    void endElement(const XMLCh* const name);
    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);

    std::string getErrorStr(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void error(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void resetErrors();

private:
    void updateValidation();

};

}
#endif //XMLBEANSXX_XERCESPARSER_INCLUDED
