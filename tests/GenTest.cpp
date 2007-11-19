#include "GenTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
//#include <log4cxx/stream.h>
#include "c.h"

#include <fstream>
#include <xmlbeansxx/LibXMLParser.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GenTest );

void namespaceTests();
void parsing();

LOGGER_PTR_SET(logger,"test.GenTest");
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
        ContentDocument root=ContentDocument::Factory::newInstance();
        //root->cgetContent()->set
        ContentType ct=ContentType::Factory::newInstance();
        root.setContent(ct);
        Fullpersoninfo p=Fullpersoninfo::Factory::newInstance();
        ct.setEmployee(p);
        p.setCity(77);
        p.xsetAddress(XmlString());
        p.xsetLastname(CdataString("_ln<>_"));
        p.xsetFirstname(XmlString("_fn_"));
        p.setDt("\n\n 2004-01-30T22:50:11  ");
	LOG4CXX_DEBUG(logger, "--table--");
        XmlArray<XmlString>  tab=p.dgetTableArray();
	LOG4CXX_DEBUG(logger, "--table2--");
	LOG4CXX_DEBUG(logger, "TableArray: " + tab.toString());
        tab.cgetArray(0);
        tab.cgetArray(1);
	LOG4CXX_DEBUG(logger, "--table3--");
	p.dsetTableArray(tab);
	LOG4CXX_DEBUG(logger, "--table4--");
	p.addNewTable();
		p.setTableArray(3,"mm");
		
		CPPUNIT_ASSERT(p.sizeOfTable()==4);
		p.removeTable(2);
		CPPUNIT_ASSERT(p.sizeOfTable()==3);
		CPPUNIT_ASSERT(p.getTableArray(2)==std::string("mm"));
		CPPUNIT_ASSERT(!p.xgetTableArray(20).hasContents());
		CPPUNIT_ASSERT(!p.xgetTableArray(20));
		
		
        p.xsetCar(CarType("2"));
        p.setCar2(CarType("3").getSimpleContent());

        p.xsetAge(XmlInt());
        p.xsetMoney(AmountType("10.256789"));
        CPPUNIT_ASSERT(p.xgetMoney().getSimpleContent()==std::string("10.26"));
        p.xsetMoney(AmountType("10.254789"));
        CPPUNIT_ASSERT(p.xgetMoney().getSimpleContent()==std::string("10.25"));
        //root->serialize(cout);
	LOG4CXX_DEBUG(logger,std::string("created:\n") + root.toString() + "\n~created\n");
	LOG4CXX_DEBUG(logger,"digest:\n" + root.digest());

        ContentDocument root2 = ContentDocument::Factory::parse(root.toString());
        CPPUNIT_ASSERT(root2.getContent().getEmployee().getLastname() == std::string("_ln<>_"));

}

void parsing() {
    XmlOptions opt;
    opt.setValidation(true);
    LibXMLParser p(opt);
    p.loadGrammar("c.xsd");
    
    
    ContentDocument doc;
    {
        ifstream in("c.xml");
        LOG4CXX_DEBUG(logger, "parsing c.xml");
        ContentDocument docC=ContentDocument::Factory::newInstance();
        try {
          p.parse(in,docC);
        } catch (BeansException &ex) {
          LOG4CXX_INFO(logger,std::string("Exception: ") + ex.getMessage());
          CPPUNIT_ASSERT(false);
        }
		
        doc = docC;
    }

    
    CPPUNIT_ASSERT(doc.getContent().getEmployee().getAge()==10);
    LOG4CXX_DEBUG(logger,doc.toString());

    doc.getContent().getEmployee().setAnyElement(QName("http://mind.p4.com/mytest","dane"),"<mojedane>abcdef</mojedane>");
    LOG4CXX_DEBUG(logger,doc.toString());

    Personinfo pi=doc.getContent().getEmployee();
    LOG4CXX_DEBUG(logger,"got pi:" + pi.toString());
    try {
        std::string v = pi.xgetMoney().toString();
        LOG4CXX_DEBUG(logger,v);
        LOG4CXX_DEBUG(logger,"*NULL passed");
//        CPPUNIT_ASSERT(false);
    } catch (BeansException e) {
        LOG4CXX_DEBUG(logger,e.getMessage());
    }

    LOG4CXX_DEBUG(logger,std::string("cdata firstname:") + doc.getContent().getEmployee().getFirstname());
    CPPUNIT_ASSERT(doc.getContent().getEmployee().getFirstname() == std::string("Name1"));
    
    LOG4CXX_DEBUG(logger,"--1--");
    CPPUNIT_ASSERT(doc.getContent().getEmployee().getChoice().getB()==20);
    LOG4CXX_DEBUG(logger,"B = " + doc.getContent().getEmployee().getChoice().xgetB().toString() );
    LOG4CXX_DEBUG(logger,"--2--");
    CPPUNIT_ASSERT(doc.getContent().getEmployee().getChoice().xgetB().getSimpleContent()=="20");
    //LOG_DEBUG<<"--3--"<<doc->getContent()->getEmployee()->getChoice()->getA()<<LOG4CXX_ENDMSG;
    CPPUNIT_ASSERT(!doc.getContent().getEmployee().getChoice().xgetA().hasContents());
    
    //LOG_DEBUG << doc->getContent()->getEmployee()->getDefault() << LOG4CXX_ENDMSG;
//    CPPUNIT_ASSERT(doc.getContent().getEmployee().getDefault()==101);
//    CPPUNIT_ASSERT(doc.getContent().getEmployee().xgetDefault().getSimpleContent()=="101");

    /* test float and double */
    LOG4CXX_DEBUG(logger, doc.getContent().getEmployee().xgetFloatElement().toString());
    LOG4CXX_DEBUG(logger, doc.getContent().getEmployee().xgetDoubleElement().toString());
    doc.getContent().getEmployee().setFloatElement("2.71");
    doc.getContent().getEmployee().setDoubleElement("2e71");
    LOG4CXX_DEBUG(logger, doc.getContent().getEmployee().xgetFloatElement().toString());
    LOG4CXX_DEBUG(logger, doc.getContent().getEmployee().xgetDoubleElement().toString());
    

    std::string d1, d2;
    doc.getContent().getEmployee().setAge(1000);
    d1 = doc.digest();
    doc.getContent().getEmployee().setAge(2000);
    d2 = doc.digest();
    LOG4CXX_DEBUG(logger,"digest1: "+ d1);
    LOG4CXX_DEBUG(logger,"digest2: "+ d2);
    CPPUNIT_ASSERT(d1 != d2);
    doc.getContent().getEmployee().setAge(1000);
    d2 = doc.digest();
    LOG4CXX_DEBUG(logger,"digest2: "+ d2);
    CPPUNIT_ASSERT(d1 == d2);


    //CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDef() == std::string("_def_"));
    //LOG_DEBUG << "doc serialized: " << doc->toString().c_str() << LOG4CXX_ENDMSG;
    //doc->getContent()->getEmployee()->setDef("mydef");
    //CPPUNIT_ASSERT(doc->getContent()->getEmployee()->getDef() == std::string("mydef"));
	
    //LOG_DEBUG << "docC serialized: " << doc->toString().c_str() << LOG4CXX_ENDMSG;
    //CPPUNIT_ASSERT(docC->getContent()->getEmployee()->getDef() == std::string("myval"));
}

