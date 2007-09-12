#ifndef ENTITIES_TEST_H
#define ENTITIES_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class EntitiesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE (EntitiesTest);
    CPPUNIT_TEST (entitiesTest);
    CPPUNIT_TEST_SUITE_END ();

    public:
    	void entitiesTest ();
};
#endif //ENTITIES_TEST_H
