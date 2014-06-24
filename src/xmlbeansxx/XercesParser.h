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


#ifndef _XMLBEANSXX_XERCESPARSER_H_
#define _XMLBEANSXX_XERCESPARSER_H_

#include "BoostAssert.h"
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <memory>

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>

#include "XmlOptions.h"
#include "XmlContext.h"
#include "XmlParser.h"
#include "XercesUtils.h"


#include "logger.h"



namespace xmlbeansxx {

class Transcoder;


class XmlObject;
class MySAX2Handler;

class XercesParser : public EmptyParser{
private:
    XMLBEANSXX_STATIC_LOGGER_PTR(log);

    std::auto_ptr<XERCES_CPP_NAMESPACE::SAX2XMLReader> sax2;
    friend class MySAX2Handler;
    std::auto_ptr<MySAX2Handler> handler;
    std::auto_ptr<xercesc::XMLGrammarPool> grammarPool;
    Transcoder transcoder;


public:
    XercesParser();
    XercesParser(const XmlOptions & opts);
    virtual ~XercesParser();

    virtual void parse(std::istream      &in, XmlObject &documentRoot);
    virtual void parse(const std::string &in, XmlObject &documentRoot);


    /** Loads grammars into memory from specified file names */
    virtual void loadGrammars(const std::vector<std::string> &fileNames);
    /** Loads grammar into memory from specified file name */
    virtual void loadGrammar(const std::string &fileName);
    /** Unloads all grammars from memory */
    virtual void unloadGrammars();

private:
    void init(bool reinit=false);

    void updateOptions();

};

class MySAX2Handler: public XERCES_CPP_NAMESPACE::DefaultHandler {
private:
    XMLBEANSXX_STATIC_LOGGER_PTR(log);
    XercesParser *parser;
public:
    MySAX2Handler(XercesParser *p);
    virtual void startElement(	const XMLCh* const uri,
    				const XMLCh* const localname,
				const XMLCh* const qname,
				const XERCES_CPP_NAMESPACE::Attributes& attrs);

    virtual void startPrefixMapping(const XMLCh* const prefix, const XMLCh* const uri);
#if (XERCES_VERSION_MAJOR == 3)
    virtual void characters(const XMLCh* const chars, const XMLSize_t length);
#else
    virtual void characters(const XMLCh* const chars, const unsigned int length);
#endif
    virtual void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);

    virtual void warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    virtual void error(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
    virtual void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc);

    virtual std::string transcode(const XMLCh* const chars,const int length = -1);

private:
    void updateValidation();
    std::string getErrorStr(const XERCES_CPP_NAMESPACE::SAXParseException& e);
};

}

#endif//_XMLBEANSXX_XERCESPARSER_H_
