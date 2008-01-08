#include "EnumTest.h"
#include <string>
#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/NSMap.h>
#include <xmlbeansxx/XmlObjectDocument.h>
#include "pawel.h"


CPPUNIT_TEST_SUITE_REGISTRATION( EnumTest );


XMLBEANSXX_LOGGER_PTR_SET(logger,"test.EnumTest");


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

	        XMLBEANSXX_DEBUG(logger,"Klient1: " + k.toString());
		CPPUNIT_ASSERT(k.getEnumTest() == Klient_EnumTest::ATOMEK);
	        XMLBEANSXX_DEBUG(logger,"Klient int: " +  k.getEnumTest() );
		CPPUNIT_ASSERT_EQUAL(k.getEnumTest(), 4);
		k.setEnumTest("lolek");
	        XMLBEANSXX_DEBUG(logger,"Klient2: " + k.toString());
		CPPUNIT_ASSERT_EQUAL(k.xgetEnumTest().getEnumValue(), 1);
		k.setEnumTest(2);
	        XMLBEANSXX_DEBUG(logger,"Klient3: " + k.toString());
		CPPUNIT_ASSERT(k.xgetEnumTest()== "tytus");
		ZakupyDocument z=ZakupyDocument::Factory::newInstance();
		z.cgetZakupy().setKlient(k);

	        XMLBEANSXX_DEBUG(logger,"zakupy4: " + z.toString());		


	}
	{
		Klient k;
		try {
			k.getNazwa();
		       	XMLBEANSXX_DEBUG(logger,"throw !! ");
			CPPUNIT_ASSERT(false);
		} catch (NullPtrException &e) {
		       	XMLBEANSXX_DEBUG(logger,"catch: " + std::string(e.what()));
		}
	
	}
	{	
		XmlString s = XmlString::Factory::parse(
		"<d:xml-fragment xsi:type='xs:string' xmlns:d='http://xmlbeans.apache.org/definitions' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
			" Ola ma kota  "
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
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
	        XMLBEANSXX_DEBUG(logger,"zakupy class: " + o.getSchemaType()->className);
	        XMLBEANSXX_DEBUG(logger,"zakupy5: " + o.toString());
		
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

	        XMLBEANSXX_DEBUG(logger,"zakupy6: " + z.toString());
		
		NSMap ns;
		ns.addNamespace("a","http://ala");
	        XMLBEANSXX_DEBUG(logger,"QName: " + ns.getQName("a:nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("a:nazwa")), std::string("nazwa@http://ala"));
		
		std::vector<XmlObject> retu=z.selectPath(ns,"./a:zakupy/a:klient/a:ala");
		
		XmlArray<XmlObject> a(retu);
	        XMLBEANSXX_DEBUG(logger,"path: " + a.toString())
		XmlDecimal d = a.xgetArray(0);
	        XMLBEANSXX_DEBUG(logger,"element: " + d.toString());
		CPPUNIT_ASSERT_EQUAL(d.getMpfValue(), mpf_class(13));

		
		ns.addNamespace("","http://ala");
	        XMLBEANSXX_DEBUG(logger,"QName2: " + ns.getQName("nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("nazwa")), std::string("nazwa@http://ala"));

		std::vector<XmlObject> retu2=z.selectPath(ns,"/zakupy/a:klient/bolek/lolek/");
	        XMLBEANSXX_DEBUG(logger,std::string("retu2 size: ") + TextUtils::intToString(retu2.size()));
		CPPUNIT_ASSERT_EQUAL(retu2.size(), size_t(0));

		std::string path = 	"declare namespace ale='http://ala' "
					"declare namespace ble='http://ala' "
					"./ble:zakupy/ale:klient/ble:nazwa/@attr";
		std::vector<XmlObject> retu3=z.selectPath(path);
		
	        XMLBEANSXX_DEBUG(logger,std::string("path: ") + path);
		XmlArray<XmlObject> a3(retu3);
	        XMLBEANSXX_DEBUG(logger,"retu3: " + a3.toString())
		CPPUNIT_ASSERT_EQUAL(XmlString(retu3[0]).getStringValue(), std::string("olo"));		
		CPPUNIT_ASSERT_EQUAL(retu3.size(), size_t(1));
		
		
		
	}
	
}



