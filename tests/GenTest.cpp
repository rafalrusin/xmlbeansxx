#include "GenTest.h"
#include <string>

#include <log4cxx/logger.h>
//#include <log4cxx/stream.h>
#include "c.h"

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( GenTest );

void namespaceTests();
void parsing();

log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(std::string("test.GenTest"));
//log4cxx::logstream LOG_DEBUG(log4cxx::Logger::getLogger(std::string("test.GenTest")), log4cxx::Level::DEBUG);

void GenTest::genTest() {
  try {
    LOG4CXX_DEBUG(logger, "genTest before namespaceTests");
    namespaceTests();
    LOG4CXX_DEBUG(logger, "genTest before parsing");
    parsing();
  } catch (xmlbeansxx::BeansException &e) {
    LOG4CXX_ERROR(logger,"Exception: "+std::string(e.what()));
    throw;
  }
}


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;

void namespaceTests() {
        //xml tree creation using generated classes from c.xsd
        ContentDocumentPtr root(new ContentDocument());
        //root->cgetContent()->set
        ContentTypePtr ct(new ContentType());
        root->setContent(ct);
        FullpersoninfoPtr p(new Fullpersoninfo());
        ct->setEmployee(p);
        p->setCity(77);
        p->xsetAddress(XmlStringPtr(new XmlString("_addr_")));
        p->xsetLastname(CdataStringPtr(new CdataString("_ln<>_")));
        p->xsetFirstname(XmlStringPtr(new XmlString("_fn_")));
        p->setDt("\n\n 2004-01-30T22:50:11  ");
        boost::shared_ptr<XmlArray<XmlString> > tab=p->dgetTableArray();
        tab->cgetArray(0);
        tab->cgetArray(1);
		p->dsetTableArray(tab);
		p->addNewTable();
		p->setTableArray(3,"mm");
		
		CPPUNIT_ASSERT(p->sizeOfTable()==4);
		p->removeTable(2);
		CPPUNIT_ASSERT(p->sizeOfTable()==3);
		CPPUNIT_ASSERT(p->getTableArray(2)==std::string("mm"));
		CPPUNIT_ASSERT(p->xgetTableArray(20)==NullPtr());
		
		
        p->xsetCar(CarTypePtr(new CarType("2")));
        p->setCarAttr(CarTypePtr(new CarType("3"))->getSimpleContent());

        p->xsetAgeAttr(XmlIntPtr(new XmlInt(20)));
        p->xsetMoneyAttr(AmountTypePtr(new AmountType("10.256789")));
        CPPUNIT_ASSERT(p->xgetMoneyAttr()->getSimpleContent()==std::string("10.26"));
        p->xsetMoneyAttr(AmountTypePtr(new AmountType("10.254789")));
        CPPUNIT_ASSERT(p->xgetMoneyAttr()->getSimpleContent()==std::string("10.25"));
        //root->serialize(cout);
		LOG4CXX_DEBUG(logger,std::string("created:\n") + root->toString() + "\n~created\n");
		LOG4CXX_DEBUG(logger,root->digest());

        std::string rootStr = root->toString();
        LOG4CXX_DEBUG(logger, rootStr);

        ContentDocumentPtr root2 = ContentDocument::Factory::parse(rootStr);
        CPPUNIT_ASSERT(root2->getContent()->getEmployee()->getLastname() == std::string("_ln<>_"));

}

void parsing() {
    XmlParser p;
    p.loadGrammar("c.xsd");
    p.getXmlOptions()->setValidation(true);
    
    /*    
    {
        ifstream in("b.xml");
        LOG4CXX_DEBUG(logger, "parsing b.xml");
        ContentDocumentPtr doc=ContentDocument::Factory::newInstance();
        try {
            p.parse(in,doc.get());
		    LOG4CXX_INFO(logger,"b.xml parse passed - error");
            CPPUNIT_ASSERT(false);
        } catch (BeansException &ex) {
		    LOG4CXX_INFO(logger,std::string("Exception: ") + ex.getMessage());
        }
    }
    */
    
    ContentDocumentPtr doc;
    {
        ifstream in("c.xml");
        LOG4CXX_DEBUG(logger, "parsing c.xml");
        ContentDocumentPtr docC=ContentDocument::Factory::newInstance();
        try {
          p.parse(in,docC.get());
        } catch (BeansException &ex) {
          LOG4CXX_INFO(logger,std::string("Exception: ") + ex.getMessage());
          CPPUNIT_ASSERT(false);
        }
		
        doc = docC;
    }

    //ifstream in("a.xml");
    //LOG4CXX_DEBUG(logger, "parsing a.xml");
    {
      /*
        for (int i = 0; i<1000; i++) {
            XmlParser *p2 = new XmlParser();
            p2->loadGrammar("c.xsd");
            //delete p2;
        }
        std::string l;
        std::cin>>l;
      */
    }

    //    ContentDocumentPtr doc=ContentDocument::Factory::newInstance();
    //p.parse(in,doc.get());
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getAgeAttr()==10);
    LOG4CXX_DEBUG(logger,doc->toString());

    doc->getContent()->getEmployee()->setAnyElement("dane","<mojedane>abcdef</mojedane>");
    LOG4CXX_DEBUG(logger,doc->toString());

    PersoninfoPtr pi=doc->getContent()->getEmployee();
    LOG4CXX_DEBUG(logger,"got pi:" + pi->toString());
    /*
    //AssertionFailedException is not thrown
    try {
        std::string v = pi->xgetMoneyAttr()->toString();
        LOG4CXX_DEBUG(logger,v);
        LOG4CXX_DEBUG(logger,"*NULL passed");
        CPPUNIT_ASSERT(false);
    } catch (BeansException e) {
        LOG4CXX_DEBUG(logger,e.getMessage());
    }*/

    LOG4CXX_DEBUG(logger,std::string("cdata firstname:") + doc->getContent()->getEmployee()->getFirstname());
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getFirstname() == std::string("Name1"));
    
    LOG4CXX_DEBUG(logger,"--1--");
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getChoice()->getB()==20);
    LOG4CXX_DEBUG(logger,"--2--");
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getChoice()->xgetB()->getSimpleContent()=="20");
    //LOG_DEBUG<<"--3--"<<doc->getContent()->getEmployee()->getChoice()->getA()<<LOG4CXX_ENDMSG;
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getChoice()->xgetA()==NULL);
    
    //LOG_DEBUG << doc->getContent()->getEmployee()->getDefault() << LOG4CXX_ENDMSG;
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDefault()==101);
    CPPUNIT_ASSERT(doc->getContent()->getEmployee()->xgetDefault()->getSimpleContent()=="101");

    /* test float and double */
    LOG4CXX_DEBUG(logger, doc->getContent()->getEmployee()->getFloatElement());
    LOG4CXX_DEBUG(logger, doc->getContent()->getEmployee()->getDoubleElement());
    doc->getContent()->getEmployee()->setFloatElement("2.71");
    doc->getContent()->getEmployee()->setDoubleElement("2e71");
    LOG4CXX_DEBUG(logger, doc->getContent()->getEmployee()->getFloatElement());
    LOG4CXX_DEBUG(logger, doc->getContent()->getEmployee()->getDoubleElement());
    
    
    std::string d1, d2;
    doc->getContent()->getEmployee()->setAgeAttr(1000);
    d1 = doc->digest();
    doc->getContent()->getEmployee()->setAgeAttr(2000);
    d2 = doc->digest();
    CPPUNIT_ASSERT(d1 != d2);
	
    //CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDef() == std::string("_def_"));
    //LOG_DEBUG << "doc serialized: " << doc->toString().c_str() << LOG4CXX_ENDMSG;
    //doc->getContent()->getEmployee()->setDef("mydef");
    //CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDef() == std::string("mydef"));
	
    //LOG_DEBUG << "docC serialized: " << doc->toString().c_str() << LOG4CXX_ENDMSG;
    //CPPUNIT_ASSERT(docC->getContent()->getEmployee()->getDef() == std::string("myval"));
}

