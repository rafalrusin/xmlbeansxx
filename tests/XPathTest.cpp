#include "XPathTest.h"
#include <string>
#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/NSMap.h>
#include <xmlbeansxx/XmlObjectDocument.h>
#include "pawel.h"


CPPUNIT_TEST_SUITE_REGISTRATION( XPathTest );


XMLBEANSXX_LOGGER_PTR_SET(logger,"test.XPathTest");


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
	        XMLBEANSXX_DEBUG(logger,"zakupy class: " + o.getSchemaType()->className);
	        XMLBEANSXX_DEBUG(logger,"zakupy: " + o.toString());
		
		ZakupyDocument z=ZakupyDocument::Factory::parse(o.toString());

	        XMLBEANSXX_DEBUG(logger,"zakupy2: " + z.toString());
		
		NSMap ns;
		ns.addNamespace("a","http://ala");
	        XMLBEANSXX_DEBUG(logger,"QName: " + ns.getQName("a:nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("a:nazwa")), std::string("nazwa@http://ala"));
		
		std::vector<XmlObject> retu=z.selectPath(ns,"./a:zakupy/a:klient/a:ala");
		
		XmlArray<XmlObject> a(retu);
	        XMLBEANSXX_DEBUG(logger,"path: " + a.toString())
		XmlDecimal d = a.xgetArray(0);
	        XMLBEANSXX_DEBUG(logger,"element: " + d.toString());
		CPPUNIT_ASSERT_EQUAL(d.getMpfValue(), xmlbeansxx::mpf_class(13));

		
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
	        XMLBEANSXX_DEBUG(logger,"path: " + a.toString())
		XmlDecimal d = a.xgetArray(0);
	        XMLBEANSXX_DEBUG(logger,"element: " + d.toString());
		CPPUNIT_ASSERT_EQUAL(d.getMpfValue(), xmlbeansxx::mpf_class(13));


		std::vector<XmlObject> retu2=z.selectPath(ns,"./*/a:klient/*:ala");
		
		XmlArray<XmlObject> a2(retu2);
	        XMLBEANSXX_DEBUG(logger,"path: " + a2.toString())
		XmlDecimal d2 = a2.xgetArray(0);
	        XMLBEANSXX_DEBUG(logger,"element: " + d2.toString());
		CPPUNIT_ASSERT_EQUAL(d2.getMpfValue(), xmlbeansxx::mpf_class(13));


		
		ns.addNamespace("","http://ala");
	        XMLBEANSXX_DEBUG(logger,"QName2: " + ns.getQName("nazwa"));
		CPPUNIT_ASSERT_EQUAL(std::string(ns.getQName("nazwa")), std::string("nazwa@http://ala"));

		std::vector<XmlObject> retu3=z.selectPath(ns,"/zakupy/a:klient/bolek/lolek/");
	        XMLBEANSXX_DEBUG(logger,std::string("retu2 size: ") + TextUtils::intToString(retu3.size()) );
		CPPUNIT_ASSERT_EQUAL(retu3.size(), size_t(0));

		std::string path = 	"declare namespace ale='http://ala' "
					"declare namespace ble='http://ala' "
					"./ble:zakupy/ale:*/ble:nazwa/@attr";
		std::vector<XmlObject> retu4=z.selectPath(path);
		
	        XMLBEANSXX_DEBUG(logger,std::string("path: ") + path);
		XmlArray<XmlObject> a4(retu4);
	        XMLBEANSXX_DEBUG(logger,"retu3: " + a4.toString())
		CPPUNIT_ASSERT_EQUAL(XmlString(retu4[0]).getStringValue(), std::string("olo"));		
		CPPUNIT_ASSERT_EQUAL(retu4.size(), size_t(1));		
	}
	{
		XmlObject z = XmlObjectDocument::Factory::parse(
			"<ala xmlns='http://ala'>"
			"	<klient enumTest='tytus'>"
			"		<nazwa attr='olo'>pawel</nazwa>"
			"		<ala>13</ala>"
			"		<ola>3</ola>"
			"		<ola>4</ola>"
			"		<ola>5</ola>"
			"		<ola>6"
			"			<zla lang='en'>3</zla>"
			"			<zla lang='pl'>4</zla>"
			"			<zla>5</zla>"
			"		</ola>"			
			"	</klient>"
			"</ala>"
		);

		{
			NSMap ns;
		        XMLBEANSXX_DEBUG(logger,"XmlObject: " + z.toString())
			ns.addNamespace("a","http://ala");		
			std::vector<XmlObject> retu=z.selectPath(ns,"./a:*/a:klient/a:ala");
		
			XmlArray<XmlObject> a(retu);
		        XMLBEANSXX_DEBUG(logger,"path: " + a.toString())
			XmlDecimal d = XmlDecimal(a.xgetArray(0).getSimpleContent());
		        XMLBEANSXX_DEBUG(logger,"element: " + d.toString());
			CPPUNIT_ASSERT_EQUAL(d.getMpfValue(), xmlbeansxx::mpf_class(13));

			std::vector<XmlObject> retu2=z.selectPath(ns,"./a:*/a:klient/a:ola");
			
			XmlArray<XmlObject> a2(retu2);
		        XMLBEANSXX_DEBUG(logger,"path: " + a2.toString())
			XmlDecimal d2 = XmlDecimal(a2.xgetArray(2).getSimpleContent());
	        	XMLBEANSXX_DEBUG(logger,"element: " + d2.toString());
			CPPUNIT_ASSERT_EQUAL(d2.getMpfValue(), xmlbeansxx::mpf_class(5));
			CPPUNIT_ASSERT_EQUAL(a2.size(), 4);
		}
		{
			std::vector<XmlObject> retu=z.selectPath(
					"declare namespace ala='http://ala' "
					"./ala:ala/ala:klient/*:ola[3]");
		
			XmlArray<XmlObject> a(retu);
		        XMLBEANSXX_DEBUG(logger,"path2: " + a.toString())
			CPPUNIT_ASSERT_EQUAL(a.size(), 1);
		}
		{
			std::vector<XmlObject> retu=z.selectPath(
					"declare namespace ala='http://ala' "
					"./ala:*/ala:klient/*:ola[3]/ala:zla[1]/@lang");
		
			XmlArray<XmlObject> a(retu);
		        XMLBEANSXX_DEBUG(logger,"path3: " + a.toString())
			CPPUNIT_ASSERT_EQUAL(a.size(), 1);

			std::string d = a.xgetArray(0).getSimpleContent();
		        XMLBEANSXX_DEBUG(logger,"element: " + d);
			CPPUNIT_ASSERT_EQUAL(d, std::string("pl"));
		}
	
	}
	
	
	
	
/*	{
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
*/
//		std::vector<XmlObject> retu=z.cselectPath(ns,"./a:*/*:klient/*:adres");
/*		
		CPPUNIT_ASSERT_EQUAL(size_t(1), retu.size());		
		retu[0].setSimpleContent("Egejska 3");
		
		XmlArray<XmlObject> a(retu);
	        XMLBEANSXX_DEBUG(logger,"path: " + a.toString());
	        XMLBEANSXX_DEBUG(logger,"XmlObjectDokument: " + o.toString());
	        XMLBEANSXX_DEBUG(logger,"ZakupyDokument: " + z.toString());
*/
//		CPPUNIT_ASSERT_EQUAL(std::string("Egejska 3"), XmlString(z.selectPath(ns,"./a:*/*:klient/*:adres").front()).getStringValue());		
/*
	
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
*/
//		std::vector<XmlObject> retu=z.cselectPath("./*/*:klient/*:nazwa");
/*		
		CPPUNIT_ASSERT_EQUAL(size_t(1), retu.size());		
		
		XmlArray<XmlObject> a(retu);
	        XMLBEANSXX_DEBUG(logger,"path: " + a.toString());
		CPPUNIT_ASSERT_EQUAL(retu[0].getSimpleContent(), std::string("pawel"));		
	
	}
*/	
	{	//(serialize <--> parse) annonimouse type
		ZakupyDocument z = ZakupyDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
		ZakupyDocument_Zakupy zz = z.getZakupy();
		std::string zzstring = zz.toString();
	        XMLBEANSXX_DEBUG(logger,"ZakupyDocument_Zakupy: " + zzstring);
		XmlObject o = XmlObject::Factory::parse(zzstring);
		std::string cmp = o.toString();
		CPPUNIT_ASSERT_EQUAL(cmp, zzstring);
	}

	{	//(serialize <--> parse) annonimouse type
		ZakupyDocument z = ZakupyDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
		ZakupyDocument_Zakupy zz = z.getZakupy();
		std::string zzstring = zz.toString(XmlOptions::serializeInnerTypes());
	        XMLBEANSXX_DEBUG(logger,"ZakupyDocument_Zakupy: " + zzstring);
		XmlObject o = XmlObject::Factory::parse(zzstring);
		CPPUNIT_ASSERT_EQUAL(o.getSchemaType(), zz.getSchemaType());
		
		std::string cmp = o.toString(XmlOptions::serializeInnerTypes());
		CPPUNIT_ASSERT_EQUAL(cmp, zzstring);
	}
	{	//(serialize <--> parse) annonimouse type
		ZakupyDocument z = ZakupyDocument::Factory::parse(
		"<zakupy xmlns='http://ala' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
		"	<klient enumTest='tytus'>"
		"		<nazwa attr='olo'>pawel</nazwa>"
		"		<ala>13</ala>"
		"	</klient>"
		"</zakupy>"
		);
		XmlObject z2=z.clone();
		z.getZakupy().setInfo(z2);
		std::string zstring = z.toString(XmlOptions::serializeInnerTypes());
	        XMLBEANSXX_DEBUG(logger,"ZakupyDocument (XmlOptions::serializeInnerTypes()): " + zstring);
		CPPUNIT_ASSERT_EQUAL(zstring, std::string(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<zakupy xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
  "<klient enumTest=\"tytus\">"
    "<nazwa attr=\"olo\">pawel</nazwa>"
    "<ala>13</ala>"
  "</klient>"
  "<info xsi:type=\"a:ala__ZakupyDocument\" xmlns:a=\"http://xmlbeansxx.touk.pl/xmlbeansxx/innerType\">"
    "<zakupy xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
      "<klient enumTest=\"tytus\">"
        "<nazwa attr=\"olo\">pawel</nazwa>"
        "<ala>13</ala>"
      "</klient>"
    "</zakupy>"
  "</info>"
"</zakupy>\n"));
		zstring = z.toString();
	        XMLBEANSXX_DEBUG(logger,"ZakupyDocument: " + zstring);
		CPPUNIT_ASSERT_EQUAL(zstring, std::string(
						"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
						"<zakupy xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
							"<klient enumTest=\"tytus\"><nazwa attr=\"olo\">pawel</nazwa><ala>13</ala></klient>"
							"<info>"
								"<zakupy xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
									"<klient enumTest=\"tytus\"><nazwa attr=\"olo\">pawel</nazwa><ala>13</ala></klient>"
								"</zakupy>"
							"</info>"
						"</zakupy>\n"));

/*		XmlObject o = XmlObject::Factory::parse(zzstring);
		CPPUNIT_ASSERT_EQUAL(o.getSchemaType(), zz.getSchemaType());
		
		std::string cmp = o.toString(XmlOptions::serializeInnerTypes());
		CPPUNIT_ASSERT_EQUAL(cmp, zzstring);
*/	}
}



