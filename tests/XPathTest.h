#ifndef XPATH_TEST_H
#define XPATH_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class XPathTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( XPathTest );
    CPPUNIT_TEST( xPathTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void xPathTest();
};

#endif  // SIMPLE_TEST_H
