#include "CalendarTest.h"

#include <fstream>
#include <string>
#include <sstream>
#include <xmlbeansxx/logger.h>
#include "xmlbeansxx/Calendar.h"
#include "xmlbeansxx/TextUtils.h"

#include "xmlbeansxx/BeansException.h"

#include <xmlbeansxx/xmlbeansxx.h>


CPPUNIT_TEST_SUITE_REGISTRATION (CalendarTest);


XMLBEANSXX_LOGGER_PTR_SET(logger, "test.CalendarTest");

using namespace std;
using namespace xmlbeansxx;

void CalendarTest::calendarTest () {

//	try {
		 
	XMLBEANSXX_DEBUG(logger, "++++++++++ Calendar Test ++++++++++");
	
	Calendar c;
	c.setYear(1978).setMonth("   10 		");
	XMLBEANSXX_DEBUG(logger, "c.getDst: " + TextUtils::intToString(c.getDst()));

	XMLBEANSXX_DEBUG(logger, "c.setYear(1978)");
	XMLBEANSXX_DEBUG(logger, "c.hasYear(): " + TextUtils::boolToString(c.hasYear()) + " ,c.getYear(): " + TextUtils::intToString(c.getYear()));
	CPPUNIT_ASSERT(c.hasYear() == true);
	CPPUNIT_ASSERT(c.getYear() == 1978);
	XMLBEANSXX_DEBUG(logger, "c.setMonth(\"10\")");
	XMLBEANSXX_DEBUG(logger, "c.hasMonth(): " + TextUtils::boolToString(c.hasMonth()) + " ,c.getMonth(): " + TextUtils::intToString(c.getMonth()));
	CPPUNIT_ASSERT(c.hasMonth() == true);
	CPPUNIT_ASSERT(c.getMonth() == 10);
	XMLBEANSXX_DEBUG(logger, "c.hasDay(): " + TextUtils::boolToString(c.hasDay()) + " ,c.getDay(): " + TextUtils::intToString(c.getDay()));
	CPPUNIT_ASSERT(c.hasDay() == false);

	c.setDay(31);

	XMLBEANSXX_DEBUG(logger, "c.setDay(31)");
	XMLBEANSXX_DEBUG(logger, "c.hasDay(): " + TextUtils::boolToString(c.hasDay()) + " ,c.getDay(): " + TextUtils::intToString(c.getDay()));
	CPPUNIT_ASSERT(c.hasDay() == true);
	CPPUNIT_ASSERT(c.getDay() == 31);
	
	CPPUNIT_ASSERT(c.hasFullDateInfo() == true);

	c.setHour(15);
	c.setMinutes("\t  22 ");
	c.setSeconds(47);

	XMLBEANSXX_DEBUG(logger, "c.setHour(15)");
	XMLBEANSXX_DEBUG(logger, "c.hasHour(): " + TextUtils::boolToString(c.hasHour()) + " ,c.getHour(): " + TextUtils::intToString(c.getHour()));

	CPPUNIT_ASSERT(c.hasHour() == true);
	CPPUNIT_ASSERT(c.getHour() == 15);
	CPPUNIT_ASSERT(c.hasMinutes() == true);
	CPPUNIT_ASSERT(c.getMinutes() == 22);
	CPPUNIT_ASSERT(c.hasSeconds() == true);
	CPPUNIT_ASSERT(c.getSeconds() == 47);

	CPPUNIT_ASSERT(c.hasFullTimeInfo() == true);
	CPPUNIT_ASSERT(c.hasFracSec() == false);

	XMLBEANSXX_DEBUG(logger, "c.getBDate(): " + boost::gregorian::to_simple_string(c.getDate()));
	XMLBEANSXX_DEBUG(logger, "c.getBPtime(): " + boost::posix_time::to_simple_string(c.getTime()));

	c.setFracSec(123);
	CPPUNIT_ASSERT(c.hasFracSec() == true);
	CPPUNIT_ASSERT(c.getFracSec() == 123);
	XMLBEANSXX_DEBUG(logger, "c.getBPtime(): " + boost::posix_time::to_simple_string(c.getTime()));

	XMLBEANSXX_DEBUG(logger, "+++++++ Time Zone Test +++++++");

	XMLBEANSXX_DEBUG(logger, "c.hasTimeZone: " + TextUtils::boolToString(c.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "c.isSetDt: " + TextUtils::boolToString(c.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "c.getDst: " + TextUtils::intToString(c.getDst()));
	XMLBEANSXX_DEBUG(logger, "c.getUTCTime: " + boost::posix_time::to_simple_string(c.getUTCTime()));
	XMLBEANSXX_DEBUG(logger, "c.getLocalTime: " + boost::posix_time::to_simple_string(c.getLocalTime()));

	CPPUNIT_ASSERT(c.hasTimeZone() == false);
	CPPUNIT_ASSERT(c.isSetDst() == false);
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(c.getUTCTime()) == "not-a-date-time");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(c.getLocalTime()) == "not-a-date-time");
	
	c.setGmtOff(1, 0).dstUnknown();
	
	XMLBEANSXX_DEBUG(logger, "c.setGmtOff(1,0).dstUnknown()");
	XMLBEANSXX_DEBUG(logger, "c.hasTimeZone: " + TextUtils::boolToString(c.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "c.isSetDst: " + TextUtils::boolToString(c.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "c.getUTCTime: " + boost::posix_time::to_simple_string(c.getUTCTime()));
	XMLBEANSXX_DEBUG(logger, "c.getLocalTime: " + boost::posix_time::to_simple_string(c.getLocalTime()));

	c.dstOn();

	XMLBEANSXX_DEBUG(logger, "c.dstOn()");
	XMLBEANSXX_DEBUG(logger, "c.hasTimeZone: " + TextUtils::boolToString(c.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "c.isSetDst: " + TextUtils::boolToString(c.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "c.getUTCTime: " + boost::posix_time::to_simple_string(c.getUTCTime()));
	XMLBEANSXX_DEBUG(logger, "c.getLocalTime: " + boost::posix_time::to_simple_string(c.getLocalTime()));
	XMLBEANSXX_DEBUG(logger, "c.toXsdDateTime: " + c.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "c.toXsdDate: " + c.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "c.toXsdTime: " + c.toXsdTime());
	
	CPPUNIT_ASSERT(c.hasTimeZone() == true);
	CPPUNIT_ASSERT(c.isSetDst() == true);
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(c.getUTCTime()) == "1978-Oct-31 13:22:47.123000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(c.getLocalTime()) == "1978-Oct-31 15:22:47.123000");
	CPPUNIT_ASSERT(c.toXsdDate() == "1978-10-31+02:00");
	CPPUNIT_ASSERT(c.toXsdDateTime() == "1978-10-31T15:22:47.123+02:00");
	CPPUNIT_ASSERT(c.toXsdTime() == "15:22:47.123+02:00");

	Calendar d("  \t\t\t\t\n\n\n\n\n  2008-12-12T02:22:22.123-12:33    \t  \n\n");
	
	XMLBEANSXX_DEBUG(logger, "Calendar d(\"2008-12-12T02:22:22.123-12:33\");");
	XMLBEANSXX_DEBUG(logger, "d.hasTimeZone: " + TextUtils::boolToString(d.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "d.isDstOn: " + TextUtils::boolToString(d.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "d.isSetDst: " + TextUtils::boolToString(d.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "d.getBPtime: " + boost::posix_time::to_simple_string(d.getTime()));
	XMLBEANSXX_DEBUG(logger, "d.getUTCTime: " + boost::posix_time::to_simple_string(d.getUTCTime()));
	XMLBEANSXX_DEBUG(logger, "d.getLocalTime: " + boost::posix_time::to_simple_string(d.getLocalTime()));
	XMLBEANSXX_DEBUG(logger, "d.toXsdDateTime: " + d.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "d.toXsdDate: " + d.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "d.toXsdTime: " + d.toXsdTime());

	CPPUNIT_ASSERT(d.hasTimeZone() == true);
	CPPUNIT_ASSERT(d.isDstOn() == false);
	CPPUNIT_ASSERT(d.isSetDst() == true);
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(d.getTime()) == "2008-Dec-12 02:22:22.123000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(d.getUTCTime()) == "2008-Dec-12 14:55:22.123000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(d.getLocalTime()) == "2008-Dec-12 02:22:22.123000");
	CPPUNIT_ASSERT(d.toXsdDateTime() == "2008-12-12T02:22:22.123-12:33");
	CPPUNIT_ASSERT(d.toXsdDate() == "2008-12-12-12:33");
	CPPUNIT_ASSERT(d.toXsdTime() == "02:22:22.123-12:33");

	Calendar e(" 2008-12-12T12:12:12.121Z  ");
	
	XMLBEANSXX_DEBUG(logger, "e.hasTimeZone: " + TextUtils::boolToString(e.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "e.hasFullTimeInfo: " + TextUtils::boolToString(e.hasFullTimeInfo()));
	XMLBEANSXX_DEBUG(logger, "e.isDstOn: " + TextUtils::boolToString(e.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "e.isSetDst: " + TextUtils::boolToString(e.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "e.getBDate: " + boost::gregorian::to_simple_string(e.getDate()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBDate: " + e.dateToString());
	XMLBEANSXX_DEBUG(logger, "e.getBPtime: " + boost::posix_time::to_simple_string(e.getTime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBPtime: " + e.timeToString());
	XMLBEANSXX_DEBUG(logger, "e.getUTCTime: " + boost::posix_time::to_simple_string(e.getUTCTime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getUTCTime: " + e.utcTimeToString());
	XMLBEANSXX_DEBUG(logger, "e.getLocalTime: " + boost::posix_time::to_simple_string(e.getLocalTime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getLocalTime: " + e.localTimeToString());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDateTime: " + e.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDate: " + e.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "e.toXsdTime: " + e.toXsdTime());

	CPPUNIT_ASSERT(e.hasTimeZone() == true);
	CPPUNIT_ASSERT(e.hasFullTimeInfo() == true);
	CPPUNIT_ASSERT(e.isDstOn() == false);
	CPPUNIT_ASSERT(e.isSetDst() == true);
	CPPUNIT_ASSERT(boost::gregorian::to_simple_string(e.getDate()) == "2008-Dec-12");
	CPPUNIT_ASSERT(e.dateToString() == "2008-12-12");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(e.getTime()) == "2008-Dec-12 12:12:12.121000");
	CPPUNIT_ASSERT(e.timeToString() == "2008-12-12T12:12:12.121000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(e.getUTCTime()) == "2008-Dec-12 12:12:12.121000");
	CPPUNIT_ASSERT(e.utcTimeToString() == "2008-12-12T12:12:12.121000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(e.getLocalTime()) == "2008-Dec-12 12:12:12.121000");
	CPPUNIT_ASSERT(e.localTimeToString() == "2008-12-12T12:12:12.121000");
	CPPUNIT_ASSERT(e.toXsdDateTime() == "2008-12-12T12:12:12.121Z");
	CPPUNIT_ASSERT(e.toXsdDate() == "2008-12-12Z");
	CPPUNIT_ASSERT(e.toXsdTime() == "12:12:12.121Z");

	e.setDate("   2008-12-12+12:12   ").setYearMonth("   2008-12-12:22   ").setgYear("   2008-12:22   ");
	e.setMonthDay("   --12-22Z   ").setgDay(" 			  ---22-12:31   ").setgMonth("   --12+12:12   ");
	
	XMLBEANSXX_DEBUG(logger, "e.hasTimeZone: " + TextUtils::boolToString(e.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "e.isDstOn: " + TextUtils::boolToString(e.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "e.isSetDst: " + TextUtils::boolToString(e.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "e.getBDate: " + boost::gregorian::to_simple_string(e.getDate()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBDate: " + e.dateToString());
	XMLBEANSXX_DEBUG(logger, "e.getBPtime: " + boost::posix_time::to_simple_string(e.getTime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBPtime: " + e.timeToString());
	XMLBEANSXX_DEBUG(logger, "e.getUTCTime: " + boost::posix_time::to_simple_string(e.getUTCTime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getUTCTime: " + e.utcTimeToString());
	XMLBEANSXX_DEBUG(logger, "e.getLocalTime: " + boost::posix_time::to_simple_string(e.getLocalTime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getLocalTime: " + e.localTimeToString());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDateTime: " + e.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDate: " + e.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "e.toXsdTime: " + e.toXsdTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdYearMonth: " + e.toXsdYearMonth());
	XMLBEANSXX_DEBUG(logger, "e.toXsdYear: " + e.toXsdYear());
	XMLBEANSXX_DEBUG(logger, "e.toXsdMonthDay: " + e.toXsdMonthDay());
	XMLBEANSXX_DEBUG(logger, "e.toXsdMonth: " + e.toXsdMonth());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDay: " + e.toXsdDay());
	XMLBEANSXX_DEBUG(logger, "e.toString: " + e.toString());
	
	CPPUNIT_ASSERT(e.hasTimeZone() == true);
	CPPUNIT_ASSERT(e.hasFullTimeInfo() == true);
	CPPUNIT_ASSERT(e.isDstOn() == false);
	CPPUNIT_ASSERT(e.isSetDst() == true);
	CPPUNIT_ASSERT(boost::gregorian::to_simple_string(e.getDate()) == "2008-Dec-22");
	CPPUNIT_ASSERT(e.dateToString() == "2008-12-22");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(e.getTime()) == "2008-Dec-22 12:12:12.121000");
	CPPUNIT_ASSERT(e.timeToString() == "2008-12-22T12:12:12.121000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(e.getUTCTime()) == "2008-Dec-22 00:00:12.121000");
	CPPUNIT_ASSERT(e.utcTimeToString() == "2008-12-22T00:00:12.121000");
	CPPUNIT_ASSERT(boost::posix_time::to_simple_string(e.getLocalTime()) == "2008-Dec-22 12:12:12.121000");
	CPPUNIT_ASSERT(e.localTimeToString() == "2008-12-22T12:12:12.121000");
	CPPUNIT_ASSERT(e.toXsdDateTime() == "2008-12-22T12:12:12.121+12:12");
	CPPUNIT_ASSERT(e.toXsdDate() == "2008-12-22+12:12");
	CPPUNIT_ASSERT(e.toXsdTime() == "12:12:12.121+12:12");
	CPPUNIT_ASSERT(e.toXsdYearMonth() == "2008-12+12:12");
	CPPUNIT_ASSERT(e.toXsdYear() == "2008+12:12");
	CPPUNIT_ASSERT(e.toXsdMonthDay() == "--12-22+12:12");
	CPPUNIT_ASSERT(e.toXsdMonth() == "--12+12:12");
	CPPUNIT_ASSERT(e.toXsdDay() == "---22+12:12");

	XMLBEANSXX_DEBUG(logger, "+++++++ Guessing Time Zone +++++++");

	Calendar a(" 2008-05-05T15:11:22 \n\n\n");
	Calendar b(" 2008-01-01T15:11:22 \n\n\n");
	XMLBEANSXX_DEBUG(logger, "a -> " + a.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "b -> " + b.toXsdDateTime());

	CPPUNIT_ASSERT(a.hasTimeZone() == false);
	CPPUNIT_ASSERT(a.isSetDst() == false);
	CPPUNIT_ASSERT(b.hasTimeZone() == false);
	CPPUNIT_ASSERT(b.isSetDst() == false);

	a.timeZoneGuess();
	XMLBEANSXX_DEBUG(logger, "a -> " + a.toXsdDateTime());
	b.timeZoneGuess();
	XMLBEANSXX_DEBUG(logger, "b -> " + b.toXsdDateTime());
	
	CPPUNIT_ASSERT(a.hasTimeZone() == true);
	CPPUNIT_ASSERT(a.isSetDst() == true);
	CPPUNIT_ASSERT(b.hasTimeZone() == true);
	CPPUNIT_ASSERT(b.isSetDst() == true);
	
	CPPUNIT_ASSERT(a.localTimeToString() == "2008-05-05T15:11:22");
	CPPUNIT_ASSERT(b.localTimeToString() == "2008-01-01T15:11:22");

	//Time Zone Specific Test! Working in CET/CEST time zone 
	CPPUNIT_ASSERT(a.toXsdDateTime() == "2008-05-05T15:11:22+02:00");
	CPPUNIT_ASSERT(a.utcTimeToString() == "2008-05-05T13:11:22");
	CPPUNIT_ASSERT(b.toXsdDateTime() == "2008-01-01T15:11:22+01:00");
	CPPUNIT_ASSERT(b.utcTimeToString() == "2008-01-01T14:11:22");
	//Time Zone Specific Test!
	
	XMLBEANSXX_DEBUG(logger, "+++++++ Duration Test +++++++");

	Duration q1("P1Y1M1DT1H1M1.1S");
	Duration q2("P2Y2M2DT2H2M2.123S");

	XMLBEANSXX_DEBUG(logger, "q1-->" + q1.toString());
	XMLBEANSXX_DEBUG(logger, "q2-->" + q2.toString());
	
	CPPUNIT_ASSERT(q1.toString() == "P1Y1M1DT1H1M1.001S");
	CPPUNIT_ASSERT(q2.toString() == "P2Y2M2DT2H2M2.123S");

	Duration q3("-P1YT3M");
	
	XMLBEANSXX_DEBUG(logger, "q3-->" + q3.toString());
	CPPUNIT_ASSERT(q3.toString() == "-P1YT3M");

	Duration q4;
	
	XMLBEANSXX_DEBUG(logger, "q4-->" + q4.toString());
	CPPUNIT_ASSERT(q4.toString() == "P");

	Calendar test(" 2000-01-12T12:13:14Z");
	XMLBEANSXX_DEBUG(logger, "test ->" + test.toXsdDateTime());
	Duration dur(" 	P1Y3M5DT7H10M3.3S ");
	XMLBEANSXX_DEBUG(logger, "dur  ->" + dur.toString());
	Calendar sol = test + dur;

	CPPUNIT_ASSERT(sol.toXsdDateTime() == "2001-04-17T19:23:17.003Z");

	XMLBEANSXX_DEBUG(logger, "sol = test + dur -> " + sol.toXsdDateTime());

	Calendar test2;
	test2.setYear(2000).setMonth(1).setGmtOff(1,43).dstOff();
	XMLBEANSXX_DEBUG(logger, "test2 ->" + test2.toXsdYearMonth());
	Duration dur2(" 	-P3M ");
	XMLBEANSXX_DEBUG(logger, "dur  ->" + dur2.toString());
	Calendar sol2 = test2 + dur2;

	XMLBEANSXX_DEBUG(logger, "sol2 = test2 + dur2 -> " + sol2.toXsdYearMonth());

	CPPUNIT_ASSERT(sol2.getYear() == 1999);
	CPPUNIT_ASSERT(sol2.getMonth() == 10);
	CPPUNIT_ASSERT(sol2.hasDay() == false);
	CPPUNIT_ASSERT(sol2.toXsdYearMonth() == "1999-10+01:43");

	Calendar test3;
	test3.setDate(" 1999-01-12");
	Duration dur3(" PT33H");

	XMLBEANSXX_DEBUG(logger, "test3 ->" + test3.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "dur  ->" + dur3.toString());
	Calendar sol3 = test3 + dur3;

	XMLBEANSXX_DEBUG(logger, "sol3 = test3 + dur3 -> " + sol3.toXsdDate());
	CPPUNIT_ASSERT(sol3.toXsdDate() == "1999-01-13");
	
	Calendar test4;
	test4.setDate(" 1978-03-31");
	Duration dur4(" P1M");

	XMLBEANSXX_DEBUG(logger, "test4 ->" + test4.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "dur  ->" + dur4.toString());
	Calendar sol4 = test4 +dur4;

	XMLBEANSXX_DEBUG(logger, "sol4 = test4 + dur4 -> " + sol4.toXsdDate());
	CPPUNIT_ASSERT(sol4.toXsdDate() == "1978-04-30");
	XMLBEANSXX_DEBUG(logger, "test4 ->" + test4.toXsdDate());

	Calendar cl1 = test4 + dur4;
	Calendar cl2 = test4 - dur4;
	XMLBEANSXX_DEBUG(logger, test4.toXsdDate() + " + " + dur4.toString() + " = " + cl1.toXsdDate());
	XMLBEANSXX_DEBUG(logger, test4.toXsdDate() + " - " + dur4.toString() + " = " + cl2.toXsdDate());

	CPPUNIT_ASSERT(cl2.toXsdDate() == "1978-02-28");

	//CPPUNIT_ASSERT (false);
	/*
	} catch (BeansException& e) {
		//std::cout << e.what();
	}*/
        
}

