#include "UnqualTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
//#include <log4cxx/stream.h>
#include "unqual.h"
#include "XercesParser.h"
#include "LibXMLParser.h"
#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( UnqualTest );


LOGGER_PTR_SET(logger,"test.UnqualTest");
//log4cxx::logstream LOG_DEBUG(log4cxx::Logger::getLogger(std::string("test.GenTest")), log4cxx::Level::DEBUG);


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest::unqualified;
using namespace std;


void libXml() {
        LOG4CXX_DEBUG(logger, "Xerces OK");
	XmlOptions opt;
	opt.setValidation(true);
	LibXMLParser p(opt);
	p.loadGrammar("unqual.xsd");
    
       	ContentDocument doc=ContentDocument::Factory::newInstance();


       	{
        	ifstream in("unqual.xml");
	        LOG4CXX_DEBUG(logger, "parsing unqual.xml");
       		p.parse(in,doc);
    	}
	std::string str=doc.toString();
        LOG4CXX_DEBUG(logger, "parsed unqual.xml:" + str);

       	{
	        LOG4CXX_DEBUG(logger, "parsing str:");
       		p.parse(str,doc);
    	}

	std::string cmp = 
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<test2:content xmlns:aka=\"http://mind.p4.com/mytest/unqualified\" xmlns:test2=\"http://mind.p4.com/mytest/unqualified\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<employee age=\"10\"><firstname xmlns:test2=\"http://mind.p4.com/mytest\">Name1</firstname><lastname>Name2</lastname><dane>bleble</dane><def>myval</def><choice><b>20</b></choice>"
			"<pattern>.</pattern><floatElement>10.55</floatElement><doubleElement>3.14</doubleElement>"
		"</employee>"
	"</test2:content>\n";
	CPPUNIT_ASSERT_EQUAL(cmp,str);
        LOG4CXX_DEBUG(logger, "LibXML OK");
}
void UnqualTest::unqualTest() {


// XercesParser

	XmlOptions opt;
	opt.setValidation(true);
//	LibXMLParser p(opt);
	XercesParser p(opt);
	p.loadGrammar("unqual.xsd");
    
       	ContentDocument doc=ContentDocument::Factory::newInstance();


       	{
        	ifstream in("unqual.xml");
	        LOG4CXX_DEBUG(logger, "parsing unqual.xml");
       		p.parse(in,doc);
    	}
	std::string str=doc.toString();
        LOG4CXX_DEBUG(logger, "parsed unqual.xml:" + str);

       	{
       		ContentDocument doc=ContentDocument::Factory::newInstance();
	        LOG4CXX_DEBUG(logger, "parsing str:");
       		p.parse(str,doc);
    	}

	std::string cmp = 
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<test2:content xmlns:aka=\"http://mind.p4.com/mytest/unqualified\" xmlns:test2=\"http://mind.p4.com/mytest/unqualified\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<employee age=\"10\"><firstname xmlns:test2=\"http://mind.p4.com/mytest\">Name1</firstname><lastname>Name2</lastname><dane>bleble</dane><def>myval</def><choice><b>20</b></choice>"
			"<pattern>.</pattern><floatElement>10.55</floatElement><doubleElement>3.14</doubleElement>"
		"</employee>"
	"</test2:content>\n";
	CPPUNIT_ASSERT_EQUAL(cmp,str);
        LOG4CXX_DEBUG(logger, "XercesParser OK");
	libXml();
		

	
}




