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
#include "XmlCursor.h"


namespace xmlbeansxx {

class Transcoder;

class MyEntityResolver: public XERCES_CPP_NAMESPACE::EntityResolver {
private:
    static log4cxx::LoggerPtr LOG;

    XERCES_CPP_NAMESPACE::InputSource *resolveEntity(const XMLCh *publicId,const XMLCh *systemId);
};

class XmlObject;
class XercesDocumentHandler;
class XercesErrorHandler;

/** This class is used for parsing of xml documents. It uses xerces xml parser and optionally can use xerces xml schema validator.  */
class XercesParser_I: public XmlParser_I {
private:
    static log4cxx::LoggerPtr LOG;

    std::auto_ptr<MyEntityResolver> entityResolver;
    std::auto_ptr<XERCES_CPP_NAMESPACE::SAXParser> sax;
    friend class XercesDocumentHandler;
    std::auto_ptr<XercesDocumentHandler> documentHandler;
    std::auto_ptr<XercesErrorHandler> errorHandler;
    std::auto_ptr<xercesc::XMLGrammarPool> grammarPool;

    /** Used to convert from xerces unicode to UTF8 */
    std::auto_ptr<Transcoder> tr;

    XmlContext xmlContext;
    XmlCursor cursor;

    StoreString xsi_ns;

    XmlOptions opts;

public:
    XercesParser_I();
    XercesParser_I(const XmlOptions &opts);
    virtual ~XercesParser_I();

    /**
     * Parses using xercesc parser an xml document from std::istream to some XmlDocument. 
     * If XmlOptions validation is set, then uses xercesc schema validator
     * (apropriate grammars should be loaded using eg. loadGrammar method).
     */
    virtual void parse(std::istream &in, const XmlObject &documentRoot);
    virtual void parse(const String &in, const XmlObject &documentRoot);

    virtual XmlOptions getXmlOptions() const {
        return opts;
    }
    virtual void setXmlOptions(const XmlOptions &options) {
        opts=options;
    }

    /** Loads grammars into xercesc parser from specified file names. */
    virtual void loadGrammars(const std::vector<String> &fileNames);
    /** Loads grammar into xercesc parser from specified file name. */
    virtual void loadGrammar(const String &fileName);
    /** Unloads all grammars from xercesc parser. */
    virtual void unloadGrammars();

private:
    void init(bool reinit=false);

    void updateOptions();

    /** converts eg. "xs:string" to <nr,"string">, where nr is namespace ID in globalTypeSystem */
    QName nsSplit(const String &str, bool isAttr = false);
    /** converts "xs:string" to <"xs","string"> */
    std::pair<String, String> tagSplit(const String &str);

};

BEGIN_CLASS(XercesParser, XercesParser_I)
END_CLASS()

class XercesDocumentHandler: public XERCES_CPP_NAMESPACE::DocumentHandler {
private:
    static log4cxx::LoggerPtr LOG;
    XercesParser_I *p;
public:
    XercesDocumentHandler(XercesParser_I *p);
    void startDocument();
    void resetDocument();
    void endDocument();
    void processingInstruction(const XMLCh *a, const XMLCh *b);
    void setDocumentLocator(const XERCES_CPP_NAMESPACE::Locator *locator);
    void startElement(const XMLCh* const name, XERCES_CPP_NAMESPACE::AttributeList& attributes);
    void characters(const XMLCh* const chars, const unsigned int length);
    void endElement(const XMLCh* const name);
    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
};

class XercesErrorHandler: public XERCES_CPP_NAMESPACE::ErrorHandler {
private:
    static log4cxx::LoggerPtr LOG;
    Transcoder *tr;
public:
    XercesErrorHandler(Transcoder *tr);

    String getErrorStr(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void error(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    void resetErrors();
};

}
#endif //XMLBEANSXX_XERCESPARSER_INCLUDED
