#include "ArrayTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
#include "c.h"

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ArrayTest );


LOGGER_PTR_SET(logger,"test.ArrayTest");


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
        	LOG4CXX_DEBUG(logger,"!1");
	        LOG4CXX_DEBUG(logger,a.toString());
	/*        c=xmlarray_java_cast<XmlObject>(a);
	        LOG4CXX_DEBUG(logger,"!2");
	        b=xmlarray_java_cast<XmlPositiveInteger>(c);
	        LOG4CXX_DEBUG(logger,"!3");
	        LOG4CXX_DEBUG(logger,a.toString());
        	LOG4CXX_DEBUG(logger,"!4");
	        LOG4CXX_DEBUG(logger,b.toString());
        	LOG4CXX_DEBUG(logger,std::string(a.getArray(0).getSchemaType()->className));
	        LOG4CXX_DEBUG(logger,std::string(a.getArray(1).getSchemaType()->className));
        	LOG4CXX_DEBUG(logger,std::string(a.getArray(2).getSchemaType()->className));
	        CPPUNIT_ASSERT(std::string(a.getArray(0).getSchemaType()->className)=="xmlbeansxx::XmlInteger");
        	CPPUNIT_ASSERT(std::string(a.getArray(1).getSchemaType()->className)=="xmlbeansxx::XmlPositiveInteger");
	        CPPUNIT_ASSERT(std::string(a.getArray(2).getSchemaType()->className)=="xmlbeansxx::XmlInteger");
	*/
	}
	
	{
		XmlString p2("10");
		XmlPositiveInteger p("10");
		XmlInteger a(p);
		a.clone();
        	LOG4CXX_DEBUG(logger,"!2");
	        LOG4CXX_DEBUG(logger,a.toString());
	}
	
}



