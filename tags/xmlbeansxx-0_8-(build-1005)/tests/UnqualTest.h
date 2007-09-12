#ifndef UNQUAL_TEST_H
#define UNQUAL_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class UnqualTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( UnqualTest );
    CPPUNIT_TEST( unqualTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void unqualTest();
};

#endif  // SIMPLE_TEST_H
