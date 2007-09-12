#ifndef XMLARRAY_TEST_H
#define XMLARRAY_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class XmlArrayTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( XmlArrayTest );
    CPPUNIT_TEST( xmlArrayTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void xmlArrayTest();
};

#endif  // SIMPLE_TEST_H
