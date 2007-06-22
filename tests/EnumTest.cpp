#include "EnumTest.h"
#include <string>
#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/XmlObjectDocument.h>
#include "pawel.h"


CPPUNIT_TEST_SUITE_REGISTRATION( EnumTest );


LOGGER_PTR_SET(logger,"test.EnumTest");


using namespace xmlbeansxx;
using namespace std;
using namespace ala;


void EnumTest::enumTest() 
{
	{

		Klient k;
		k.setSimpleContent("ala ma kota");
		k.setNazwa("pawel").setAla(12).setEnumTest(Klient_EnumTest::ATOMEK);

//		a= k.xgetEnumTest()
//		a.setSimpleString(" ble ble ");

	        LOG4CXX_DEBUG(logger,"Klient: " + k.toString());
		CPPUNIT_ASSERT(k.getEnumTest() == Klient_EnumTest::ATOMEK);
	        LOG4CXX_DEBUG(logger,"Klient int: " <<  k.getEnumTest() );
		CPPUNIT_ASSERT_EQUAL(k.getEnumTest(), 4);
		k.setEnumTest("lolek");
	        LOG4CXX_DEBUG(logger,"Klient: " + k.toString());
		CPPUNIT_ASSERT_EQUAL(k.xgetEnumTest().getEnumValue(), 1);
		k.setEnumTest(2);
	        LOG4CXX_DEBUG(logger,"Klient: " + k.toString());
		CPPUNIT_ASSERT(k.xgetEnumTest()== "tytus");
		ZakupyDocument z=ZakupyDocument::Factory::newInstance();
		z.cgetZakupy().setKlient(k);

	        LOG4CXX_DEBUG(logger,"zakupy: " + z.toString());		


	}
	{
		Klient k;
		try {
			k.getNazwa();
			CPPUNIT_ASSERT(true);
		       	LOG4CXX_DEBUG(logger,"throw !! ");
		} catch (NullPtrException &e) {
		       	LOG4CXX_DEBUG(logger,"catch: " + std::string(e.what()));
		}
	
	}
	{	
		XmlString s = XmlString::Factory::parse(
		"<d:xml-fragment xmlns:d='http://xmlbeans.apache.org/definitions' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
			"<d:element xsi:type='xs:string'> Ola ma kota  </d:element>"
		"</d:xml-fragment>");
		CPPUNIT_ASSERT_EQUAL(s.getStringValue(), std::string(" Ola ma kota  "));
		XmlString t("Ula");
		XmlString u("Ala");
		u = XmlString::Factory::parse(t.toString());
		CPPUNIT_ASSERT_EQUAL(u.getStringValue(), std::string("Ula"));		
	}
	{
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa>pawel</nazwa>"
		"		<ala>12.000000</ala>"
		"	</klient>"
		"</zakupy>"
		);
	        LOG4CXX_DEBUG(logger,"zakupy class: " + o.getSchemaType()->className);
	        LOG4CXX_DEBUG(logger,"zakupy: " + o.toString());
		
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

	        LOG4CXX_DEBUG(logger,"zakupy2: " + z.toString());
		
		
		
	}
	
}



