#include "ArrayTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
#include "c.h"

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ArrayTest );


XMLBEANSXX_LOGGER_PTR_SET(logger,"test.ArrayTest");


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;


void ArrayTest::arrayTest()
{
	{
	      	XmlArray<XmlInteger>  a=XmlArray<XmlInteger>::Factory::newInstance();
		XmlArray<XmlPositiveInteger> b;
        	XmlArray<XmlObject>  c;
	        a.append(XmlInteger("101")).append(XmlPositiveInteger("102")).append(XmlInteger("103"));
        	XMLBEANSXX_DEBUG(logger,"!1");
	        XMLBEANSXX_DEBUG(logger,a.toString());
	        c=xmlarray_java_cast<XmlObject>(a);
	        XMLBEANSXX_DEBUG(logger,"!2");
	        b=xmlarray_java_cast<XmlPositiveInteger>(c);
	        XMLBEANSXX_DEBUG(logger,"!3");
	        XMLBEANSXX_DEBUG(logger,a.toString());
        	XMLBEANSXX_DEBUG(logger,"!4");
	        XMLBEANSXX_DEBUG(logger,b.toString());
        	XMLBEANSXX_DEBUG(logger,std::string(a.getArray(0).getSchemaType()->className));
	        XMLBEANSXX_DEBUG(logger,std::string(a.getArray(1).getSchemaType()->className));
        	XMLBEANSXX_DEBUG(logger,std::string(a.getArray(2).getSchemaType()->className));
	        CPPUNIT_ASSERT(std::string(a.getArray(0).getSchemaType()->className)=="xmlbeansxx::XmlInteger");
        	CPPUNIT_ASSERT(std::string(a.getArray(1).getSchemaType()->className)=="xmlbeansxx::XmlPositiveInteger");
	        CPPUNIT_ASSERT(std::string(a.getArray(2).getSchemaType()->className)=="xmlbeansxx::XmlInteger");

	}

	{
		XmlString p2("10");
		XmlPositiveInteger  pp("10");
		XmlInteger aa(pp);
		XmlInteger bb=p2;
		XmlInteger cc("12");
		XmlPositiveInteger dd(cc);
        	XMLBEANSXX_DEBUG(logger,"!2");
	        XMLBEANSXX_DEBUG(logger,aa.toString());
	        XMLBEANSXX_DEBUG(logger,bb.toString());
	        XMLBEANSXX_DEBUG(logger,dd.toString());
	}

	{
	//cast
	        XMLBEANSXX_DEBUG(logger,"cast!!");
		Personinfo p=Personinfo::Factory::newInstance();
		p.setFirstname("ala");

		try {
			Fullpersoninfo p2(p);
	        	XMLBEANSXX_DEBUG(logger,"invalid cast");
			CPPUNIT_ASSERT(false);
		}
		catch (xmlbeansxx::ClassCastException& e) {
	        	XMLBEANSXX_DEBUG(logger,"catch exception");
		}

		Fullpersoninfo p2=Fullpersoninfo::Factory::newInstance();
		p2.setAge("10");

		XmlObject o(p2);
		Personinfo p3(o);
		Fullpersoninfo p4(o);

		XMLBEANSXX_DEBUG(logger,p4.toString());
		XMLBEANSXX_DEBUG(logger,p3.toString());
		CPPUNIT_ASSERT(p4.digest()==p3.digest());


	}
}





