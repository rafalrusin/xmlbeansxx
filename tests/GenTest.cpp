#include "GenTest.h"
#include <string>
#include <xmlbeansxx/XmlParser.h>
#include <xmlbeansxx/XmlCursor.h>
#include <xmlbeansxx/defs.h>

#include <log4cxx/logger.h>
//#include <log4cxx/stream.h>
#include "c.h"

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( GenTest );

void namespaceTests();
void parsing();

log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger(xmlbeansxx::String("test.GenTest"));
//log4cxx::logstream LOG_DEBUG(log4cxx::Logger::getLogger(String("test.GenTest")), log4cxx::Level::DEBUG);

void GenTest::genTest() {
    try {
        LOG4CXX_DEBUG(LOG, "genTest before namespaceTests");
        namespaceTests();
        LOG4CXX_DEBUG(LOG, "genTest before parsing");
        parsing();
    } catch (xmlbeansxx::BeansException &e) {
        LOG4CXX_ERROR(LOG,"Exception: " + xmlbeansxx::String(e.what()));
        throw;
    }
}


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;

void namespaceTests() {
    //xml tree creation using generated classes from c.xsd
    ContentDocument root(ContentDocument::Factory::newInstance());
    //root->cgetContent()->set
    ContentType ct(ContentType::Factory::newInstance());
    root->setContent(ct);
    Fullpersoninfo p(Fullpersoninfo::Factory::newInstance());
    ct->setEmployee(p);
    p->setCity(77);
    LOG4CXX_DEBUG(LOG, "1");
    XmlString s = XmlString::Factory::newInstance();
    LOG4CXX_DEBUG(LOG, "2");
    s->setStringValue("_addr_");
    p->xsetAddress(s);
    p->xsetLastname(newInstance<CdataString>("_ln<>_"));
    p->xsetFirstname(newInstance<XmlString>("_fn_"));
    LOG4CXX_DEBUG(LOG, "3");
    p->xsetDt(newInstance<XmlDateTime>("\n\n 2004-01-30T22:50:11  "));
    LOG4CXX_DEBUG(LOG, "3.1");
    Array<XmlString> tab=p->xgetTableArray();
    LOG4CXX_DEBUG(LOG, "3.2");
    tab.push_back(XmlString::Factory::newInstance());
    tab.push_back(XmlString::Factory::newInstance());
    p->xsetTableArray(tab);
    p->addNewTable();
    p->setTableArray(3, "mm");
    LOG4CXX_DEBUG(LOG, "4");

    CPPUNIT_ASSERT(p->sizeOfTable()==4);
    p->removeTable(2);
    CPPUNIT_ASSERT(p->sizeOfTable()==3);
    CPPUNIT_ASSERT(p->getTableArray(2)==String("mm"));
    CPPUNIT_ASSERT(p->xgetTableArray(20) == Null());
    LOG4CXX_DEBUG(LOG, "5");


    p->xsetCar(newInstance<CarType>("2"));
    p->xsetCarAttr(newInstance<CarType>("3"));

    p->xsetAgeAttr(newInstance<XmlInt>("20"));
    p->xsetMoneyAttr(newInstance<AmountType>("10.256789"));
    //TODO CPPUNIT_ASSERT(p->xgetMoneyAttr()->getStringValue() == String("10.26"));
    LOG4CXX_DEBUG(LOG, "6");
    p->xsetMoneyAttr(newInstance<AmountType>("10.254789"));
    //TODO CPPUNIT_ASSERT(p->xgetMoneyAttr()->getStringValue() == String("10.25"));
    //root->serialize(cout);
    p->setDane(DaneType::Factory::newInstance());
    LOG4CXX_DEBUG(LOG,String("created:\n") + root->toString() + "\n~created\n");
    //LOG4CXX_DEBUG(LOG,root->digest());

    LOG4CXX_DEBUG(LOG, "7");
    String rootStr = root->toString();
    LOG4CXX_DEBUG(LOG, "To parse: " << rootStr);

    ContentDocument root2 = ContentDocument::Factory::newInstance();
    {
        XmlParser p(XmlParser::create());
        p->loadGrammar("c.xsd");
        p->getXmlOptions()->setValidation(true);
    
        istringstream in(rootStr);
        p->parse(in, root2);
    }
    CPPUNIT_ASSERT(root2->getContent()->getEmployee()->getLastname() == String("_ln<>_"));
    LOG4CXX_DEBUG(LOG, "8");
}

void parsing() {
    XmlParser p(XmlParser::create());
    p->loadGrammar("c.xsd");
    p->getXmlOptions()->setValidation(true);

    /*
    {
        ifstream in("b.xml");
        LOG4CXX_DEBUG(LOG, "parsing b.xml");
        ContentDocumentPtr doc=ContentDocument::Factory::newInstance();
        try {
            p->parse(in,doc.get());
      LOG4CXX_INFO(LOG,"b.xml parse passed - error");
            CPPUNIT_ASSERT(false);
        } catch (BeansException &ex) {
      LOG4CXX_INFO(LOG,String("Exception: ") + ex.getMessage());
        }
    }
    */

    ContentDocument doc;
    {
        ifstream in("c.xml");
        LOG4CXX_DEBUG(LOG, "parsing c.xml");
        ContentDocument docC = ContentDocument::Factory::newInstance();
        try {
            p->parse(in,docC);
        } catch (BeansException &ex) {
            LOG4CXX_INFO(LOG, String("Exception: ") + ex.getMessage());
            CPPUNIT_ASSERT(false);
        }

        doc = docC;
    }

    //ifstream in("a.xml");
    //LOG4CXX_DEBUG(LOG, "parsing a.xml");
    {
        /*
          for (int i = 0; i<1000; i++) {
              XmlParser *p2 = new XmlParser();
              p2->loadGrammar("c.xsd");
              //delete p2;
          }
          String l;
          std::cin>>l;
        */
    }

    //    ContentDocumentPtr doc=ContentDocument::Factory::newInstance();
    //p->parse(in,doc.get());
    VAL(age, doc->getContent()->getEmployee()->getAgeAttr());
    LOG4CXX_DEBUG(LOG, "age: " << age);
    LOG4CXX_DEBUG(LOG,doc->toString());
    CPPUNIT_ASSERT( age == 10);

    XmlCursor cursor =  doc->getContent()->getEmployee()->newCursor();
    cursor->beginElement(QName("", "dane"));
    cursor->beginElement(QName("", "mojedane"));
    cursor->insertChars("abcdef");
    cursor->dispose();
    LOG4CXX_DEBUG(LOG,doc->toString());

    Personinfo pi=doc->getContent()->getEmployee();
    LOG4CXX_DEBUG(LOG,"got pi:" + pi->toString());
    /*
    //AssertionFailedException is not thrown
    try {
        String v = pi->xgetMoneyAttr()->toString();
        LOG4CXX_DEBUG(LOG,v);
        LOG4CXX_DEBUG(LOG,"*NULL passed");
        CPPUNIT_ASSERT(false);
    } catch (BeansException e) {
        LOG4CXX_DEBUG(LOG,e.getMessage());
    }*/

    LOG4CXX_DEBUG(LOG,String("cdata firstname:") + doc->getContent()->getEmployee()->getFirstname());
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getFirstname() == String("Name1"));

    LOG4CXX_DEBUG(LOG,"--1--");
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getChoice()->getB() == 20);
    LOG4CXX_DEBUG(LOG,"--2--");
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getChoice()->xgetB()->getStringValue() == "20");
    //LOG_DEBUG<<"--3--"<<doc->getContent()->getEmployee()->getChoice()->getA()<<LOG4CXX_ENDMSG;
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getChoice()->xgetA() == Null());

    //LOG_DEBUG << doc->getContent()->getEmployee()->getDefault() << LOG4CXX_ENDMSG;
    //TODO CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDefault() == 101);
    //TODO CPPUNIT_ASSERT(doc->getContent()->getEmployee()->xgetDefault()->getStringValue() == "101");

    /* test float and double */
    LOG4CXX_DEBUG(LOG, doc->getContent()->getEmployee()->getFloatElement());
    LOG4CXX_DEBUG(LOG, doc->getContent()->getEmployee()->getDoubleElement());
    doc->getContent()->getEmployee()->setFloatElement(2.71);
    doc->getContent()->getEmployee()->setDoubleElement(2e71);
    LOG4CXX_DEBUG(LOG, doc->getContent()->getEmployee()->getFloatElement());
    LOG4CXX_DEBUG(LOG, doc->getContent()->getEmployee()->getDoubleElement());


    /*
    TODO uncomment
    String d1, d2;
    doc->getContent()->getEmployee()->setAgeAttr(1000);
    d1 = doc->digest();
    doc->getContent()->getEmployee()->setAgeAttr(2000);
    d2 = doc->digest();
    CPPUNIT_ASSERT(d1 != d2);
    */

    //CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDef() == String("_def_"));
    //LOG_DEBUG << "doc serialized: " << doc->toString().c_str() << LOG4CXX_ENDMSG;
    //doc->getContent()->getEmployee()->setDef("mydef");
    //CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDef() == String("mydef"));

    //LOG_DEBUG << "docC serialized: " << doc->toString().c_str() << LOG4CXX_ENDMSG;
    //CPPUNIT_ASSERT(docC->getContent()->getEmployee()->getDef() == String("myval"));
}

