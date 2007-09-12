#ifndef ENUM_TEST_H
#define ENUM_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class EnumTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( EnumTest );
    CPPUNIT_TEST( enumTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void enumTest();
};

#endif  // SIMPLE_TEST_H
