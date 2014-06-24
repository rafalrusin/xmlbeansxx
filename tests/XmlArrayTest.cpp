#include "XmlArrayTest.h"
#include "XmlOptions.h"

#include <string>

#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/xmlbeansxx.h>
#include <boost/shared_array.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( XmlArrayTest );


XMLBEANSXX_LOGGER_PTR_SET(logger,"test.XmlArrayTest");
using namespace xmlbeansxx;

void XmlArrayTest::xmlArrayTest()
{


	try {
		XmlArray<XmlString> a;
		a = XmlString("test1");
	       	XMLBEANSXX_DEBUG(logger,"invalid cast");
		CPPUNIT_ASSERT(false);
	}
	catch (xmlbeansxx::ClassCastException& e) {
	       	XMLBEANSXX_DEBUG(logger,"catch exception");
	}

	{
		XmlArray<XmlString> a1=XmlArray<XmlString>::Factory::newInstance();
		a1.append(XmlString("bolek"));
		a1.append(XmlString("lolek"));
		std::string s1 = a1.toString();
		XmlObject o(a1);
		XmlArray<XmlString> a2(o);
		std::string s2 = a2.toString();

	       	XMLBEANSXX_DEBUG(logger,"s2 string:" + s2);
		CPPUNIT_ASSERT_EQUAL(s1,s2);

		XmlArray<XmlObject> a3(a1);
	       	XMLBEANSXX_DEBUG(logger,"a3:" + a3.toString());
	}

	{
		XmlArray<XmlObject> a1=XmlArray<XmlObject>::Factory::newInstance();
		a1.append(XmlString("bolek"));
		a1.append(XmlString("lolek"));

	       	XMLBEANSXX_DEBUG(logger,"a1:" + a1.toString());

		try {
			XmlArray<XmlString> a2(a1);
		       	XMLBEANSXX_DEBUG(logger,"invalid cast");
			CPPUNIT_ASSERT(false);
		}
		catch (xmlbeansxx::ClassCastException& e) {
		       	XMLBEANSXX_DEBUG(logger,"catch exception");
		}

	}

	{
		XmlArray<XmlObject> a1=XmlArray<XmlObject>::Factory::newInstance();
		a1.append(XmlString("bolek"));
		a1.append(XmlString("lolek"));
		a1.append(XmlInteger(12));

	       	XMLBEANSXX_DEBUG(logger,"a1:" + a1.toString(XmlOptions::serializeInnerTypes()));

	}

}
