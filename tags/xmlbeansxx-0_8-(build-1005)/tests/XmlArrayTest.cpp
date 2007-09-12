#include "XmlArrayTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/xmlbeansxx.h>
#include <boost/shared_array.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( XmlArrayTest );


LOGGER_PTR_SET(logger,"test.XmlArrayTest");
using namespace xmlbeansxx;

void XmlArrayTest::xmlArrayTest() 
{
	try {
		XmlArray<XmlString> a;
		a = XmlString("test1");
	       	LOG4CXX_DEBUG(logger,"invalid cast");
		CPPUNIT_ASSERT(false);
	} 
	catch (xmlbeansxx::ClassCastException& e) {
	       	LOG4CXX_DEBUG(logger,"catch exception");		
	}

	{
		XmlArray<XmlString> a1=XmlArray<XmlString>::Factory::newInstance();
		a1.append(XmlString("bolek"));
		a1.append(XmlString("lolek"));
		std::string s1 = a1.toString();
		XmlObject o(a1);
		XmlArray<XmlString> a2(o);
		std::string s2 = a2.toString();
		
	       	LOG4CXX_DEBUG(logger,"s2 string:" + s2);		
		CPPUNIT_ASSERT_EQUAL(s1,s2);

		XmlArray<XmlObject> a3(a1);
	       	LOG4CXX_DEBUG(logger,"a3:" + a3.toString());		
	}

	{
		XmlArray<XmlObject> a1=XmlArray<XmlObject>::Factory::newInstance();
		a1.append(XmlString("bolek"));
		a1.append(XmlString("lolek"));
		
	       	LOG4CXX_DEBUG(logger,"a1:" + a1.toString());		
		
		try {
			XmlArray<XmlString> a2(a1);
		       	LOG4CXX_DEBUG(logger,"invalid cast");
			CPPUNIT_ASSERT(false);
		} 
		catch (xmlbeansxx::ClassCastException& e) {
		       	LOG4CXX_DEBUG(logger,"catch exception");		
		}
		
	}	
	{
	       	LOG4CXX_DEBUG(logger,"testing Cast:");		
		XmlBoolean b(true);
		XmlString s1(b);
		XmlObject o(b);
		XmlString s2(o);
	       	LOG4CXX_DEBUG(logger,"s1 string:" + s2.toString());		
		CPPUNIT_ASSERT_EQUAL(s1.toString(),s2.toString());
	}
	{
	       	LOG4CXX_DEBUG(logger,"testing boolean true:");		
		XmlString b("TruE");
		XmlObject o(b);
		XmlBoolean s2(o);
	       	LOG4CXX_DEBUG(logger,"s1 string:" + s2.toString());		
		CPPUNIT_ASSERT_EQUAL(bool(s2),true);
	}
	{
	       	LOG4CXX_DEBUG(logger,"testing boolean false:");		
		XmlString b("FalSe");
		XmlObject o(b);
		XmlBoolean s2(o);
	       	LOG4CXX_DEBUG(logger,"s1 string:" + s2.toString());		
		CPPUNIT_ASSERT_EQUAL(bool(s2),false);
	}

}