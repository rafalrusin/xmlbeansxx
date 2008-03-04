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

XMLBEANSXX_LOGGER_PTR_SET(logger,"test.ParserTest");
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
//		LibXMLParser p(opt);
		p.loadGrammar("c.xsd");
			    
	       	ContentDocument doc=ContentDocument::Factory::newInstance();
	

		for(int i=0;i<2;i++) {
			//Test 2 times
		       	try {
	        		ifstream in("bad_c.xml");
			        XMLBEANSXX_DEBUG(logger, "parsing bad_c.xml");
	       			p.parse(in,doc);
		        	XMLBEANSXX_ERROR(logger, "bad_c.xml parsed !!!");
				CPPUNIT_ASSERT(false);
	    		}catch (xmlbeansxx::XmlParseException & e) {
				XMLBEANSXX_DEBUG(logger, std::string("EXCEPTION: ") + e.what());
				std::string cmp = "Xml:While parsing: Xerces-c error: At line 4, char 64, Unknown element 'test2:firstname', std";
				CPPUNIT_ASSERT_EQUAL(std::string(e.what()),cmp);
			}
		}
	
	
	
	       	{
	        	ifstream in("c.xml");
		        XMLBEANSXX_DEBUG(logger, "parsing c.xml");
	       		p.parse(in,doc);
			XMLBEANSXX_DEBUG(logger, "dump:" + Contents::Walker::dump(doc.contents));
	    	}
		std::string str=doc.toString();
	        XMLBEANSXX_DEBUG(logger, "parsed c.xml:" + str);
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<test2:content xmlns:test2=\"http://mind.p4.com/mytest\" xmlns:test3=\"http://mind.p4.com/mytest\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		  "<test2:employee age=\"10\" xmlns=\"http://mind.p4.com/mytest\">"
		    "<test2:firstname xmlns:test2=\"http://mind.p4.com/mytest\">Name1</test2:firstname>"
		    "<lastname>Name2</lastname>"
		    "<dane>bleble</dane>"
		    "<def>myval</def>"
		    "<choice>"
		      "<b>20</b>"
		    "</choice>"
		    "<pattern>.</pattern><floatElement>10.55</floatElement><doubleElement>3.14</doubleElement>"
		  "</test2:employee>"
		"</test2:content>\n";
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
	        XMLBEANSXX_DEBUG(logger, "z.getZakupy().getInfo().getSchemaType()->name: " + infoSchema);
		std::string cmp = "anyType@http://www.w3.org/2001/XMLSchema";
		CPPUNIT_ASSERT_EQUAL(cmp,infoSchema);
		XmlObject info= z.getZakupy().getInfo();
		Contents::Walker::ElemsType infoE = Contents::Walker::getElems(info);
		int size = infoE.size();
		CPPUNIT_ASSERT_EQUAL(size,1);
		std::string infoESchema=infoE[0].second->st->name;
	        XMLBEANSXX_DEBUG(logger, "info element SchemaType name: " + infoESchema);
		std::string cmp2 = "ala__ZakupyDocument_Zakupy@http://xmlbeansxx.touk.pl/xmlbeansxx/innerType";
		CPPUNIT_ASSERT_EQUAL(cmp2,infoESchema);
		
		ZakupyDocument z2(info);
	        XMLBEANSXX_DEBUG(logger, "zakupy: " + z2.toString());		
	}

	// cast => serialize <-> parse of anyType
	
	{
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<cos xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
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
		"</cos>");
		
	        XMLBEANSXX_DEBUG(logger, "Document: " + o.toString(XmlOptions::serializeInnerTypes()));
		XmlObject k = o.selectPath("/*/*:klient").front();
		std::string k_str=k.toString();
	        XMLBEANSXX_DEBUG(logger, "klient: " + k_str);
		Klient k3 = Klient::Factory::newInstance();
	        XMLBEANSXX_DEBUG(logger, "klient3: " + k3.toString());
		
		
		Klient k2=Klient::Factory::parse(k_str);
	        XMLBEANSXX_DEBUG(logger, "klient2: " + k2.toString());
	}
	{
		XmlObject s=XmlString::Factory::parse("<test> something </test>");
		std::string s_str=s.toString(XmlOptions::serializeTypes());
		std::string s_equ=
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<a:xml-fragment xsi:type=\"b:string\" xmlns:a=\"http://xmlbeans.apache.org/definitions\" xmlns:b=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"\
				" something "
			"</a:xml-fragment>\n";
		XMLBEANSXX_DEBUG(logger, "something: " + s_str);
		CPPUNIT_ASSERT_EQUAL(s_str,s_equ);
	}
	{
		XmlObject s=XmlObjectDocument::Factory::parse("<test> something </test>");
		std::string s_str=s.toString(XmlOptions::XmlOptions::serializeInnerTypes());
		std::string s_equ=
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<test> something </test>\n";
	        		
		XMLBEANSXX_DEBUG(logger, "something: " + s_str);
		CPPUNIT_ASSERT_EQUAL(s_str,s_equ);
	}
	{
		XmlObject s=XmlObjectDocument::Factory::parse("<test2/>");
		std::string s_str=s.toString(XmlOptions::XmlOptions::serializeInnerTypes());
		std::string s_equ=
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<test2/>\n";
	        		
		XMLBEANSXX_DEBUG(logger, "something: " + s_str);
		CPPUNIT_ASSERT_EQUAL(s_str,s_equ);
	}
	
/*	{
		XmlObject s=XmlToken::Factory::parse("<test> something \t a \n b   </test>");
		std::string s_str=s.toString(XmlOptions::serializeTypes());
		std::string s_equ=
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<a:xml-fragment xsi:type=\"b:string\" xmlns:a=\"http://xmlbeans.apache.org/definitions\" xmlns:b=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"\
				" something "
			"</a:xml-fragment>\n";
		XMLBEANSXX_DEBUG(logger, "something: " + s_str);
		CPPUNIT_ASSERT_EQUAL(s_str,s_equ);
	}
*/	
	{ //Xercex
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<e xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		" Jeden "
		" <a/> \n "
		" Dwa "
		" <b/> "
		" Trzy \n " 
		" <c/> "
		"</e>\n";
		
		XmlObjectDocument o;
		XercesParser p;
		p.parse(cmp,o);
		XMLBEANSXX_DEBUG(logger, "dump: " + Contents::Walker::dump(o.contents));
		std::string str = o.toString();
	        XMLBEANSXX_DEBUG(logger, "Document: " + str);
		CPPUNIT_ASSERT_EQUAL(cmp,str);
	}
	{ //libxml
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<e xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		" Jeden "
		" <a/> \n "
		" Dwa "
		" <b/> "
		" Trzy \n " 
		" <c/> "
		"</e>\n";
		
		XmlObjectDocument o;
		LibXMLParser p;
		p.parse(cmp,o);
		XMLBEANSXX_DEBUG(logger, "dump: " + Contents::Walker::dump(o.contents));
		std::string str = o.toString();
	        XMLBEANSXX_DEBUG(logger, "Document: " + str);
		CPPUNIT_ASSERT_EQUAL(cmp,str);
	}
	{
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<e xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		" Jeden "
		" <a/> \n "
		" Dwa "
		" <b/> "
		" Trzy \n " 
		" <c/> "
		"</e>\n";
		
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger, "dump: " + Contents::Walker::dump(o.contents));
		std::string str = o.toString();
	        XMLBEANSXX_DEBUG(logger, "Document: " + str);
		CPPUNIT_ASSERT_EQUAL(cmp,str);
	}

}

