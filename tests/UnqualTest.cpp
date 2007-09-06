#include "UnqualTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
//#include <log4cxx/stream.h>
#include "unqual.h"
#include "XercesParser.h"
#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( UnqualTest );


LOGGER_PTR_SET(logger,"test.UnqualTest");
//log4cxx::logstream LOG_DEBUG(log4cxx::Logger::getLogger(std::string("test.GenTest")), log4cxx::Level::DEBUG);


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;

void UnqualTest::unqualTest() {


// XercesParser

	XmlOptions opt;
	opt.setValidation(true);
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
	std::string cmp = 
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<a:content xmlns:a=\"http://mind.p4.com/mytest\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<a:employee age=\"10\"><a:firstname>Name1</a:firstname><a:lastname>Name2</a:lastname><a:dane>bleble</a:dane><a:def>myval</a:def><a:choice><a:b>20</a:b></a:choice>"
			"<a:pattern>.</a:pattern><a:floatElement>10.55</a:floatElement><a:doubleElement>3.14</a:doubleElement>"
		"</a:employee>"
	"</a:content>\n";
	CPPUNIT_ASSERT_EQUAL(cmp,str);


/*	for(int i=0;i<2;i++) {
		//Test 2 times
	       	try {
        		ifstream in("bad_c.xml");
		        LOG4CXX_DEBUG(logger, "parsing bad_c.xml");
       			p.parse(in,doc);
	        	LOG4CXX_ERROR(logger, "bad_c.xml parsed !!!");
			CPPUNIT_ASSERT(false);
    		}catch (xmlbeansxx::XmlParseException & e) {
			LOG4CXX_DEBUG(logger, "EXCEPTION: " << e.what());
			std::string cmp = "Xml:While parsing: Xerces-c error: At line 4, char 64, Unknown element 'test2:firstname', std";
			CPPUNIT_ASSERT_EQUAL(std::string(e.what()),cmp);
		}
	}

*/

	
}




