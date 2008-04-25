/*
    Copyright 2004-2005 TouK s.c.
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */


#ifndef _XMLBEANSXX_CALENDAR_H_
#define _XMLBEANSXX_CALENDAR_H_

#include <string>
#include <ctime>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "logger.h"

namespace xmlbeansxx {
			
XMLBEANSXX_LOGGER_PTR_SET(Calendar_log, "xmlbeansxx.Calendar");

class Calendar {
	public:
		Calendar();
		Calendar(boost::gregorian::date);
		Calendar(boost::posix_time::ptime);
		Calendar(std::string); //xsd:DateTime

		Calendar& setDate(std::string); //xsd:Date
		Calendar& setTime(std::string); //xsd:Time
		Calendar& setYearMonth(std::string); //xsd:YearMonth
		Calendar& setgYear(std::string); //xsd:Year
		Calendar& setMonthDay(std::string); //xsd:MonthDay
		Calendar& setgMonth(std::string); //xsd:Month
		Calendar& setgDay(std::string); //xsd:Day

		Calendar& setYear(int);
		Calendar& setYear(std::string);
		int getYear();
		bool hasYear();

		Calendar& setMonth(int);
		Calendar& setMonth(std::string);
		int getMonth();
		bool hasMonth();

		Calendar& setDay(int);
		Calendar& setDay(std::string);
		int getDay();
		bool hasDay();

		Calendar& setHour(int);
		Calendar& setHour(std::string);
		int getHour();
		bool hasHour();

		Calendar& setMinutes(int);
		Calendar& setMinutes(std::string);
		int getMinutes();
		bool hasMinutes();

		Calendar& setSeconds(int);
		Calendar& setSeconds(std::string);
		int getSeconds();
		bool hasSeconds();

		Calendar& setFracSec(int);
		Calendar& setFracSec(std::string);
		int getFracSec();
		bool hasFracSec();

		bool hasFullTimeInfo();
		bool hasFullDateInfo();

		Calendar& setGmtOff(int, int);
		bool hasTimeZone();

		Calendar& dstOn();
		Calendar& dstOff();
		Calendar& dstUnknown();
		bool isDstOn();
		bool isSetDst();
		int getDst(); 
		
		boost::gregorian::date getDate();
		boost::posix_time::ptime getPtime();
		boost::posix_time::ptime getUTCPtime();
		boost::posix_time::ptime getLocalPtime();
		
		std::string dateToString();
		std::string ptimeToString();
		std::string utcPtimeToString();
		std::string localPtimeToString();

		std::string toXsdDateTime();
		std::string toXsdDate();
		std::string toXsdTime();
		std::string toXsdYearMonth();
		std::string toXsdYear();
		std::string toXsdMonthDay();
		std::string toXsdDay();
		std::string toXsdMonth();
		
		inline operator boost::posix_time::ptime() {
			return getPtime();
		}

	private:
		struct tm cal_tm;
		int frac_sec;
		int gmt_off_hours;
		int gmt_off_minutes;
		int flags;
		void fixTm();
		std::string timeZoneToString();

};


class Duration {
	public:
		Duration();
		Duration(std::string); 
		
		Duration& setYears(int);
		Duration& setYears(std::string);
		int getYears();
		bool hasYears();

		Duration& setMonths(int);
		Duration& setMonths(std::string);
		int getMonths();
		bool hasMonths();
		
		Duration& setDays(int);
		Duration& setDays(std::string);
		int getDays();
		bool hasDays();

		Duration& setHours(int);
		Duration& setHours(std::string);
		int getHours();
		bool hasHours();
		
		Duration& setMinutes(int);
		Duration& setMinutes(std::string);
		int getMinutes();
		bool hasMinutes();

		Duration& setSeconds(int);
		Duration& setSeconds(std::string);
		int getSeconds();
		bool hasSeconds();
		
		Duration& setFracSec(int);
		Duration& setFracSec(std::string);
		int getFracSec();
		bool hasFracSec();

		bool isNeg();
		std::string toString();
		
	private:
		int years;
		int months;
		int days;
		int hours;
		int minutes;
		int seconds;
		int frac_sec;

		bool neg;

		int flags;
		
};


}


#endif //_XMLBEANSXX_CALENDAR_H_

