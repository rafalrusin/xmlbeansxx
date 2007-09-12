#ifndef PARSE_TEST_H
#define PARSE_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class ParseTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( ParseTest );
    CPPUNIT_TEST( parseTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void parseTest();
};

#endif  // SIMPLE_TEST_H
