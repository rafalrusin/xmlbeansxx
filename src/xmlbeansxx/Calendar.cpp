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


#include "Calendar.h"

#include "TextUtils.h"
#include "BeansException.h"

#include <iostream>
#include <sstream>


using namespace xmlbeansxx;


XMLBEANSXX_LOGGER_PTR_SET(Calendar::Calendar_log, "xmlbeansxx.Calendar");


static int has_day = 1;
static int has_month = 2;
static int has_year = 4;
static int has_hour = 8;
static int has_minutes = 16;
static int has_seconds = 32;
static int has_fracsec = 64;
static int has_timezone = 128;
/*
struct tm {
	int tm_sec; 	//seconds after the minute 	0-61 tm_sec is generally 0-59. Extra range to accommodate for leap seconds in certain systems.
	int tm_min; 	//minutes after the hour 	0-59
	int tm_hour; 	//hours since midnight 		0-23
	int tm_mday; 	//day of the month 			1-31
	int tm_mon; 	//months since January 		0-11
	int tm_year; 	//years since 1900
	int tm_wday; 	//days since Sunday 		0-6
	int tm_yday; 	//days since January 1 		0-365
	int tm_isdst; 	//Daylight Saving Time flags  
					//	tm_isdst is greater than zero if Daylight Saving Time is in effect, 
					//	zero if Daylight Saving Time is not in effect, 
					//	and less than zero if the information is not available.
						
	//Only in linux:				
						
	long int tm_gmtoff 
					//This field describes the time zone that was used to compute this broken-down time value; 
					//it is the amount you must add to the local time in that zone to get GMT, in units of seconds. 
					//The value is like that of the variable timezone (see section Functions and Variables for Time Zones). 
					//You can also think of this as the "number of seconds west" of GMT. The tm_gmtoff field is a GNU library extension.
	cnst char *tm_zone
					//This field is the three-letter name for the time zone that was used to compute this broken-down time value. 
					//It is a GNU library extension.
}
*/


bool isLeapYear(int year) {
	return ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)));
}

int maximumDayInMonth(int year, int month) {
	switch (month) {
		 case 4:
		 case 6:
		 case 9:
		 case 11:
			 return 30;
		 case 2:
			 return (isLeapYear(year) ? 29 : 28);
		 default:
			 return 31;
	}
}

int getInt(std::string sint) {
	std::string tmp = TextUtils::collapse(sint);
	int len = tmp.length();

	if (!TextUtils::areDigits(tmp))
		throw CalendarException("Not all requested signs are digits!");
	
	std::stringstream ss;
	ss << tmp;
	ss >> len;

	return len;
}


Calendar::Calendar() {
	flags = 0;
	cal_tm.tm_isdst = -1;
}

Calendar::Calendar(const boost::gregorian::date &date) {
	flags = 0;
	if (!date.is_special()) {
		cal_tm = to_tm(date);
		flags |= has_year;
		flags |= has_month;
		flags |= has_day;
	} else
		throw CalendarException("Bad date format");
}

Calendar::Calendar(const boost::posix_time::ptime &ptime) {
	flags = 0;
	if (!ptime.is_special()) {
		cal_tm = to_tm(ptime);
		flags |= has_year;
		flags |= has_month;
		flags |= has_day;
		flags |= has_hour;
		flags |= has_minutes;
		flags |= has_seconds;
		boost::posix_time::time_duration td = ptime.time_of_day();
		if (td.fractional_seconds() != 0) {
			flags |= has_fracsec;
			//frac_sec = td.fractional_seconds() / (10 * td.num_fractional_digits()); //TODO
			frac_sec = td.fractional_seconds() / 1000;
			//XMLBEANSXX_DEBUG(Calendar_log, ">>>>" + TextUtils::intToString(frac_sec));
		}
	} else
		throw CalendarException("Bad time format");
}

Calendar::Calendar(const std::string &str) { 
	//str = CCYY-MM-DDThh:mm:ss(.sss)(Z|(+|-)hh:mm)
	XMLBEANSXX_DEBUG(Calendar_log, "creating Calendar from str: " + str);
	flags = 0;
	const char * sign = "?";
	std::string pt, frac, tz;
	
	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else if (pt.rfind('-') > pt.find('T'))
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}
	
	if (pt.rfind('.') != std::string::npos) {
		frac = pt.substr(pt.rfind('.') + 1, pt.length());
		pt = pt.substr(0, pt.rfind('.'));
	}

	while (pt.find('-') != std::string::npos) //no B.C. ;-)
		pt = pt.erase(pt.find('-'), 1); 
	
	while (pt.find(':') != std::string::npos) 
		pt = pt.erase(pt.find(':'), 1);
	
	boost::posix_time::ptime ptime(boost::posix_time::from_iso_string(pt));

	if (!ptime.is_special()) {
		cal_tm = to_tm(ptime);
		flags |= has_year;
		flags |= has_month;
		flags |= has_day;
		flags |= has_hour;
		flags |= has_minutes;
		flags |= has_seconds;
		fixTm();
	} 

	if (!frac.empty()) {
		int fsec = getInt(frac);
		if (fsec >= 0 && fsec < 1000) {
			frac_sec = fsec;	 
			flags |= has_fracsec;
		} else
			throw CalendarException("Bad FracSec value!");
	}

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
}

Calendar& Calendar::setDate(const std::string &str) {
	//str = CCYY-MM-DD(Z|(+|-)hh:mm)
	const char * sign = "?";
	std::string pt, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') - pt.find('-') == 2)
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}

	while (pt.find('-') != std::string::npos) //no B.C. ;-)
		pt = pt.erase(pt.find('-'), 1); 

	boost::gregorian::date date(boost::gregorian::from_undelimited_string(pt));

	if (!date.is_special()) {
		tm tmp = cal_tm;
		cal_tm = to_tm(date);
		flags |= has_year;
		flags |= has_month;
		flags |= has_day;

		cal_tm.tm_sec = tmp.tm_sec;
		cal_tm.tm_min = tmp.tm_min;
		cal_tm.tm_hour = tmp.tm_hour;
		cal_tm.tm_isdst = tmp.tm_isdst;

		fixTm();
	}

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}

	return *this;
}


Calendar& Calendar::setTime(const std::string &str) { 
	//str = hh:mm:ss(.sss)(Z|(+|-)hh:mm)
	XMLBEANSXX_DEBUG(Calendar_log, "setting Time from str: " + str);

	const char * sign = "?";
	std::string pt, frac, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') != std::string::npos)
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}

	if (pt.rfind('.') != std::string::npos) {
		frac = pt.substr(pt.rfind('.') + 1, pt.length());
		pt = pt.substr(0, pt.rfind('.'));
	}

	XMLBEANSXX_DEBUG(Calendar_log, "pt,tz,frac  str: " + pt + "," + tz + "," + frac);
	
	boost::posix_time::time_duration td(boost::posix_time::duration_from_string(pt));

	if (!td.is_special()) {
		tm tmp = cal_tm;
		cal_tm = to_tm(td);
		flags |= has_hour;
		flags |= has_minutes;
		flags |= has_seconds;
		
		cal_tm.tm_mday = tmp.tm_mday;
		cal_tm.tm_wday = tmp.tm_wday;
		cal_tm.tm_yday = tmp.tm_yday;
		cal_tm.tm_mon = tmp.tm_mon;
		cal_tm.tm_year = tmp.tm_year;
		cal_tm.tm_isdst = tmp.tm_isdst;
	}
	XMLBEANSXX_DEBUG(Calendar_log, "2pt,tz,frac  str: " + pt + "," + tz + "," + frac);

	if (!frac.empty()) {
		int fsec = getInt(frac);
		if (fsec >= 0 && fsec < 1000) {
			frac_sec = fsec;	 
			flags |= has_fracsec;
		} else
			throw CalendarException("Bad FracSec value!");
	}

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	XMLBEANSXX_DEBUG(Calendar_log, "3pt,tz,frac  str: " + pt + "," + tz + "," + frac);
	return *this;
}

Calendar& Calendar::setYearMonth(const std::string &str) {
	//str = CCYY-MM(Z|(+|-)hh:mm)
	const char * sign = "?";
	std::string pt, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') != pt.find('-'))
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}

	setYear(getInt(pt.substr(0, pt.find('-'))));
	flags |= has_year;
	setMonth(getInt(pt.substr(pt.find('-') + 1, pt.length())));
	flags |= has_month;
	fixTm();

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgYear(const std::string &str) {
	//str = CCYY(Z|(+|-)hh:mm)
	const char * sign = "?";
	std::string pt, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') != 1)
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}

	setYear(getInt(pt));
	flags |= has_year;
	fixTm();

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setMonthDay(const std::string &str) {
	//str = --MM-DD(Z|(+|-)hh:mm)
	const char * sign = "?";
	std::string pt, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') - pt.find('-') == 7)
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}
	
	setMonth(getInt(pt.substr(2, 2)));
	flags |= has_month;
	setDay(getInt(pt.substr(5, 6)));
	flags |= has_day;
	fixTm();

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgMonth(const std::string &str) {
	//str = --MM(Z|(+|-)hh:mm)
	const char * sign = "?";
	std::string pt, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') - pt.find('-') == 7)
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}

	setMonth(getInt(pt.substr(2, pt.length())));
	flags |= has_month;
	fixTm();

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgDay(const std::string &str) {
	//str = ---DD(Z|(+|-)hh:mm)
	const char * sign = "?";
	std::string pt, tz;

	pt = TextUtils::collapse(str);

	if (pt.rfind('+') != std::string::npos)
		sign = "+";
	else if (pt.rfind('Z') != std::string::npos)
		sign = "Z";
	else 	if (pt.rfind('-') - pt.find('-') == 5)
		sign = "-";

	if (sign != "?") {
		tz = pt.substr(pt.rfind(sign), pt.length());
		pt = pt.substr(0, pt.rfind(sign));
	}
	setDay(getInt(pt.substr(3, pt.length())));
	flags |= has_day;
	fixTm();

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else {
			if (tz[3] != ':' || tz.length() != 6) 
				throw CalendarException("Bad TimeZone format!");
			if (sign == "+") {
				gmt_off_hours = getInt(tz.substr(1,2)); 
				gmt_off_minutes = getInt(tz.substr(4,5)); 
			} else {
				gmt_off_hours = -getInt(tz.substr(1,2)); //- 
				gmt_off_minutes = -getInt(tz.substr(4,5)); //-
			}
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setYear(int year) {
	if (year < -4713 || year > 9999999) 
		throw CalendarException("Bad year value!");
	if (year == 0) 
		throw CalendarException("There's no 0 year!");
	cal_tm.tm_year = year - 1900;
	flags |= has_year;
	fixTm();
	return *this;
}

Calendar& Calendar::setYear(const std::string &year) {
	return setYear(getInt(year));
}

int Calendar::getYear() const {
	if (hasYear())
		return 1900 + cal_tm.tm_year;
	return 0;
}

bool Calendar::hasYear() const {
	return ((flags & has_year) != 0); 
}

Calendar& Calendar::setMonth(int month) {
	if (month > 0 && month < 13) {
		cal_tm.tm_mon = month - 1;
		flags |= has_month;
		fixTm();
	} else
		throw CalendarException("Bad month value!");
	return *this;
}

Calendar& Calendar::setMonth(const std::string &month) {
	return setMonth(getInt(month));
}

int Calendar::getMonth() const {
	if (hasMonth())	
		return cal_tm.tm_mon + 1;
	return 0;
}

bool Calendar::hasMonth() const {
	 return ((flags & has_month) != 0);
}

Calendar& Calendar::setDay(int day) {
	if (day > 0 && day < 32) {
		cal_tm.tm_mday = day;	 
		flags |= has_day;
		fixTm();
	} else
		throw CalendarException("Bad day value!");
	return *this;
}

Calendar& Calendar::setDay(const std::string &day) {
	return setDay(getInt(day));
}

int Calendar::getDay() const {
	if (hasDay()) 
		return cal_tm.tm_mday;
	return 0;
}

bool Calendar::hasDay() const {
	return ((flags & has_day) != 0);	
}

Calendar& Calendar::setHour(int hour) {
	if (hour >= 0 && hour < 25) {
		cal_tm.tm_hour = hour;	 
		flags |= has_hour;
	} else
		throw CalendarException("Bad hour value!");
	return *this;
}

Calendar& Calendar::setHour(const std::string &hour) {
	return setHour(getInt(hour));
}

int Calendar::getHour() const {
	if (hasHour()) 
		return cal_tm.tm_hour;
	return 0;
}

bool Calendar::hasHour() const {
	return ((flags & has_hour) != 0);	
}

Calendar& Calendar::setMinutes(int min) {
	if (min >= 0 && min < 61) {
		cal_tm.tm_min = min;	 
		flags |= has_minutes;
	} else
		throw CalendarException("Bad minutes value!");
	return *this;
}

Calendar& Calendar::setMinutes(const std::string &min) {
	return setMinutes(getInt(min));
}

int Calendar::getMinutes() const {
	if (hasMinutes()) 
		return cal_tm.tm_min;
	return 0;
}

bool Calendar::hasMinutes() const {
	return ((flags & has_minutes) != 0);	
}

Calendar& Calendar::setSeconds(int sec) {
	if (sec >= 0 && sec < 61) {
		cal_tm.tm_sec = sec;	 
		flags |= has_seconds;
	} else
		throw CalendarException("Bad seconds value!");
	return *this;
}

Calendar& Calendar::setSeconds(const std::string &sec) {
	return setSeconds(getInt(sec));
}

int Calendar::getSeconds() const {
	if (hasSeconds()) 
		return cal_tm.tm_sec;
	return 0;
}

bool Calendar::hasSeconds() const {
	return ((flags & has_seconds) != 0);	
}

Calendar& Calendar::setFracSec(int fsec) {
	if (fsec >= 0 && fsec < 1000) {
		frac_sec = fsec;	 
		flags |= has_fracsec;
	} else
		throw CalendarException("Bad fracsec value!");
	return *this;
}

Calendar& Calendar::setFracSec(const std::string &fsec) {
	return setFracSec(getInt(fsec));
}

int Calendar::getFracSec() const {
	if (hasFracSec()) 
		return frac_sec;
	return 0;
}

bool Calendar::hasFracSec() const {
	return ((flags & has_fracsec) != 0);	
}

bool Calendar::hasFullDateInfo() const {
	return hasYear() && hasMonth() && hasDay();	 
}

bool Calendar::hasFullTimeInfo() const {
	return hasHour() && hasMinutes() && hasSeconds();
}

void Calendar::fixTm() {
	tm tmp = cal_tm;
	if (hasFullDateInfo()) {
		boost::posix_time::ptime p(boost::gregorian::date(getYear(), getMonth(), getDay()));
		cal_tm = to_tm(p); 
		cal_tm.tm_sec = tmp.tm_sec;
		cal_tm.tm_min = tmp.tm_min;
		cal_tm.tm_hour = tmp.tm_hour;
		cal_tm.tm_isdst = tmp.tm_isdst;
	};
}

boost::gregorian::date Calendar::getDate() const {
	boost::gregorian::date ret = boost::gregorian::date(boost::gregorian::not_a_date_time);
	if (hasFullDateInfo()) 
		ret = boost::gregorian::date(getYear(), getMonth(), getDay());
	return ret;
}

std::string Calendar::dateToString() const {
	return boost::gregorian::to_iso_extended_string(getDate()); 
}

boost::posix_time::ptime Calendar::getTime() const {
	if (hasFullDateInfo() && hasFullTimeInfo()) 
		return boost::posix_time::ptime(
				getDate(),
				boost::posix_time::hours(getHour()) +
				boost::posix_time::minutes(getMinutes()) +
				boost::posix_time::seconds(getSeconds()) +
				boost::posix_time::millisec(getFracSec())
			);
	else
		return boost::posix_time::ptime(boost::gregorian::not_a_date_time);
}

boost::posix_time::time_duration Calendar::getTimeDuration() const {
	if (hasFullTimeInfo()) {
		return boost::posix_time::time_duration(
				getHour(),
				getMinutes(),
				getSeconds(),
				getFracSec()
				);	 
	}
	return boost::posix_time::time_duration();
};




std::string Calendar::timeToString() const {
	return boost::posix_time::to_iso_extended_string(getTime());	 
}

Calendar& Calendar::dstOn() {
	cal_tm.tm_isdst = 1; 
	return *this;
}

Calendar& Calendar::dstOff() {
	cal_tm.tm_isdst = 0; 
	return *this;
}

Calendar& Calendar::dstUnknown() {
	cal_tm.tm_isdst = -1;
	return *this;
}

int Calendar::getDst() const {
	return cal_tm.tm_isdst;
}

bool Calendar::isDstOn() const { 
	if (getDst() == 1) 
		return true;
	return false;
}

bool Calendar::isSetDst() const {
	if ((getDst() == 1) || (getDst() == 0))
		return true;
	return false;
}

Calendar& Calendar::setGmtOff(int hour, int minutes) { 
	// setGmtOff(1,	1) 		-> +01:01 
	// setGmtOff(-2, -30) 	-> -2:30
	// setGmtOff(0, 0) 		-> Z
	if (hour * minutes < 0) 
		throw CalendarException("Bad GMT offset!");
	gmt_off_hours = hour; 
	gmt_off_minutes = minutes; 
	flags |= has_timezone;
	return *this;
}

std::string Calendar::timeZoneToString() const { 
	std::string ret = "";
	std::stringstream ss;

	if (hasTimeZone()) {
		if (gmt_off_hours == 0 && gmt_off_minutes == 0 && (getDst() == 0)) 
			ret += "Z";
		else {
			int tmp = gmt_off_hours;
			int tmp2 = gmt_off_minutes;
			if (isDstOn()) 
				tmp += 1;
			if (tmp >= 0) 
				ss << "+";
			else {
				ss << "-";
				tmp = -tmp;
				tmp2 = -tmp2;
			}
			ss << std::setw(2) << std::setfill('0') << tmp << ":" << std::setw(2) << std::setfill('0') << tmp2;
			ss >> ret;
		}
	}
	return ret;
}

bool Calendar::hasTimeZone() const {
	return ((flags & has_timezone) != 0);
}

boost::posix_time::ptime Calendar::getUTCTime() const {
	boost::posix_time::ptime tmp = boost::posix_time::ptime(boost::gregorian::not_a_date_time);

	if (hasFullDateInfo() && hasFullTimeInfo())
		if (hasTimeZone()) {
			tmp = getTime() - boost::posix_time::hours(gmt_off_hours) - boost::posix_time::minutes(gmt_off_minutes);
			if (isDstOn()) 
				tmp = tmp - boost::posix_time::hours(1);
		}  
	return tmp;
}

std::string Calendar::utcTimeToString() const {
	return boost::posix_time::to_iso_extended_string(getUTCTime());
}

boost::posix_time::ptime Calendar::getLocalTime() const {
	boost::posix_time::ptime tmp = boost::posix_time::ptime(boost::gregorian::not_a_date_time);

	if (hasFullDateInfo() && hasFullTimeInfo())
		if (hasTimeZone()) 
			tmp = getTime();

	return tmp;
}

std::string Calendar::localTimeToString() const {
	return boost::posix_time::to_iso_extended_string(getLocalTime());
}

std::string Calendar::toXsdDateTime() const {
	std::string ret("not-a-date-time");
	if (hasFullDateInfo() && hasFullTimeInfo()) {
		ret = boost::gregorian::to_iso_extended_string(
				boost::gregorian::date(getYear(), getMonth(), getDay()));
	
		ret += "T";
		std::stringstream ss;	
		std::string ret2;
		ss << std::setw(2) << std::setfill('0') << getHour();
		ss << ":";
		ss << std::setw(2) << std::setfill('0') << getMinutes();
		ss << ":";
		ss << std::setw(2) << std::setfill('0') << getSeconds();
		if (hasFracSec()) {
			ss << ".";
			ss << std::setw(3) << std::setfill('0') << getFracSec();
		}
		ss >> ret2;
		ret += ret2;
		ret += timeZoneToString();
}
	return ret;
}

std::string Calendar::toXsdDate() const { 
	std::string ret("not-a-date-time");
	if (hasFullDateInfo()) {
		boost::gregorian::date tmp(getYear(), getMonth(), getDay());
		ret = boost::gregorian::to_iso_extended_string(tmp);		 
		if (hasTimeZone())
			ret += timeZoneToString();
	}		 

	return ret;
}

std::string Calendar::toXsdTime() const { 
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasFullTimeInfo()) {
		ss << std::setw(2) << std::setfill('0') << getHour();
		ss << ":";
		ss << std::setw(2) << std::setfill('0') << getMinutes();
		ss << ":";
		ss << std::setw(2) << std::setfill('0') << getSeconds();
		if (hasFracSec()) {
			ss << ".";
			ss << std::setw(3) << std::setfill('0') << getFracSec();
		}
		if (hasTimeZone())
			ss << timeZoneToString();
	}
	ss >> ret;
	return ret;
}

std::string Calendar::toXsdYearMonth() const {
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasYear() && hasMonth()) {
		ss << getYear();
		ss << "-";
		ss << std::setw(2) << std::setfill('0') << getMonth();
		if (hasTimeZone())
			ss << timeZoneToString();
	}
	ss >> ret;
	return ret;
}

std::string Calendar::toXsdYear() const {
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasYear()) {
		ss << getYear();
		if (hasTimeZone())
			ss << timeZoneToString();
	}
	ss >> ret;
	return ret;
}

std::string Calendar::toXsdDay() const {
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasDay()) {
		ss << "---";
		ss << getDay();
		if (hasTimeZone())
			ss << timeZoneToString();
	}
	ss >> ret;
	return ret;
}

std::string Calendar::toXsdMonth() const {
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasMonth()) {
		ss << "--";
		ss << getMonth();
		if (hasTimeZone())
			ss << timeZoneToString();
	}
	ss >> ret;
	return ret;
}

std::string Calendar::toXsdMonthDay() const {
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasMonth() && hasDay()) {
		ss << "--";
		ss << getMonth();
		ss << "-";
		ss << getDay();
		if (hasTimeZone())
			ss << timeZoneToString();
	}
	ss >> ret;
	return ret;
}

boost::posix_time::ptime Calendar::timeZoneMagic(const boost::posix_time::ptime& t) {
	boost::gregorian::date time_t_start_day(1970, 1, 1);
	boost::posix_time::ptime time_t_start_time(time_t_start_day, boost::posix_time::time_duration(0, 0, 0));
	if (t < time_t_start_time)
		throw std::out_of_range("Cannot convert dates prior to Jan 1, 1970");
	
	boost::gregorian::date_duration dd = t.date() - time_t_start_day;
	boost::posix_time::time_duration td = t.time_of_day();
	std::time_t t2 = dd.days()*86400 + td.hours()*3600 + td.minutes()*60 + td.seconds();
	std::tm tms, *tms_ptr;
	tms_ptr = boost::date_time::c_time::gmtime(&t2, &tms);
	tms_ptr->tm_isdst = -1;
	std::time_t t3 = ::mktime(tms_ptr);
	tms_ptr = boost::date_time::c_time::gmtime(&t3,&tms);
	boost::gregorian::date d(static_cast<unsigned short>(tms_ptr->tm_year + 1900),
			                       static_cast<unsigned short>(tms_ptr->tm_mon + 1),
														 static_cast<unsigned short>(tms_ptr->tm_mday));
	boost::posix_time::time_duration td2(tms_ptr->tm_hour,
																			 tms_ptr->tm_min,
																			 tms_ptr->tm_sec,
																			 t.time_of_day().fractional_seconds());

	return boost::posix_time::ptime(d,td2);
}
	 
Calendar& Calendar::timeZoneGuess() {
	if (hasFullDateInfo() && hasFullTimeInfo() && !hasTimeZone()) {
		//Guess time zone offset
		xmlbeansxx::Calendar pom(timeZoneMagic(getTime()));
		boost::posix_time::time_duration td = getTime() - pom.getTime();
		setGmtOff(td.hours(), td.minutes()).dstOff();
	} 
	return *this;
}

Duration::Duration() {
	flags = 0;	 
	neg = false;
}
	
Duration::Duration(const std::string &dur) {
	//(-)P(nY)(nM)(nD)T(nH)(nM)(nS)
	flags = 0;
	std::string tmp = TextUtils::collapse(dur);

	if (tmp.find('-') == 0) {
		neg = true;
		tmp = tmp.substr(1, tmp.length());
	} else
		neg = false;

	if (tmp.find('P') == 0) 
		tmp = tmp.substr(1, tmp.length());
	else
		throw CalendarException("Bad duration format");

	if (tmp.find('T') != 0) 
		if (tmp.find('Y') != std::string::npos) {
			setYears(tmp.substr(0, tmp.find('Y')));
			tmp = tmp.substr(tmp.find('Y') + 1, tmp.length());
		}

	if (tmp.find('T') != 0)
		if (tmp.find('M') != std::string::npos) {
			setMonths(tmp.substr(0, tmp.find('M')));
			tmp = tmp.substr(tmp.find('M') + 1, tmp.length());
		}
	
	if (tmp.find('T') != 0)
		if (tmp.find('D') != std::string::npos) {
			setDays(tmp.substr(0, tmp.find('D')));
			tmp = tmp.substr(tmp.find('D') + 1, tmp.length());
		}

	if (!tmp.empty()) {
		if (tmp.find('T') == 0) {
			tmp = tmp.substr(1, tmp.length());
			if (tmp.empty())
				throw CalendarException("Bad duration format");
		}
		else
			throw CalendarException("Bad duration format");

		if (tmp.find('H') != std::string::npos) {
			setHours(tmp.substr(0, tmp.find('H')));
			tmp = tmp.substr(tmp.find('H') + 1, tmp.length());
		}

		if (tmp.find('M') != std::string::npos) {
			setMinutes(tmp.substr(0, tmp.find('M')));
			tmp = tmp.substr(tmp.find('M') + 1, tmp.length());
		}

		if (tmp.find('S') != std::string::npos) {
			if (tmp.find('.') != std::string::npos) {
				setFracSec(tmp.substr(tmp.find('.') + 1, tmp.find('S') - tmp.find('.') - 1));
				tmp = tmp.substr(0, tmp.find('.'));
			}
			setSeconds(tmp.substr(0, tmp.find('S')));
		}
	}
}

Duration& Duration::setYears(int y) {
	years = y;
	flags |= has_year;
	return *this;
}

Duration& Duration::setYears(const std::string &y) {
	return setYears(getInt(y));
}

int Duration::getYears() {
	if (hasYears())
		return years;
	return 0;
}

bool Duration::hasYears() {
	return ((flags & has_year) != 0);	 
}

Duration& Duration::setMonths(int m) {
	months = m;
	flags |= has_month;
	return *this;
}

Duration& Duration::setMonths(const std::string &m) {
	return setMonths(getInt(m));
}

int Duration::getMonths() {
	if (hasMonths())
		return months;
	return 0;
}

bool Duration::hasMonths() {
	return ((flags & has_month) != 0);	 
}

Duration& Duration::setDays(int d) {
	days = d;
	flags |= has_day;
	return *this;
}

Duration& Duration::setDays(const std::string &d) {
	return setDays(getInt(d));
}

int Duration::getDays() {
	if (hasDays())
		return days;
	return 0;
}

bool Duration::hasDays() {
	return ((flags & has_day) != 0);	 
}

Duration& Duration::setHours(int h) {
	hours = h;
	flags |= has_hour;
	return *this;
}

Duration& Duration::setHours(const std::string &h) {
	return setHours(getInt(h));
}

int Duration::getHours() {
	if (hasHours())
		return hours;
	return 0;
}

bool Duration::hasHours() {
	return ((flags & has_hour) != 0);	 
}

Duration& Duration::setMinutes(int m) {
	minutes = m;
	flags |= has_minutes;
	return *this;
}

Duration& Duration::setMinutes(const std::string &m) {
	return setMinutes(getInt(m));
	
}

int Duration::getMinutes() {
	if (hasMinutes())
		return minutes;
	return 0;
}

bool Duration::hasMinutes() {
	return ((flags & has_minutes) != 0);	 
}

Duration& Duration::setSeconds(int s) {
	seconds = s;
	flags |= has_seconds;
	return *this;
}

Duration& Duration::setSeconds(const std::string &s) {
	return setSeconds(getInt(s));
}

int Duration::getSeconds() {
	if (hasSeconds())
		return seconds;
	return 0;
}

bool Duration::hasSeconds() {
	return ((flags & has_seconds) != 0);	 
}

Duration& Duration::setFracSec(int f) {
	frac_sec = f;
	flags |= has_fracsec;
	return *this;
}

Duration& Duration::setFracSec(const std::string &f) {
	setFracSec(getInt(f));
	return *this;
}

int Duration::getFracSec() {
	if (hasFracSec())
		return frac_sec;
	return 0;
}

bool Duration::hasFracSec() {
	return ((flags & has_fracsec) != 0);	 
}

Duration& Duration::setNeg(bool n) {
	neg = n;
	return *this;
}


bool Duration::isNeg() const {
	return neg;	 
}

bool Duration::hasFullDateInfo() {
	if (hasYears() && hasMonths() && hasDays())	 
		return true;
	return false;
}

bool Duration::hasFullTimeInfo() {
	if (hasHours() && hasMinutes() && hasSeconds())	 
		return true;
	return false;
}

std::string Duration::toString() {
	std::stringstream ss;
	std::string ret("");
	
	if (isNeg())
		ss << "-";
	ss << "P";
	if (hasYears()) {
		ss << getYears(); 
		ss << "Y";
	}
	if (hasMonths()) {
		ss << getMonths(); 
		ss << "M";
	}
	if (hasDays()) {
		ss << getDays(); 
		ss << "D";
	}
	if (hasHours() || hasMinutes() || hasSeconds()) {
		ss << "T";
		if (hasHours()) {
			ss << getHours(); 
			ss << "H";
		}
		if (hasMinutes()) {
			ss << getMinutes(); 
			ss << "M";
		}
		if (hasSeconds()) {
			ss << getSeconds();
			if (hasFracSec()) {
				 ss << "."; 
				 ss << std::setw(3);
				 ss << std::setfill('0');
				 ss << getFracSec();
			}
			ss << "S";
		}
	}
	ss >> ret;
	return ret;
}

Calendar::Calendar(struct tm stm, int fs, int gh, int gm, int dst, int f) {
	cal_tm = stm;
	cal_tm.tm_isdst = dst;
	frac_sec = fs;
	gmt_off_hours = gh;
	gmt_off_minutes = gm;
	flags = f;
	fixTm();
}

Calendar &Calendar::applyDuration(Duration dur) {
	boost::posix_time::time_duration tmp;
	boost::posix_time::ptime ret;
	int year = 0, month = 0, fl = flags;

	tmp = boost::posix_time::hours(dur.getHours()) 
			+ boost::posix_time::minutes(dur.getMinutes()) 
			+ boost::posix_time::seconds(dur.getSeconds())
			+ boost::posix_time::millisec(dur.getFracSec());

	if (hasFracSec() || dur.hasFracSec()) 
		fl |= has_fracsec;

	bool sign = true;
	if (dur.isNeg())
		sign = false;
	
	int tmpy = getYear();
	int tmpm = getMonth();
	int tmpd = getDay();

	if (tmpy == 0)
		tmpy = 1978;
	if (tmpm == 0)
		tmpm = 1;
	if (tmpd == 0)
		tmpd = 1;
	
	boost::gregorian::date d(tmpy, tmpm, tmpd);
	boost::posix_time::time_duration td(getHour(), getMinutes(), getSeconds(), getFracSec());

	//XMLBEANSXX_DEBUG(Calendar_log, "addDurr: sign " + TextUtils::intToString(sign));
	//XMLBEANSXX_DEBUG(Calendar_log, "addDurr: date " + boost::gregorian::to_iso_extended_string(d));
	//XMLBEANSXX_DEBUG(Calendar_log, "addDurr: td " + boost::posix_time::to_simple_string(td));

	boost::posix_time::ptime pt(d, td); 
	
	//XMLBEANSXX_DEBUG(Calendar_log, "addDurr: pt " + boost::posix_time::to_simple_string(pt));
	//XMLBEANSXX_DEBUG(Calendar_log, "addDurr: tmp " + boost::posix_time::to_simple_string(tmp));
	
	if (sign)
		ret = pt + tmp;
	else
		ret = pt - tmp;
	
	//XMLBEANSXX_DEBUG(Calendar_log, "addDurr: ret " + boost::posix_time::to_simple_string(ret));
	
	if (dur.hasDays()) {
		//XMLBEANSXX_DEBUG(Calendar_log, "ret 10-> " + boost::posix_time::to_simple_string(ret));
		if (sign) 
			ret = ret + boost::gregorian::days(dur.getDays());
		else
			ret = ret - boost::gregorian::days(dur.getDays());
		//XMLBEANSXX_DEBUG(Calendar_log, "ret 11-> " + boost::posix_time::to_simple_string(ret));
	}

	if (dur.hasMonths() || hasYear()) {
		int pom;
		if (sign) {
			pom = (ret.date().month() + dur.getMonths());
			//XMLBEANSXX_DEBUG(Calendar_log, "month --> " + TextUtils::intToString(ret.date().month()) + " + " + TextUtils::intToString(dur.getMonths()));
		} else {
			pom = (ret.date().month() - dur.getMonths());
			//XMLBEANSXX_DEBUG(Calendar_log, "month --> " + TextUtils::intToString(ret.date().month()) + " - " + TextUtils::intToString(dur.getMonths()));
		}
	
		//TODO ??????
		if (pom < 0) {
			pom = -pom;
			month = pom % 12;
			month = 12 - month;
			year -= pom / 12 + 1; 
		} else {
			month += pom % 12;
			year += pom / 12;
			if (month == 0)
				month = 1;
			if (pom == 12 && !hasMonth()) //fixing
				year--;
		}
			//XMLBEANSXX_DEBUG(Calendar_log, "month = " + TextUtils::intToString(month));
			//XMLBEANSXX_DEBUG(Calendar_log, "pom = " + TextUtils::intToString(pom));
			//XMLBEANSXX_DEBUG(Calendar_log, "pom / 12:" + TextUtils::intToString(pom / 12));
	}

	if (dur.hasYears() || hasYear()) {
		if (sign)
			year = year + ret.date().year() + dur.getYears();
		else
			year = year + ret.date().year() - dur.getYears();
	}

	if (ret.date().day() > maximumDayInMonth(year, month)) {
			ret = ret - boost::gregorian::days(dur.getDays());
			ret = ret + boost::gregorian::days(maximumDayInMonth(year, month));
	}
		 
	//XMLBEANSXX_DEBUG(Calendar_log, "ret 1-> " + boost::posix_time::to_simple_string(ret));
	//XMLBEANSXX_DEBUG(Calendar_log, "year 2-> " + TextUtils::intToString(year));
	//XMLBEANSXX_DEBUG(Calendar_log, "month 3-> " + TextUtils::intToString(month));

	Calendar cal(ret);

	cal.setMonth(month).setYear(year); //TODO -- set DST i TZ

	//XMLBEANSXX_DEBUG(Calendar_log, "cal -> " + cal.toString());

	return *(new Calendar(cal.cal_tm, cal.frac_sec, gmt_off_hours, gmt_off_minutes, cal_tm.tm_isdst, fl));
}

std::string Calendar::toString() const {
	std::stringstream ss;

	if (hasYear()) {
		ss << "Y_";
		ss << getYear();
	};
	if (hasMonth()) {
		ss << "_M_";
		ss << getMonth();
	};
	if (hasDay()) {
		ss << "_D_";
		ss << getDay();
	};
	if (hasHour()) {
		ss << "_h_";
		ss << getHour();
	};
	if (hasMinutes()) {
		ss << "_m_";
		ss << getMinutes();
	};
	if (hasSeconds()) {
		ss << "_s_";
		ss << getSeconds();
	};
	if (hasFracSec()) {
		ss << "_f_";
		ss << getFracSec();
	};
	if (hasTimeZone()) {
		ss << "_tz_";
		ss << timeZoneToString();
	};


	std::string ret;
	ss >> ret;

	//XMLBEANSXX_DEBUG(Calendar_log, "!!!!!!!!!!!!! " + ret);
		

	return ret;
}

Calendar &Calendar::operator= (const Calendar &c) {
	return *(new Calendar(cal_tm, frac_sec, gmt_off_hours, gmt_off_minutes, cal_tm.tm_isdst, flags));
}

Calendar &Calendar::operator+ (const Duration &d) {
	Calendar ret = applyDuration(d);
	return *(new Calendar(ret.cal_tm, ret.frac_sec, ret.gmt_off_hours, ret.gmt_off_minutes, ret.cal_tm.tm_isdst, ret.flags));
}

Calendar &Calendar::operator- (const Duration &d) {
	Duration pom = d;

	if (pom.isNeg() == true) 
		 pom.setNeg(false);
	else
		pom.setNeg(true);

	Calendar ret = applyDuration(pom);
	return *(new Calendar(ret.cal_tm, ret.frac_sec, ret.gmt_off_hours, ret.gmt_off_minutes, ret.cal_tm.tm_isdst, ret.flags));
}

Duration::Duration(int y, int m, int d, int ho, int mi, int se, int fs, bool ne, int fl) {
	years = y;
	months = m;
	days = d;
	hours = ho;
	minutes = mi;
	seconds = se;
	frac_sec = fs;
	neg = ne;
	flags = fl;
}

Duration &Duration::operator= (const Duration &d) {
	return *(new Duration(d.years, d.months, d.days, d.hours, d.minutes, d.seconds, d.frac_sec, d.neg, d.flags));
}

//TODO:: FIx it
/*
Duration &Duration::operator+ (const Duration &d) {
	if (!d.isNeg() && !isNeg())
		return *(new Duration(years + d.years, months + d.months, days + d.days, 
					hours + d.hours, minutes + d.minutes, seconds + d.seconds, frac_sec + d.frac_sec, 
					d.neg, d.flags));
	else if (d.isNeg() && isNeg()) 
		return *(new Duration(years + d.years, months + d.months, days + d.days, 
					hours + d.hours, minutes + d.minutes, seconds + d.seconds, frac_sec + d.frac_sec, 
					true, d.flags));
	

}*/

