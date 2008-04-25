#include "CalendarTest.h"

#include <fstream>
#include <string>
#include <sstream>
#include <xmlbeansxx/logger.h>
#include "xmlbeansxx/Calendar.h"
#include "xmlbeansxx/TextUtils.h"

#include "xmlbeansxx/BeansException.h"


CPPUNIT_TEST_SUITE_REGISTRATION (CalendarTest);

using namespace std;
using namespace xmlbeansxx;

XMLBEANSXX_LOGGER_PTR_SET(logger, "test.CalendarTest");

void CalendarTest::calendarTest () {

	XMLBEANSXX_DEBUG(logger, "++++++++++ Calendar Test ++++++++++");
	
	try {
		 
	Calendar c;
	c.setYear(1978).setMonth("   10 		");

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

	XMLBEANSXX_DEBUG(logger, "c.getDate(): " + boost::gregorian::to_simple_string(c.getDate()));
	XMLBEANSXX_DEBUG(logger, "c.getPtime(): " + boost::posix_time::to_simple_string(c.getPtime()));

	c.setFracSec(123);
	XMLBEANSXX_DEBUG(logger, "c.getPtime(): " + boost::posix_time::to_simple_string(c.getPtime()));

	XMLBEANSXX_DEBUG(logger, "+++++++ Time Zone Test +++++++");

	XMLBEANSXX_DEBUG(logger, "c.hasTimeZone: " + TextUtils::boolToString(c.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "c.isSetDst: " + TextUtils::boolToString(c.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "c.getUTCPtime: " + boost::posix_time::to_simple_string(c.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "c.getLocalPtime: " + boost::posix_time::to_simple_string(c.getLocalPtime()));
	
	c.setGmtOff(1, 0).dstUnknown();
	
	XMLBEANSXX_DEBUG(logger, "c.setGmtOff(1,0).dstUnknown()");

	XMLBEANSXX_DEBUG(logger, "c.hasTimeZone: " + TextUtils::boolToString(c.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "c.isSetDst: " + TextUtils::boolToString(c.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "c.getUTCPtime: " + boost::posix_time::to_simple_string(c.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "c.getLocalPtime: " + boost::posix_time::to_simple_string(c.getLocalPtime()));

	c.dstOn();
	XMLBEANSXX_DEBUG(logger, "c.dstOn()");

	XMLBEANSXX_DEBUG(logger, "c.hasTimeZone: " + TextUtils::boolToString(c.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "c.isSetDst: " + TextUtils::boolToString(c.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "c.getUTCPtime: " + boost::posix_time::to_simple_string(c.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "c.getLocalPtime: " + boost::posix_time::to_simple_string(c.getLocalPtime()));
	XMLBEANSXX_DEBUG(logger, "c.toXsdDateTime: " + c.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "c.toXsdDate: " + c.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "c.toXsdTime: " + c.toXsdTime());

	Calendar d("  \t\t\t\t\n\n\n\n\n  2008-12-12T02:22:22.123-12:33    \t  \n\n");
	XMLBEANSXX_DEBUG(logger, "Calendar d(\"2008-12-12T02:22:22.123-12:33\");");
	XMLBEANSXX_DEBUG(logger, "d.hasTimeZone: " + TextUtils::boolToString(d.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "d.isDstOn: " + TextUtils::boolToString(d.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "d.isSetDst: " + TextUtils::boolToString(d.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "d.getPtime: " + boost::posix_time::to_simple_string(d.getPtime()));
	XMLBEANSXX_DEBUG(logger, "d.getUTCPtime: " + boost::posix_time::to_simple_string(d.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "d.getLocalPtime: " + boost::posix_time::to_simple_string(d.getLocalPtime()));
	XMLBEANSXX_DEBUG(logger, "d.toXsdDateTime: " + d.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "d.toXsdDate: " + d.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "d.toXsdTime: " + d.toXsdTime());

	/*	
	Calendar e;
	e.setDate("   2008-12-12Z   ");
	XMLBEANSXX_DEBUG(logger, "e.hasTimeZone: " + TextUtils::boolToString(e.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "e.isDstOn: " + TextUtils::boolToString(e.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "e.isSetDst: " + TextUtils::boolToString(e.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "e.getBDate: " + boost::gregorian::to_simple_string(e.getBDate()));
	XMLBEANSXX_DEBUG(logger, "e.getBPtime: " + boost::posix_time::to_simple_string(e.getBPtime()));
	XMLBEANSXX_DEBUG(logger, "e.getUTCPtime: " + boost::posix_time::to_simple_string(e.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "e.getLocalPtime: " + boost::posix_time::to_simple_string(e.getLocalPtime()));
	XMLBEANSXX_DEBUG(logger, "e.toXsdDateTime: " + e.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDate: " + e.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "e.toXsdTime: " + e.toXsdTime());
	*/

	Calendar e(" 2008-12-12T12:12:12.121+12:21  ");
	/*
	XMLBEANSXX_DEBUG(logger, "e.hasTimeZone: " + TextUtils::boolToString(e.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "e.hasFullTimeInfo: " + TextUtils::boolToString(e.hasFullTimeInfo()));
	XMLBEANSXX_DEBUG(logger, "e.isDstOn: " + TextUtils::boolToString(e.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "e.isSetDst: " + TextUtils::boolToString(e.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "e.getBDate: " + boost::gregorian::to_simple_string(e.getBDate()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBDate: " + e.bDateToString());
	XMLBEANSXX_DEBUG(logger, "e.getBPtime: " + boost::posix_time::to_simple_string(e.getBPtime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBPtime: " + e.bPtimeToString());
	XMLBEANSXX_DEBUG(logger, "e.getUTCPtime: " + boost::posix_time::to_simple_string(e.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getUTCPtime: " + e.utcPtimeToString());
	XMLBEANSXX_DEBUG(logger, "e.getLocalPtime: " + boost::posix_time::to_simple_string(e.getLocalPtime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getLocalPtime: " + e.localPtimeToString());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDateTime: " + e.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDate: " + e.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "e.toXsdTime: " + e.toXsdTime());
	*/

	
	//e.setgDate("   2008-12-12+12:12   ");
	//e.setgYearMonth("   2008-12-12:22   ");
	//e.setgYear("   2008-12:22   ");
	//e.setgMonthDay("   --12-22Z   ");
	//e.setgDay(" 			  ---22-12:31   ");
	//e.setgMonth("   --12+12:12   ");
	/*
	XMLBEANSXX_DEBUG(logger, "e.hasTimeZone: " + TextUtils::boolToString(e.hasTimeZone()));
	XMLBEANSXX_DEBUG(logger, "e.isDstOn: " + TextUtils::boolToString(e.isDstOn()));
	XMLBEANSXX_DEBUG(logger, "e.isSetDst: " + TextUtils::boolToString(e.isSetDst()));
	XMLBEANSXX_DEBUG(logger, "e.getBDate: " + boost::gregorian::to_simple_string(e.getBDate()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBDate: " + e.bDateToString());
	XMLBEANSXX_DEBUG(logger, "e.getBPtime: " + boost::posix_time::to_simple_string(e.getBPtime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getBPtime: " + e.bPtimeToString());
	XMLBEANSXX_DEBUG(logger, "e.getUTCPtime: " + boost::posix_time::to_simple_string(e.getUTCPtime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getUTCPtime: " + e.utcPtimeToString());
	XMLBEANSXX_DEBUG(logger, "e.getLocalPtime: " + boost::posix_time::to_simple_string(e.getLocalPtime()));
	XMLBEANSXX_DEBUG(logger, "-->e.getLocalPtime: " + e.localPtimeToString());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDateTime: " + e.toXsdDateTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDate: " + e.toXsdDate());
	XMLBEANSXX_DEBUG(logger, "e.toXsdTime: " + e.toXsdTime());
	XMLBEANSXX_DEBUG(logger, "e.toXsdYearMonth: " + e.toXsdYearMonth());
	XMLBEANSXX_DEBUG(logger, "e.toXsdYear: " + e.toXsdYear());
	XMLBEANSXX_DEBUG(logger, "e.toXsdMonthDay: " + e.toXsdMonthDay());
	XMLBEANSXX_DEBUG(logger, "e.toXsdMonth: " + e.toXsdMonth());
	XMLBEANSXX_DEBUG(logger, "e.toXsdDay: " + e.toXsdDay());
	*/
	/*
	Calendar r;
	XMLBEANSXX_DEBUG(logger, "r.hasYear: " + TextUtils::boolToString(r.getYear()));
	r.setYear(1234);
	XMLBEANSXX_DEBUG(logger, "r.hasYear: " + TextUtils::boolToString(r.getYear()));
	XMLBEANSXX_DEBUG(logger, "r.getYear: " + TextUtils::intToString(r.getYear()));
	r.setYear(4321);
	XMLBEANSXX_DEBUG(logger, "r.hasYear: " + TextUtils::boolToString(r.getYear()));
	XMLBEANSXX_DEBUG(logger, "r.getYear: " + TextUtils::intToString(r.getYear()));
	*/
	XMLBEANSXX_DEBUG(logger, "========== DURATION ============");
	//Duration d1(" -P123Y12M21DT2H12.001S ");
	
	//XMLBEANSXX_DEBUG(logger, "-->" + d1.toString());

	Duration q1("P1Y1M1DT1H1M1.1S");
	Duration q2("P2Y2M2DT2H2M2.123S");

	XMLBEANSXX_DEBUG(logger, "q1-->" + q1.toString());
	XMLBEANSXX_DEBUG(logger, "q2-->" + q2.toString());
	Duration q4;
	XMLBEANSXX_DEBUG(logger, "q4-->" + q4.toString());

	boost::posix_time::time_duration td1(1,0,3,4);
	boost::posix_time::time_duration td2(-2,2,0,4);
	
	XMLBEANSXX_DEBUG(logger, "td1-->" + boost::posix_time::to_simple_string(td1));
	XMLBEANSXX_DEBUG(logger, "td2-->" + boost::posix_time::to_simple_string(td2));
	
	XMLBEANSXX_DEBUG(logger, "td1 + td2-->" + boost::posix_time::to_simple_string(td1 + td2));
	XMLBEANSXX_DEBUG(logger, "td1 - td2-->" + boost::posix_time::to_simple_string(td1 - td2));

	
	//CPPUNIT_ASSERT (false);
	} catch (BeansException& e) {
		//std::cout << e.what();
	}
        
}

