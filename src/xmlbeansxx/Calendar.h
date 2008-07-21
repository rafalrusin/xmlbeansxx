/*
    Copyright 2004-2008 TouK sp. z o.o. s.k.a.
    
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
			

class Duration;

class Calendar {
	XMLBEANSXX_STATIC_LOGGER_PTR(Calendar_log);

	public:
		Calendar();
		Calendar(const boost::gregorian::date&);
		Calendar(const boost::posix_time::ptime&);
		Calendar(const std::string&); //xsd:DateTime

		Calendar& setDate(const std::string&); //xsd:Date
		Calendar& setTime(const std::string&); //xsd:Time
		Calendar& setYearMonth(const std::string&); //xsd:YearMonth
		Calendar& setgYear(const std::string&); //xsd:Year
		Calendar& setMonthDay(const std::string&); //xsd:MonthDay
		Calendar& setgMonth(const std::string&); //xsd:Month
		Calendar& setgDay(const std::string&); //xsd:Day

		Calendar& setYear(int);
		Calendar& setYear(const std::string&);
		int getYear() const;
		bool hasYear() const;

		Calendar& setMonth(int);
		Calendar& setMonth(const std::string&);
		int getMonth() const;
		bool hasMonth() const;

		Calendar& setDay(int);
		Calendar& setDay(const std::string&);
		int getDay() const;
		bool hasDay() const;

		Calendar& setHour(int);
		Calendar& setHour(const std::string&);
		int getHour() const;
		bool hasHour() const;

		Calendar& setMinutes(int);
		Calendar& setMinutes(const std::string&);
		int getMinutes() const;
		bool hasMinutes() const;

		Calendar& setSeconds(int);
		Calendar& setSeconds(const std::string&);
		int getSeconds() const;
		bool hasSeconds() const;

		Calendar& setFracSec(int);
		Calendar& setFracSec(const std::string&);
		int getFracSec() const;
		bool hasFracSec() const;

		bool hasFullTimeInfo() const;
		bool hasFullDateInfo() const;

		Calendar& setGmtOff(int, int);
		bool hasTimeZone() const;

		Calendar& dstOn();
		Calendar& dstOff();
		Calendar& dstUnknown();
		bool isDstOn() const;
		bool isSetDst() const;
		int getDst() const; 
		
		boost::gregorian::date getDate() const;
		boost::posix_time::ptime getTime() const;
		boost::posix_time::time_duration getTimeDuration() const;
		boost::posix_time::ptime getUTCTime() const;
		boost::posix_time::ptime getLocalTime() const;
		
		std::string dateToString() const;
		std::string timeToString() const;
		std::string utcTimeToString() const;
		std::string localTimeToString() const;

		std::string toXsdDateTime() const;
		std::string toXsdDate() const;
		std::string toXsdTime() const;
		std::string toXsdYearMonth() const;
		std::string toXsdYear() const;
		std::string toXsdMonthDay() const;
		std::string toXsdDay() const;
		std::string toXsdMonth() const;

		std::string toString() const;

		inline operator boost::posix_time::ptime() const{
			return getTime();
		}

		Calendar &operator= (const Calendar &);
		Calendar &operator+ (const Duration &);
		Calendar &operator- (const Duration &);

		Calendar &timeZoneGuess();

	private:
		struct tm cal_tm;
		int frac_sec;
		int gmt_off_hours;
		int gmt_off_minutes;
		int flags;

		void fixTm();
		std::string timeZoneToString() const;
		Calendar &applyDuration(xmlbeansxx::Duration);
		Calendar(struct tm, int /*frac_sec*/, int /*gmt_off_hours*/, int /*gmt_off_minutes*/, int, int /*flags*/);
		boost::posix_time::ptime timeZoneMagic(const boost::posix_time::ptime&);
};


class Duration {
	public:
		Duration();
		Duration(const std::string&); 
		Duration(const boost::posix_time::time_duration&); 
		
		Duration& setYears(int);
		Duration& setYears(const std::string&);
		int getYears();
		bool hasYears();

		Duration& setMonths(int);
		Duration& setMonths(const std::string&);
		int getMonths();
		bool hasMonths();
		
		Duration& setDays(int);
		Duration& setDays(const std::string&);
		int getDays();
		bool hasDays();

		Duration& setHours(int);
		Duration& setHours(const std::string&);
		int getHours();
		bool hasHours();
		
		Duration& setMinutes(int);
		Duration& setMinutes(const std::string&);
		int getMinutes();
		bool hasMinutes();

		Duration& setSeconds(int);
		Duration& setSeconds(const std::string&);
		int getSeconds();
		bool hasSeconds();
		
		Duration& setFracSec(int);
		Duration& setFracSec(const std::string&);
		int getFracSec();
		bool hasFracSec();

		bool hasFullDateInfo();
		bool hasFullTimeInfo();

		Duration& setNeg(bool);
		bool isNeg() const;
		std::string toString();

		Duration &operator= (const Duration &);
		//Duration &operator+ (const Duration &);
		
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

		Duration(int, int, int, int, int, int, int, bool, int);
};

}


#endif //_XMLBEANSXX_CALENDAR_H_

