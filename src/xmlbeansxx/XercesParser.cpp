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

#include "XercesParser.h"
#include <log4cxx/logger.h>

#include "XercesUtils.h"
#include "defs.h"
#include "XmlBeans.h"
#include "XmlException.h"
#include "XmlTypesGen.h"
#include "Tracer.h"

#include "BoostAssert.h"
#include <boost/scoped_array.hpp>

#include <boost/config.hpp>
#include <set>

#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>

namespace xmlbeansxx {
using namespace std;
//------------------------------------------------------------------------------------
//Xml Parser

#define ENCODING "utf8"
#define TRACER2(a,b) 
#define LOG4CXX_DEBUG2(a,b)


log4cxx::LoggerPtr XercesParser_I::LOG = log4cxx::Logger::getLogger(String("xmlbeansxx.XercesParser"));
log4cxx::LoggerPtr MyEntityResolver::LOG = log4cxx::Logger::getLogger(String("xmlbeansxx.MyEntityResolver"));
log4cxx::LoggerPtr XercesDocumentHandler::LOG = log4cxx::Logger::getLogger(String("xmlbeansxx.XercesDocumentHandler"));
log4cxx::LoggerPtr XercesErrorHandler::LOG = log4cxx::Logger::getLogger(String("xmlbeansxx.XercesErrorHandler"));

XERCES_CPP_NAMESPACE_USE

InputSource *MyEntityResolver::resolveEntity(const XMLCh *publicId,const XMLCh *systemId) {
    TRACER(LOG,"resolveEntity")

    Transcoder tr(ENCODING);
    String pub(tr.transcode(publicId));
    String sys(tr.transcode(systemId));
    LOG4CXX_DEBUG(LOG,String("Resolving grammar of publicId: ")+pub+" systemId: "+sys);
    return NULL;
}


XercesDocumentHandler::XercesDocumentHandler(XercesParser_I *p): p(p) {}

void XercesDocumentHandler::startDocument() {}
void XercesDocumentHandler::resetDocument() {}
void XercesDocumentHandler::endDocument() {}
void XercesDocumentHandler::processingInstruction(const XMLCh *a, const XMLCh *b) {}
void XercesDocumentHandler::setDocumentLocator(const XERCES_CPP_NAMESPACE::Locator *locator) {}


void XercesDocumentHandler::startElement(const XMLCh* const oname, AttributeList& oatts) {
    LocalAttributeList latts(oatts,p->tr.get());
    int latts_size=latts.getLength();
    p->xmlContext.remember();

    {
        boost::scoped_array<bool> skipped(new bool[latts_size]);

        LOG4CXX_DEBUG2(LOG,String("Pass 1 - xmlns attributes "));
        FOR(i,latts_size) {
            skipped[i]=false;
            pair<string,string> tag=p->tagSplit(latts.getName(i));
            if (tag.first=="")
                swap(tag.first,tag.second);
            if (tag.first=="xmlns") {
                //cout<<"set ns link: "<<(p->mbuf+y)<<" -> "<<latts[i+1].getLocalStr()<<"\n";
                StoreString ns(latts.getValue(i));
                p->xmlContext.setLink(tag.second,ns);
                skipped[i]=true;
            }
        }
    
        p->cursor->beginElement(p->nsSplit(p->tr->transcode(oname)));
        
        LOG4CXX_DEBUG2(LOG,String("Pass 3 - adding attributes to object"));
        //adding attributes to new object
        {
            FOR(i,latts_size) {
                if (skipped[i])
                    continue;
                VAL(name, p->nsSplit(latts.getName(i), true));
                if (name == XmlBeans::xsi_type()) {
                    p->cursor->insertAttributeWithValue(name, p->nsSplit(latts.getValue(i)));
                } else {
                    p->cursor->insertAttributeWithValue(name, latts.getValue(i));
                }
            }
        }
    }
}
void XercesDocumentHandler::characters(const XMLCh* const chars, const unsigned int length) {
    TRACER2(LOG,"characters")
    boost::scoped_array<XMLCh> chars2(new XMLCh[length+1]);
    FOR(i,(int)length) chars2[i]=chars[i];
    chars2[length]=0;
    String s(p->tr->transcode(chars2.get()));
    p->cursor->insertChars(s);
}
void XercesDocumentHandler::endElement(const XMLCh *const name) {
    //return;
    TRACER2(LOG,"endElement")
    //LOG4CXX_DEBUG2(LOG,String("XercesDocumentHandler::endElement - start"));
    p->cursor->pop();
    p->xmlContext.restore();
    //LOG4CXX_DEBUG2(LOG,String("XercesDocumentHandler::endElement - finish"));
}
void XercesDocumentHandler::ignorableWhitespace(const XMLCh* const chars, const unsigned int length) {}

//---
XercesErrorHandler::XercesErrorHandler(Transcoder *tr): tr(tr) {}

string XercesErrorHandler::getErrorStr(const SAXParseException& e) {
    ostringstream ss;
    ss << "At line " << e.getLineNumber()
    << ", char " << e.getColumnNumber()
    << ", " << tr->transcode(e.getMessage());
    return ss.str();
}
void XercesErrorHandler::warning(const SAXParseException& exc) {
    //TODO: log warning
    //LOG4CXX_INFO(log,String("XercesDocumentHandler::warning() - Xerces-c warning: ")+getErrorStr(exc));
    throw XmlParseException(string("Xerces-c warning: ")+getErrorStr(exc));
}
void XercesErrorHandler::error(const SAXParseException& exc) {
    throw XmlParseException(string("Xerces-c error: ")+getErrorStr(exc));
}
void XercesErrorHandler::fatalError(const SAXParseException& exc) {
    throw XmlParseException(string("Xerces-c fatal error: ")+getErrorStr(exc));
}
void XercesErrorHandler::resetErrors() { }

//---
XercesParser_I::XercesParser_I():tr(new Transcoder(ENCODING)),opts(XmlOptions::New()) {
    init();
}

XercesParser_I::XercesParser_I(const XmlOptions &opts):tr(new Transcoder(ENCODING)),opts(opts) {
    init();
}

namespace {
    xercesc::XMLGrammarPool *getGrammarPool() {
        static xercesc::XMLGrammarPoolImpl *gp = new xercesc::XMLGrammarPoolImpl(xercesc::XMLPlatformUtils::fgMemoryManager);
        return gp;
    }
    xercesc::XMLGrammarPool *__glob_gp = getGrammarPool();

#ifdef BOOST_HAS_THREADS
    boost::recursive_mutex &getGrammarPoolMutex() {
        static boost::recursive_mutex m;
        return m;
    }
    boost::recursive_mutex &__glob_gpm = getGrammarPoolMutex();
#endif

    std::set<String> &getGrammarPoolSet() {
        static std::set<String> s;
        return s;
    }
        
}

void XercesParser_I::init(bool reinit) {
    LOG4CXX_DEBUG(LOG,"start");
    xsi_ns = XmlBeans::xsi_ns();
    /*if (initialized==false)
        throw XmlException("XmlBeans++ not initialized. Use org::apache::xmlbeans::initializeBeans()");*/

    entityResolver=auto_ptr<MyEntityResolver>(new MyEntityResolver());
    //TODO: uncomment while xerces parallel validation bug is fixed
    //sax=auto_ptr<SAXParser>(new SAXParser(0,XMLPlatformUtils::fgMemoryManager,getGrammarPool()));
    if (reinit==false) {
        grammarPool=auto_ptr<xercesc::XMLGrammarPool>(new xercesc::XMLGrammarPoolImpl(xercesc::XMLPlatformUtils::fgMemoryManager));
    }
    sax=auto_ptr<SAXParser>(new SAXParser(0,XMLPlatformUtils::fgMemoryManager,grammarPool.get()));
    //~
    documentHandler=auto_ptr<XercesDocumentHandler>(new XercesDocumentHandler(this));
    errorHandler=auto_ptr<XercesErrorHandler>(new XercesErrorHandler(tr.get()));
    sax->setEntityResolver(entityResolver.get());
    sax->setDocumentHandler(documentHandler.get());
    sax->setErrorHandler(errorHandler.get());
    sax->setDoNamespaces(true);
    sax->cacheGrammarFromParse(true);
    sax->useCachedGrammarInParse(true);
    LOG4CXX_DEBUG(LOG,"finish");
}

XercesParser_I::~XercesParser_I() {}

void XercesParser_I::updateOptions() {
    sax->setValidationScheme(opts->getValidation()?SAXParser::Val_Always:SAXParser::Val_Never);
    sax->setDoSchema(opts->getValidation());
    sax->setValidationSchemaFullChecking(opts->getValidation());
}

void XercesParser_I::parse(istream &in, const XmlObject &root) {
    TRACER(LOG,"parse")
    LOG4CXX_DEBUG(LOG,String("XercesParser::parse() - start"));
    updateOptions();
    
    cursor = root->newCursor();
    sax->resetDocument();
    StdInputSource s(in);
    try {
        sax->parse(s);
    } catch (...) {
        init(true); //TODO: remove this when xerces-c validation bug is fixed
        cursor = Null();
        throw;
    }
    cursor = Null();
    
    LOG4CXX_DEBUG(LOG,String("XercesParser::parse() - finish"));
}

QName XercesParser_I::nsSplit(const String &str, bool isAttr) {
    string::size_type pos=str.find(':');
    if (pos == string::npos) {
        if (isAttr) {
            return QName("", str);
        } else {
            return QName(xmlContext.getLink(""), str);
        }
    } else {
        return QName(xmlContext.getLink(str.substr(0,pos)) ,str.substr(pos+1));
    }
}

std::pair<String, String> XercesParser_I::tagSplit(const String &str) {
    String::size_type pos = str.find(':');
    if (pos==string::npos) {
        return pair<String, String>("",str);
    } else {
        return pair<String, String>(str.substr(0,pos),str.substr(pos+1));
    }
}


void XercesParser_I::loadGrammars(const std::vector<String> &fileNames) {
    FOREACH(it,fileNames) {
        loadGrammar(*it);
    }
}

/*
//TODO: Uncomment while xerces-c parallel parsing bug is fixed
void XercesParser::loadGrammar(const String fileName) {
#ifdef BOOST_HAS_THREADS
    boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(getGrammarPoolMutex());
    LOG4CXX_DEBUG(LOG,String("locked grammar pool"));
#endif
    if (getGrammarPoolSet().count(fileName)==0) {
        sax->loadGrammar(fileName.c_str(),Grammar::SchemaGrammarType,true);
        getGrammarPoolSet().insert(fileName);
    } else {
        LOG4CXX_DEBUG(LOG,"Taking grammar "<<fileName<<" from cache");
    }
}

void XercesParser::unloadGrammars() {
#ifdef BOOST_HAS_THREADS
    boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(getGrammarPoolMutex());
    LOG4CXX_DEBUG(LOG,String("locked grammar pool"));
#endif
    sax->resetCachedGrammarPool();
    getGrammarPoolSet().clear();
}
*/

void XercesParser_I::loadGrammar(const String &fileName) {
    sax->loadGrammar(fileName.c_str(),Grammar::SchemaGrammarType,true);
}
void XercesParser_I::unloadGrammars() {
    sax->resetCachedGrammarPool();
}

}

