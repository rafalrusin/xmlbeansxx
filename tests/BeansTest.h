#ifndef BEANS_TEST_H
#define BEANS_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class BeansTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BeansTest );
    CPPUNIT_TEST( beansTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void beansTest();
};

#endif  // SIMPLE_TEST_H
