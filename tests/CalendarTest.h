#ifndef CALENDAR_TEST_H
#define CALENDAR_TEST_H



#include <cppunit/extensions/HelperMacros.h>

class CalendarTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( CalendarTest );
  CPPUNIT_TEST( calendarTest );
  CPPUNIT_TEST_SUITE_END();

public:
  void calendarTest();
};

#endif  // CALENDAR_TEST_H
