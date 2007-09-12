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
		XmlPositiveInteger  pp("10");
		XmlInteger aa(pp);
		XmlInteger bb=p2;
		XmlInteger cc("12");
		XmlPositiveInteger dd(cc);
        	LOG4CXX_DEBUG(logger,"!2");
	        LOG4CXX_DEBUG(logger,aa.toString());
	        LOG4CXX_DEBUG(logger,bb.toString());
	        LOG4CXX_DEBUG(logger,dd.toString());
	}

	{
	//cast
	        LOG4CXX_DEBUG(logger,"cast!!");
		Personinfo p=Personinfo::Factory::newInstance();
		p.setFirstname("ala");
		
		try {
			Fullpersoninfo p2(p);
	        	LOG4CXX_DEBUG(logger,"invalid cast");
			CPPUNIT_ASSERT(false);
		} 
		catch (xmlbeansxx::ClassCastException& e) {
	        	LOG4CXX_DEBUG(logger,"catch exception");		
		}
		
		Fullpersoninfo p2=Fullpersoninfo::Factory::newInstance();
		p2.setAge("10");
		
		XmlObject o(p2);
		Personinfo p3(o);
		Fullpersoninfo p4(o);
		
		LOG4CXX_DEBUG(logger,p4.toString());
		LOG4CXX_DEBUG(logger,p3.toString());
		CPPUNIT_ASSERT(p4.digest()==p3.digest());
		
		
	}	
}





