#ifndef ARRAY_TEST_H
#define ARRAY_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class ArrayTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( ArrayTest );
    CPPUNIT_TEST( arrayTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void arrayTest();
};

#endif  // SIMPLE_TEST_H
