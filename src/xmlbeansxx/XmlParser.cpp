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


#include "XmlParser.h"
#include "XercesUtils.h"
#include "defs.h"
#include "TypeSystem.h"
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


log4cxx::LoggerPtr XmlParser::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.XmlParser"));
log4cxx::LoggerPtr MyEntityResolver::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.MyEntityResolver"));
log4cxx::LoggerPtr MyHandler::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.MyHandler"));

XERCES_CPP_NAMESPACE_USE

InputSource *MyEntityResolver::resolveEntity(const XMLCh *publicId,const XMLCh *systemId) {
    TRACER(log,"resolveEntity")

    Transcoder tr(ENCODING);
    std::string pub(tr.transcode(publicId));
    std::string sys(tr.transcode(systemId));
    LOG4CXX_DEBUG(log,std::string("Resolving grammar of publicId: ")+pub+" systemId: "+sys);
    return NULL;
}


MyHandler::MyHandler(XmlParser *p): p(p) {}
void MyHandler::resetDocument() {}
void MyHandler::startElement(const XMLCh* const oname, AttributeList& oatts) {
    std::string name(p->tr->transcode(oname));
    TRACER2(log,std::string("startElement(")+string(name)+")")
    //cout<<"start el "<<name<<"\n";
    LocalAttributeList latts(oatts,p->tr.get());
    int latts_size=latts.getLength();
    int restorePosition=p->xmlContext.restorePosition();

    if (p->nodesStack.top().processContents==false) {
        LOG4CXX_DEBUG2(log,std::string("Contents not processed"));
        XmlObjectPtr n(new XmlObject());

        FOR(i,latts_size) {
            n->contents.appendAttr(latts.getName(i),latts.getValue(i));
        }

        p->nodesStack.top().obj->contents.appendElem(name,n);
        p->nodesStack.push(XmlParser::StackEl(n.get(),false,restorePosition));
    } else {
        name=p->tagSplit(name).second;

        boost::scoped_array<bool> skipped(new bool[latts_size]);

        LOG4CXX_DEBUG2(log,std::string("Pass 1 - xmlns attributes "));
        FOR(i,latts_size) {
            skipped[i]=false;
            pair<string,string> tag=p->tagSplit(latts.getName(i));
            if (tag.first=="")
                swap(tag.first,tag.second);
            if (tag.first=="xmlns") {
                //cout<<"set ns link: "<<(p->mbuf+y)<<" -> "<<latts[i+1].getLocalStr()<<"\n";
                int ns=globalTypeSystem()->getNamespace(latts.getValue(i));
                if (ns==-1 && p->opts->getTypedParsing())
                    throw XmlException(string("Namespace '")+string(latts.getValue(i))+string("' is not registered in globalTypeSystem."));
                if (ns==-1)
                    ns=-2;
                p->xmlContext.setLink(tag.second,ns);
                skipped[i]=true;
            }
        }

        LOG4CXX_DEBUG2(log,std::string("Pass 2 - xsi attributes "));
        //Creating object for this element

        XmlObjectPtr n;
        bool isArray=false;

        FOR(i,latts_size) {
            if (skipped[i])
                continue;
            pair<int,string> tag=p->nsSplit(latts.getName(i));
            
            /*
            if (tag.first==-1) {
                throw XmlException(string("Undefined namespace link '")+string(latts.getName(i))+string("' in element '")+string(name)+string("'"));
            }*/
            
            if (tag.first==p->xsi_ns) {
                skipped[i]=true;
                //LOG4CXX_DEBUG2(log,"xsi_ns:'%s'",p->mbuf+u);

                if (tag.second=="type") {
                    pair<int,string> tag2=p->nsSplit(latts.getValue(i));
                    if (tag2.first==-1)
                        throw XmlException(string("Undefined namespace link '")+string(latts.getValue(i))+string("' in type name, in element '")+string(name)+string("'"));
                    //log.debug("MyHandler::startElement() 1.");

                    //log.debug("MyHandler::startElement() 2.");

                    if (!isArray) {
                        LOG4CXX_DEBUG2(log,std::string("createByName.."));
                        n=globalTypeSystem()->createByName(tag2.second,tag2.first);
                        LOG4CXX_DEBUG2(log,std::string("createByName..returned"));

                        if (n==NULL)
                            throw XmlException(string("Xsd Type '")+string(latts.getValue(i))+string("' not defined in builtin type system"));
                    }
                } else if (tag.second=="array") {
                    pair<int,string> tag2=p->nsSplit(latts.getValue(i));
                    if (tag2.first==-1)
                        throw XmlException(string("Undefined namespace link '")+string(latts.getValue(i))+string("' in type name, in element '")+string(name)+string("'"));

                    isArray=true;
                    LOG4CXX_DEBUG2(log,std::string("createArrayByName.."));
                    n=globalTypeSystem()->createArrayByName(tag2.second,tag2.first);
                    LOG4CXX_DEBUG2(log,std::string("createArrayByName..returned"));
                    if (n==NULL)
                        throw XmlException(string("Xsd Type '")+string(latts.getValue(i))+string("' not defined in builtin type system"));

                } else if (tag.second=="schemaLocation") {}
                else {
                    throw XmlException(string("Undefined schema instance usage '")+string(latts.getValue(i))+string("' in element ")+string(name));
                }
            }
        }

        //cout<<"start:name:"<<name<<"\n";
        if (n==NULL) {
            LOG4CXX_DEBUG2(log,std::string("createSubObject ")+std::string(name)+std::string(" on object: "+p->nodesStack.top().obj->getSchemaType()->className));
            n=p->nodesStack.top().obj->getSchemaType()->createSubObject(name);
            LOG4CXX_DEBUG2(log,std::string("createSubObject returned"));
            if(n==NULL) {
                LOG4CXX_DEBUG2(log,std::string("createSubObject was NULL"));
                throw XmlException(string("Cannot create subelement '")+std::string(name)+string("' on object of class "+p->nodesStack.top().obj->getSchemaType()->className));
            } else {
                LOG4CXX_DEBUG2(log,std::string("createSubObject was not NULL"));
            }
            //LOG4CXX_DEBUG2(log,std::string("MyHandler::startElement - created type: ")
        }

        //Removing default attributes and elements, they will be added after parsing this element
        n->contents.free();

        LOG4CXX_DEBUG2(log,std::string("Pass 3 - adding attributes to object"));
        //adding attributes to new object
        FOR(i,latts_size) {
            if (skipped[i])
                continue;
            n->contents.appendAttr(latts.getName(i),latts.getValue(i));
        }
        p->nodesStack.top().obj->contents.appendElem(name,n);
        p->nodesStack.push(XmlParser::StackEl(n.get(),n->getSchemaType()->processContents,restorePosition));
    }
}
void MyHandler::characters(const XMLCh* const chars, const unsigned int length) {
    TRACER2(log,"characters")
    boost::scoped_array<XMLCh> chars2(new XMLCh[length+1]);
    FOR(i,(int)length) chars2[i]=chars[i];
    chars2[length]=0;
    std::string s(p->tr->transcode(chars2.get()));
    p->nodesStack.top().str+=s;
}
void MyHandler::endElement(const XMLCh *const name) {
    TRACER2(log,"endElement")
    //LOG4CXX_DEBUG2(log,std::string("MyHandler::endElement - start"));
    XmlParser::StackEl e=p->nodesStack.top();
    e.obj->setSimpleContent(e.str);
    e.obj->contents.insertDefaults(e.obj->getSchemaType());
    p->xmlContext.restore(p->nodesStack.top().restorePosition);
    p->nodesStack.pop();
    //LOG4CXX_DEBUG2(log,std::string("MyHandler::endElement - finish"));
}
void MyHandler::ignorableWhitespace(const XMLCh* const chars, const unsigned int length) {}

//---
string MyHandler::getErrorStr(const SAXParseException& e) {
    ostringstream ss;
    ss << "At line " << e.getLineNumber()
    << ", char " << e.getColumnNumber()
    << ", " << p->tr->transcode(e.getMessage());
    return ss.str();
}
void MyHandler::warning(const SAXParseException& exc) {
    //TODO: log warning
    //LOG4CXX_INFO(log,std::string("MyHandler::warning() - Xerces-c warning: ")+getErrorStr(exc));
    throw XmlParseException(string("Xerces-c warning: ")+getErrorStr(exc));
}
void MyHandler::error(const SAXParseException& exc) {
    throw XmlParseException(string("Xerces-c error: ")+getErrorStr(exc));
}
void MyHandler::fatalError(const SAXParseException& exc) {
    throw XmlParseException(string("Xerces-c fatal error: ")+getErrorStr(exc));
}
void MyHandler::resetErrors() { }

//---
XmlParser::XmlParser():tr(new Transcoder(ENCODING)),opts(new XmlOptions()) {
    init();
}

XmlParser::XmlParser(const boost::shared_ptr<XmlOptions> &opts):tr(new Transcoder(ENCODING)),opts(opts) {
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

    std::set<std::string> &getGrammarPoolSet() {
        static std::set<std::string> s;
        return s;
    }
        
}

void XmlParser::init(bool reinit) {
    LOG4CXX_DEBUG(log,"start");
    xsi_ns=globalTypeSystem()->addNamespace("http://www.w3.org/2001/XMLSchema-instance");
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
    handler=auto_ptr<MyHandler>(new MyHandler(this));
    sax->setEntityResolver(entityResolver.get());
    sax->setDocumentHandler(handler.get());
    sax->setErrorHandler(handler.get());
    sax->setDoNamespaces(true);
    sax->cacheGrammarFromParse(true);
    sax->useCachedGrammarInParse(true);
    LOG4CXX_DEBUG(log,"finish");
}

XmlParser::~XmlParser() {}

void XmlParser::updateOptions() {
    sax->setValidationScheme(opts->getValidation()?SAXParser::Val_Always:SAXParser::Val_Never);
    sax->setDoSchema(opts->getValidation());
    sax->setValidationSchemaFullChecking(opts->getValidation());
}

void XmlParser::parse(istream &in,XmlObject *root) {
    TRACER(log,"parse")
    LOG4CXX_INFO(log,std::string("XmlParser::parse() - start"));
    //SYNC(*(root->mutex()))
#ifdef BOOST_HAS_THREADS
    boost::detail::thread::scoped_lock<boost::recursive_mutex> lock((root->mutex()));
    LOG4CXX_DEBUG(log,std::string("locked"));
#endif
    updateOptions();
    while (!nodesStack.empty())
        nodesStack.pop();
    nodesStack.push(StackEl(root,root->getSchemaType()->processContents,0));

    sax->resetDocument();
    StdInputSource s(in);
    try {
        sax->parse(s);
    } catch (...) {
        init(true); //TODO: remove this when xerces-c validation bug is fixed
        throw;
    }
    nodesStack.pop();
    BOOST_ASSERT(nodesStack.empty());
    LOG4CXX_INFO(log,std::string("XmlParser::parse() - finish"));
}

std::pair<int,std::string> XmlParser::nsSplit(const std::string str) {
    string::size_type pos=str.find(':');
    if (pos==string::npos) {
        return pair<int,string>(xmlContext.getLink(""),str);
    } else {
        return pair<int,string>(xmlContext.getLink(str.substr(0,pos)),str.substr(pos+1));
    }
}

std::pair<std::string,std::string> XmlParser::tagSplit(const std::string str) {
    string::size_type pos=str.find(':');
    if (pos==string::npos) {
        return pair<string,string>("",str);
    } else {
        return pair<string,string>(str.substr(0,pos),str.substr(pos+1));
    }
}


void XmlParser::loadGrammars(const std::vector<std::string> &fileNames) {
    FOREACH(it,fileNames) {
        loadGrammar(*it);
    }
}

/*
//TODO: Uncomment while xerces-c parallel parsing bug is fixed
void XmlParser::loadGrammar(const std::string fileName) {
#ifdef BOOST_HAS_THREADS
    boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(getGrammarPoolMutex());
    LOG4CXX_DEBUG(log,std::string("locked grammar pool"));
#endif
    if (getGrammarPoolSet().count(fileName)==0) {
        sax->loadGrammar(fileName.c_str(),Grammar::SchemaGrammarType,true);
        getGrammarPoolSet().insert(fileName);
    } else {
        LOG4CXX_DEBUG(log,"Taking grammar "<<fileName<<" from cache");
    }
}

void XmlParser::unloadGrammars() {
#ifdef BOOST_HAS_THREADS
    boost::detail::thread::scoped_lock<boost::recursive_mutex> lock(getGrammarPoolMutex());
    LOG4CXX_DEBUG(log,std::string("locked grammar pool"));
#endif
    sax->resetCachedGrammarPool();
    getGrammarPoolSet().clear();
}
*/

void XmlParser::loadGrammar(const std::string fileName) {
    sax->loadGrammar(fileName.c_str(),Grammar::SchemaGrammarType,true);
}
void XmlParser::unloadGrammars() {
    sax->resetCachedGrammarPool();
}

}

