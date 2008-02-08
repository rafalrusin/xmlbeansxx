#include "NamespaceTest.h"
#include <string>
#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/NSMap.h>
#include <xmlbeansxx/XmlObjectDocument.h>
#include <xmlbeansxx/XmlBeans.h>


CPPUNIT_TEST_SUITE_REGISTRATION( NamespaceTest );


XMLBEANSXX_LOGGER_PTR_SET(logger,"test.NamespaceTest");


using namespace xmlbeansxx;
using namespace std;


void NamespaceTest::namespaceTest() 
{

	{
		XmlObject o=XmlObject::Factory::newInstance();
	
		o.setElement(QName("","pawel"),XmlString("bolek"));
	
		XMLBEANSXX_DEBUG(logger,"object " + o.toString());
		CPPUNIT_ASSERT_EQUAL(std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
							"<a:xml-fragment xmlns:a=\"http://xmlbeans.apache.org/definitions\"><pawel>bolek</pawel></a:xml-fragment>\n")
					,o.toString());

	}


	{
		XmlObject o=XmlObject::Factory::newInstance();
	
		o.setElement(QName("","pawel"),XmlString("bolek"));
		o.setAttribute(QName(XmlBeans::xmlns(),""),XmlString("http://ala"));
		o.setAttribute(QName("","attr"),XmlString("attr1"));
	
		XMLBEANSXX_DEBUG(logger,"object " + o.toString());
		CPPUNIT_ASSERT_EQUAL(std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
							"<a:xml-fragment attr=\"attr1\" xmlns=\"http://ala\" xmlns:a=\"http://xmlbeans.apache.org/definitions\"><pawel xmlns=\"\">bolek</pawel></a:xml-fragment>\n")
					,o.toString());

	}


	{
		XmlObject o=XmlObject::Factory::newInstance();
		XmlString bolek=XmlString("bolek");
		bolek.setAttribute(QName(XmlBeans::xmlns(),""),XmlString("http://"));
		bolek.setAttribute(QName(XmlBeans::xmlns(),"empty"),XmlString("http://empty"));
		
		o.setElement(QName("sss","pawel"),bolek);
		o.setAttribute(QName(XmlBeans::xmlns(),""),XmlString("http://ala"));
		o.setAttribute(QName("","attr"),XmlString("attr1"));
	
		XMLBEANSXX_DEBUG(logger,"object " + o.toString());
		CPPUNIT_ASSERT_EQUAL(std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
							"<a:xml-fragment attr=\"attr1\" xmlns=\"http://ala\" xmlns:a=\"http://xmlbeans.apache.org/definitions\"><b:pawel xmlns=\"http://\" xmlns:empty=\"http://empty\" xmlns:b=\"sss\">bolek</b:pawel></a:xml-fragment>\n")
					,o.toString());

	}

	{
		XmlObject o=XmlObject::Factory::newInstance();
		XmlString bolek=XmlString("bolek");
		bolek.setAttribute(QName(XmlBeans::xmlns(),""),XmlString("http://"));
		bolek.setAttribute(QName(XmlBeans::xmlns(),"empty"),XmlString("http://empty"));
		
		o.setElement(QName("","pawel"),bolek);
		o.setAttribute(QName(XmlBeans::xmlns(),""),XmlString("http://ala"));
		o.setAttribute(QName("","attr"),XmlString("attr1"));
	
		XMLBEANSXX_DEBUG(logger,"object " + o.toString());
		CPPUNIT_ASSERT_EQUAL(std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
							"<a:xml-fragment attr=\"attr1\" xmlns=\"http://ala\" xmlns:a=\"http://xmlbeans.apache.org/definitions\"><pawel xmlns=\"\" xmlns:empty=\"http://empty\">bolek</pawel></a:xml-fragment>\n")
					,o.toString());

	}
	
}



