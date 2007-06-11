#include "EnumTest.h"
#include <string>
#include <xmlbeansxx/logger.h>
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
		k.setNazwa("pawel");
		k.setAla(12);
		k.setEnumTest(Klient_EnumTest::ATOMEK);

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


	}
	
}



