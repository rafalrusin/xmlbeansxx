#include "XPathTest.h"
#include <string>
#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/NSMap.h>
#include <xmlbeansxx/XmlObjectDocument.h>
#include "pawel.h"


CPPUNIT_TEST_SUITE_REGISTRATION( XPathTest );


LOGGER_PTR_SET(logger,"test.XPathTest");


using namespace xmlbeansxx;
using namespace std;
using namespace ala;


void XPathTest::xPathTest() 
{
	{
		//selectPath
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
	        LOG4CXX_DEBUG(logger,"zakupy class: " + o.getSchemaType()->className);
	        LOG4CXX_DEBUG(logger,"zakupy: " + o.toString());
		
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

	        LOG4CXX_DEBUG(logger,"zakupy2: " + z.toString());
		
		NSMap ns;
		ns.addNamespace("a","http://ala");
	        LOG4CXX_DEBUG(logger,"QName: " + ns.getQName("a:nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("a:nazwa")), std::string("nazwa@http://ala"));
		
		std::vector<XmlObject> retu=z.selectPath(ns,"./a:zakupy/a:klient/a:ala");
		
		XmlArray<XmlObject> a(retu);
	        LOG4CXX_DEBUG(logger,"path: " + a.toString())
		XmlDecimal d = a.xgetArray(0);
	        LOG4CXX_DEBUG(logger,"element: " + d.toString());
		CPPUNIT_ASSERT_EQUAL(d.getMpfValue(), mpf_class(13));

		
		ns.addNamespace("","http://ala");
	        LOG4CXX_DEBUG(logger,"QName2: " + ns.getQName("nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("nazwa")), std::string("nazwa@http://ala"));

		std::vector<XmlObject> retu2=z.selectPath(ns,"/zakupy/a:klient/bolek/lolek/");
	        LOG4CXX_DEBUG(logger,std::string("retu2 size: ") << retu2.size());
		CPPUNIT_ASSERT_EQUAL(retu2.size(), size_t(0));

		std::string path = 	"declare namespace ale='http://ala' "
					"declare namespace ble='http://ala' "
					"./ble:zakupy/ale:klient/ble:nazwa/@attr";
		std::vector<XmlObject> retu3=z.selectPath(path);
		
	        LOG4CXX_DEBUG(logger,std::string("path: ") + path);
		XmlArray<XmlObject> a3(retu3);
	        LOG4CXX_DEBUG(logger,"retu3: " + a3.toString())
		CPPUNIT_ASSERT_EQUAL(XmlString(retu3[0]).getStringValue(), std::string("olo"));		
		CPPUNIT_ASSERT_EQUAL(retu3.size(), size_t(1));		
	}
	{
		//selectPath with *
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

		NSMap ns;
		ns.addNamespace("a","http://ala");		
		std::vector<XmlObject> retu=z.selectPath(ns,"./a:*/a:klient/a:ala");
		
		XmlArray<XmlObject> a(retu);
	        LOG4CXX_DEBUG(logger,"path: " + a.toString())
		XmlDecimal d = a.xgetArray(0);
	        LOG4CXX_DEBUG(logger,"element: " + d.toString());
		CPPUNIT_ASSERT_EQUAL(d.getMpfValue(), mpf_class(13));


		std::vector<XmlObject> retu2=z.selectPath(ns,"./*/a:klient/*:ala");
		
		XmlArray<XmlObject> a2(retu2);
	        LOG4CXX_DEBUG(logger,"path: " + a2.toString())
		XmlDecimal d2 = a2.xgetArray(0);
	        LOG4CXX_DEBUG(logger,"element: " + d2.toString());
		CPPUNIT_ASSERT_EQUAL(d2.getMpfValue(), mpf_class(13));


		
		ns.addNamespace("","http://ala");
	        LOG4CXX_DEBUG(logger,"QName2: " + ns.getQName("nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("nazwa")), std::string("nazwa@http://ala"));

		std::vector<XmlObject> retu3=z.selectPath(ns,"/zakupy/a:klient/bolek/lolek/");
	        LOG4CXX_DEBUG(logger,std::string("retu2 size: ") << retu3.size());
		CPPUNIT_ASSERT_EQUAL(retu3.size(), size_t(0));

		std::string path = 	"declare namespace ale='http://ala' "
					"declare namespace ble='http://ala' "
					"./ble:zakupy/ale:*/ble:nazwa/@attr";
		std::vector<XmlObject> retu4=z.selectPath(path);
		
	        LOG4CXX_DEBUG(logger,std::string("path: ") + path);
		XmlArray<XmlObject> a4(retu4);
	        LOG4CXX_DEBUG(logger,"retu3: " + a4.toString())
		CPPUNIT_ASSERT_EQUAL(XmlString(retu4[0]).getStringValue(), std::string("olo"));		
		CPPUNIT_ASSERT_EQUAL(retu4.size(), size_t(1));		
	}
	{
		//cselectPath with *
	
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

		NSMap ns;
		ns.addNamespace("a","http://ala");		
		std::vector<XmlObject> retu=z.cselectPath(ns,"./a:*/*:klient/*:adres");
		
		CPPUNIT_ASSERT_EQUAL(size_t(1), retu.size());		
		retu[0].setSimpleContent("Egejska 3");
		
		XmlArray<XmlObject> a(retu);
	        LOG4CXX_DEBUG(logger,"path: " + a.toString());
	        LOG4CXX_DEBUG(logger,"XmlObjectDokument: " + o.toString());
	        LOG4CXX_DEBUG(logger,"ZakupyDokument: " + z.toString());
		CPPUNIT_ASSERT_EQUAL(std::string("Egejska 3"), XmlString(z.selectPath(ns,"./a:*/*:klient/*:adres").front()).getStringValue());		

	
	}

	{	//cselectPath without namespace
	
		XmlObject o = XmlObjectDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

		std::vector<XmlObject> retu=z.cselectPath("./*/*:klient/*:nazwa");
		
		CPPUNIT_ASSERT_EQUAL(size_t(1), retu.size());		
		
		XmlArray<XmlObject> a(retu);
	        LOG4CXX_DEBUG(logger,"path: " + a.toString());
		CPPUNIT_ASSERT_EQUAL(retu[0].getSimpleContent(), std::string("pawel"));		
	
	}
}



