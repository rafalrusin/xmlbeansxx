#include "ParseTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
//#include <log4cxx/stream.h>
#include "c.h"
#include "pawel.h"
#include "XercesParser.h"
#include "LibXMLParser.h"
#include "XmlObjectDocument.h"
#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ParseTest );

void namespaceTests();
void parsing();

LOGGER_PTR_SET(logger,"test.ParserTest");
//log4cxx::logstream LOG_DEBUG(log4cxx::Logger::getLogger(std::string("test.GenTest")), log4cxx::Level::DEBUG);


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;
using namespace ala;

void ParseTest::parseTest() {


	{
// XercesParser
	
		XmlOptions opt;
		opt.setValidation(true);
		XercesParser p(opt);
		p.loadGrammar("c.xsd");
	    
	       	ContentDocument doc=ContentDocument::Factory::newInstance();
	
	
		for(int i=0;i<2;i++) {
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
	
	
	
	       	{
	        	ifstream in("c.xml");
		        LOG4CXX_DEBUG(logger, "parsing c.xml");
	       		p.parse(in,doc);
	    	}
		std::string str=doc.toString();
	        LOG4CXX_DEBUG(logger, "parsed c.xml:" + str);
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<a:content xmlns:a=\"http://mind.p4.com/mytest\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
			"<a:employee age=\"10\"><a:firstname>Name1</a:firstname><a:lastname>Name2</a:lastname><a:dane>bleble</a:dane><a:def>myval</a:def><a:choice><a:b>20</a:b></a:choice>"
				"<a:pattern>.</a:pattern><a:floatElement>10.55</a:floatElement><a:doubleElement>3.14</a:doubleElement>"
			"</a:employee>"
		"</a:content>\n";
		CPPUNIT_ASSERT_EQUAL(cmp,str);
		
	}
	{
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"	<info>"
		"		<zakupy>"
		"			<klient enumTest='tytus'>"
		"				<nazwa attr='olo'>pawel</nazwa>"
		"				<ala>13</ala>"
		"			</klient>"
		"		</zakupy>"
		"	</info>"
		"</zakupy>"
		);
		
		
		ZakupyDocument z = o;
		std::string infoSchema=z.getZakupy().getInfo().getSchemaType()->name;
	        LOG4CXX_DEBUG(logger, "z.getZakupy().getInfo().getSchemaType()->name: " + infoSchema);
		std::string cmp = "anyType@http://www.w3.org/2001/XMLSchema";
		CPPUNIT_ASSERT_EQUAL(cmp,infoSchema);
		XmlObject info= z.getZakupy().getInfo();
		Contents::Walker::ElemsType infoE = Contents::Walker::getElems(info);
		int size = infoE.size();
		CPPUNIT_ASSERT_EQUAL(size,1);
		std::string infoESchema=infoE[0].second->st->name;
	        LOG4CXX_DEBUG(logger, "info element SchemaType name: " + infoESchema);
		std::string cmp2 = "ala__ZakupyDocument_Zakupy@http://xmlbeansxx.touk.pl/xmlbeansxx/innerType";
		CPPUNIT_ASSERT_EQUAL(cmp2,infoESchema);
		
		ZakupyDocument z2(info);
	        LOG4CXX_DEBUG(logger, "zakupy: " + z2.toString());
		
		
		
		 

	
	}
	
	
	
}

