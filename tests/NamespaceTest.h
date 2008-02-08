#ifndef NAMESPACE_TEST_H
#define NAMESPACE_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class NamespaceTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( NamespaceTest );
    CPPUNIT_TEST( namespaceTest );
    CPPUNIT_TEST_SUITE_END();

public:
    void namespaceTest();
};

#endif  // SIMPLE_TEST_H
