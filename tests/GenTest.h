#ifndef GEN_TEST_H
#define GEN_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class GenTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( GenTest );
    CPPUNIT_TEST( genTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void genTest();
};

#endif  // SIMPLE_TEST_H
