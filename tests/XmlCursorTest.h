#ifndef XMLCURSOR_TEST_H
#define XMLCURSOR_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class XmlCursorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( XmlCursorTest );
    CPPUNIT_TEST( xmlCursorTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void xmlCursorTest();
};

#endif  // SIMPLE_TEST_H
