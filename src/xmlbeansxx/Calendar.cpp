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

extern XMLBEANSXX_LOGGER_PTR(Calendar_log);

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



int getInt(std::string sint) {
	std::string tmp = TextUtils::collapse(sint);
	int len = tmp.length();

	if (!TextUtils::areDigits(tmp))
		throw BeansException("Not all requested signs are digits!");
	
	std::stringstream ss;
	ss << tmp;
	ss >> len;

	return len;
}

///////////////////////////////////////////////////

Calendar::Calendar() {
	flags = 0;
}

Calendar::Calendar(boost::gregorian::date date) {
	flags = 0;
	if (!date.is_special()) {
		cal_tm = to_tm(date);
		flags |= has_year;
		flags |= has_month;
		flags |= has_day;
	}
}

Calendar::Calendar(boost::posix_time::ptime ptime) {
	flags = 0;
	if (!ptime.is_special()) {
		cal_tm = to_tm(ptime);
		flags |= has_year;
		flags |= has_month;
		flags |= has_day;
		flags |= has_hour;
		flags |= has_minutes;
		flags |= has_seconds;
	}
}

Calendar::Calendar(std::string str) { 
	//str = CCYY-MM-DDThh:mm:ss(.sss)(Z|(+|-)hh:mm)
	flags = 0;
	char * sign = "?";
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
			throw BeansException("Bad FracSec value!");
	}

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); //- 
			gmt_off_minutes = -getInt(tz.substr(4,5)); //-
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	
}

Calendar& Calendar::setgDate(std::string str) {
	//str = CCYY-MM-DD(Z|(+|-)hh:mm)
	char * sign = "?";
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
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}


Calendar& Calendar::setgTime(std::string str) { 
	//str = hh-mm-ss(.sss)(Z|(+|-)hh:mm)
	char * sign = "?";
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

	if (!frac.empty()) {
		int fsec = getInt(frac);
		if (fsec >= 0 && fsec < 1000) {
			frac_sec = fsec;	 
			flags |= has_fracsec;
		} else
			throw BeansException("Bad FracSec value!");
	}

	if (sign != "?") {
		if (sign == "Z") {
			gmt_off_hours = 0; 
			gmt_off_minutes = 0; 
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}

	return *this;
}

Calendar& Calendar::setgYearMonth(std::string str) {
	//str = CCYY-MM(Z|(+|-)hh:mm)
	char * sign = "?";
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
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgYear(std::string str) {
	//str = CCYY(Z|(+|-)hh:mm)
	char * sign = "?";
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
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgMonthDay(std::string str) {
	//str = --MM-DD(Z|(+|-)hh:mm)
	char * sign = "?";
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
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgMonth(std::string str) {
	//str = --MM(Z|(+|-)hh:mm)
	char * sign = "?";
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
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}

Calendar& Calendar::setgDay(std::string str) {
	//str = ---DD(Z|(+|-)hh:mm)
	char * sign = "?";
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
		} else if (sign == "+") {
			gmt_off_hours = getInt(tz.substr(1,2)); 
			gmt_off_minutes = getInt(tz.substr(4,5)); 
		} else {
			gmt_off_hours = -getInt(tz.substr(1,2)); 
			gmt_off_minutes = -getInt(tz.substr(4,5)); 
		}
		cal_tm.tm_isdst = 0; 
		flags |= has_timezone;
	}
	return *this;
}
///////////////////////////////////////////////////

// Year
Calendar& Calendar::setYear(int year) {
	cal_tm.tm_year = year - 1900;
	flags |= has_year;
	fixTm();
	return *this;
}

Calendar& Calendar::setYear(std::string year) {
	setYear(getInt(year));
}

int Calendar::getYear() {
	if (hasYear())
		return 1900 + cal_tm.tm_year;
	return 0;
}

bool Calendar::hasYear() {
	return ((flags & has_year) != 0); 
}

//Month
Calendar& Calendar::setMonth(int month) {
	if (month > 0 && month < 13) {
		cal_tm.tm_mon = month - 1;
		flags |= has_month;
		fixTm();
	} else
		throw BeansException("Bad month value!");
	return *this;
}

Calendar& Calendar::setMonth(std::string month) {
	setMonth(getInt(month));
}

int Calendar::getMonth() {
	if (hasMonth())	
		return cal_tm.tm_mon + 1;
	return 0;
}

bool Calendar::hasMonth() {
	 return ((flags & has_month) != 0);
}

//Day
Calendar& Calendar::setDay(int day) {
	if (day > 0 && day < 32) {
		cal_tm.tm_mday = day;	 
		flags |= has_day;
		fixTm();
	} else
		throw BeansException("Bad day value!");
	return *this;
}

Calendar& Calendar::setDay(std::string day) {
	setDay(getInt(day));
}

int Calendar::getDay() {
	if (hasDay()) 
		return cal_tm.tm_mday;
	return 0;
}

bool Calendar::hasDay() {
	return ((flags & has_day) != 0);	
}

//Hour
Calendar& Calendar::setHour(int hour) {
	if (hour >= 0 && hour < 25) {
		cal_tm.tm_hour = hour;	 
		flags |= has_hour;
	} else
		throw BeansException("Bad huor value!");
	return *this;
}

Calendar& Calendar::setHour(std::string hour) {
	setHour(getInt(hour));
}

int Calendar::getHour() {
	if (hasHour()) 
		return cal_tm.tm_hour;
	return 0;
}

bool Calendar::hasHour() {
	return ((flags & has_hour) != 0);	
}

//Minutes
Calendar& Calendar::setMinutes(int min) {
	if (min >= 0 && min < 61) {
		cal_tm.tm_min = min;	 
		flags |= has_minutes;
	} else
		throw BeansException("Bad minutes value!");
	return *this;
}

Calendar& Calendar::setMinutes(std::string min) {
	setMinutes(getInt(min));
}

int Calendar::getMinutes() {
	if (hasMinutes()) 
		return cal_tm.tm_min;
	return 0;
}

bool Calendar::hasMinutes() {
	return ((flags & has_minutes) != 0);	
}

//Seconds
Calendar& Calendar::setSeconds(int sec) {
	if (sec >= 0 && sec < 61) {
		cal_tm.tm_sec = sec;	 
		flags |= has_seconds;
	} else
		throw BeansException("Bad seconds value!");
	return *this;
}

Calendar& Calendar::setSeconds(std::string sec) {
	setSeconds(getInt(sec));
}

int Calendar::getSeconds() {
	if (hasSeconds()) 
		return cal_tm.tm_sec;
	return 0;
}

bool Calendar::hasSeconds() {
	return ((flags & has_seconds) != 0);	
}

//FracSeconds
Calendar& Calendar::setFracSec(int fsec) {
	if (fsec >= 0 && fsec < 1000) {
		frac_sec = fsec;	 
		flags |= has_fracsec;
	} else
		throw BeansException("Bad fracsec value!");
	return *this;
}

Calendar& Calendar::setFracSec(std::string fsec) {
	setFracSec(getInt(fsec));
}

int Calendar::getFracSec() {
	if (hasFracSec()) 
		return frac_sec;
	return 0;
}

bool Calendar::hasFracSec() {
	return ((flags & has_fracsec) != 0);	
}

////////////////////////////

bool Calendar::hasFullDateInfo() {
	return hasYear() && hasMonth() && hasDay();	 
}

bool Calendar::hasFullTimeInfo() {
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

boost::gregorian::date Calendar::getBDate() {
	boost::gregorian::date ret = boost::gregorian::date(boost::gregorian::not_a_date_time);
	if (hasFullDateInfo()) 
		ret = boost::gregorian::date(getYear(), getMonth(), getDay());
	return ret;
}

std::string Calendar::bDateToString() {
	return boost::gregorian::to_iso_extended_string(getBDate()); 
}

boost::posix_time::ptime Calendar::getBPtime() {
	if (hasFullDateInfo() && hasFullTimeInfo()) 
		if (hasFracSec())
			return boost::posix_time::ptime(
					getBDate(),
					boost::posix_time::hours(getHour()) +
					boost::posix_time::minutes(getMinutes()) +
					boost::posix_time::seconds(getSeconds()) +
					boost::posix_time::millisec(getFracSec())
					);
		else
			return boost::posix_time::ptime(
					getBDate(),
					boost::posix_time::hours(getHour()) +
					boost::posix_time::minutes(getMinutes()) +
					boost::posix_time::seconds(getSeconds())
					);
	else
		return boost::posix_time::ptime(boost::gregorian::not_a_date_time);
}

std::string Calendar::bPtimeToString() {
	return boost::posix_time::to_iso_extended_string(getBPtime());	 
}

//DST
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

int Calendar::getDst() {
	return cal_tm.tm_isdst;
}

bool Calendar::isDstOn() { 
	if (getDst() == 1) 
		return true;
	return false;
}

bool Calendar::isSetDst() {
	if ((getDst() == 1) || (getDst() == 0))
		return true;
	return false;
}

//TimeZone
Calendar& Calendar::setGmtOff(int hour, int minutes) { 
	// setGmtOff(1,	1) 		-> +01:01 
	// setGmtOff(-2, -30) 	-> -2:30
	// setGmtOff(0, 0) 		-> Z
	if (hour * minutes < 0) 
		throw BeansException("Bad GMT offset!");
	gmt_off_hours = hour; //[-14:14] ???
	gmt_off_minutes = minutes; // {0,30} ???
	flags |= has_timezone;
	return *this;
}

std::string Calendar::timeZoneToString() { 
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

bool Calendar::hasTimeZone() {
	return ((flags & has_timezone) != 0) && isSetDst();
}

boost::posix_time::ptime Calendar::getUTCPtime() {
	boost::posix_time::ptime tmp = boost::posix_time::ptime(boost::gregorian::not_a_date_time);

	if (hasFullDateInfo() && hasFullTimeInfo())
		if (hasTimeZone()) {
			tmp = getBPtime() - boost::posix_time::hours(gmt_off_hours) - boost::posix_time::minutes(gmt_off_minutes);
			if (isDstOn()) 
				tmp = tmp - boost::posix_time::hours(1);
		}  

	return tmp;
}

std::string Calendar::utcPtimeToString() {
	return boost::posix_time::to_iso_extended_string(getUTCPtime());
}

boost::posix_time::ptime Calendar::getLocalPtime() {
	boost::posix_time::ptime tmp = boost::posix_time::ptime(boost::gregorian::not_a_date_time);

	if (hasFullDateInfo() && hasFullTimeInfo())
		if (hasTimeZone()) 
			tmp = getBPtime();

	return tmp;
}

std::string Calendar::localPtimeToString() {
	return boost::posix_time::to_iso_extended_string(getLocalPtime());
}

std::string Calendar::toXsdDateTime() {
	boost::posix_time::ptime tmp = getLocalPtime();
	std::string ret = boost::posix_time::to_iso_extended_string(tmp);		 
	
	if (!tmp.is_not_a_date_time()) {
		if (hasFracSec())
			ret = ret.substr(0, ret.find('.') + 4);
		ret += timeZoneToString();
	}

	return ret;
}

std::string Calendar::toXsdDate() { 
	std::string ret("not-a-date-time");
	if (hasFullDateInfo()) {
		boost::gregorian::date tmp(getYear(), getMonth(), getDay());
		ret = boost::gregorian::to_iso_extended_string(tmp);		 
		if (hasTimeZone())
			ret += timeZoneToString();
	}		 
	return ret;
}

std::string Calendar::toXsdTime() { 
	std::stringstream ss;	
	std::string ret("not-a-date-time");
	if (hasFullTimeInfo()) {
		ss << std::setw(2) << std::setfill('0') << getHour();
		ss << "-";
		ss << std::setw(2) << std::setfill('0') << getMinutes();
		ss << "-";
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

std::string Calendar::toXsdYearMonth() {
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

std::string Calendar::toXsdYear() {
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

std::string Calendar::toXsdDay() {
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

std::string Calendar::toXsdMonth() {
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

std::string Calendar::toXsdMonthDay() {
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


////////////////////////////////////////////////
// DURATION
////////////////////////////////////////////////
/*
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
*/


Duration::Duration() {
	flags = 0;	 
	neg = false;
}
		

Duration::Duration(std::string dur) {
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
		XMLBEANSXX_DEBUG(Calendar_log, "Bad, bad boy >" + tmp + "<");

	if (tmp.find('Y') != std::string::npos) {
		setYears(tmp.substr(0, tmp.find('Y')));
		tmp = tmp.substr(tmp.find('Y') + 1, tmp.length());
	}
	
	XMLBEANSXX_DEBUG(Calendar_log, "tmp >" + tmp + "<");
	if (tmp.find('M') != std::string::npos) {
		setMonths(tmp.substr(0, tmp.find('M')));
		tmp = tmp.substr(tmp.find('M') + 1, tmp.length());
	}
	XMLBEANSXX_DEBUG(Calendar_log, "tmp >" + tmp + "<");
	
	if (tmp.find('D') != std::string::npos) {
		setDays(tmp.substr(0, tmp.find('D')));
		tmp = tmp.substr(tmp.find('D') + 1, tmp.length());
	}

	XMLBEANSXX_DEBUG(Calendar_log, "tmp >" + tmp + "<");

	if (!tmp.empty()) {
		if (tmp.find('T') == 0) {
			tmp = tmp.substr(1, tmp.length());
			if (tmp.empty())
				XMLBEANSXX_DEBUG(Calendar_log, "Bad, bad boy >" + tmp + "<");
		}
		else
			XMLBEANSXX_DEBUG(Calendar_log, "Bad, bad boy >" + tmp + "<");

		if (tmp.find('H') != std::string::npos) {
			setHours(tmp.substr(0, tmp.find('H')));
			tmp = tmp.substr(tmp.find('H') + 1, tmp.length());
		}

		if (tmp.find('M') != std::string::npos) {
			setMinutes(tmp.substr(0, tmp.find('M')));
			tmp = tmp.substr(tmp.find('M') + 1, tmp.length());
		}

		if (tmp.find('S') != std::string::npos) {
			XMLBEANSXX_DEBUG(Calendar_log, "1Bad, bad boy >" + tmp + "<");
			if (tmp.find('.') != std::string::npos) {
			XMLBEANSXX_DEBUG(Calendar_log, "2Bad, bad boy >" + TextUtils::intToString(tmp.find('.')) + "<>" + TextUtils::intToString(tmp.find('S')) + "<");
			XMLBEANSXX_DEBUG(Calendar_log, "2Bad, bad boy >" + tmp.substr(tmp.find('.') + 1, tmp.find('S') - tmp.find('.') - 1) + "<");
				setFracSec(tmp.substr(tmp.find('.') + 1, tmp.find('S') - tmp.find('.') - 1));
				tmp = tmp.substr(0, tmp.find('.'));
			XMLBEANSXX_DEBUG(Calendar_log, "3Bad, bad boy >" + tmp + "<");
			}
			XMLBEANSXX_DEBUG(Calendar_log, "3Bad, bad boy >" + tmp.substr(0, tmp.find('S')) + "<");
			setSeconds(tmp.substr(0, tmp.find('S')));
		}
	}

}

Duration& Duration::setYears(int y) {
	years = y;
	flags |= has_year;
	return *this;
}

Duration& Duration::setYears(std::string y) {
	setYears(getInt(y));
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

Duration& Duration::setMonths(std::string m) {
	setMonths(getInt(m));
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

Duration& Duration::setDays(std::string d) {
	setDays(getInt(d));
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

Duration& Duration::setHours(std::string h) {
	setHours(getInt(h));
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

Duration& Duration::setMinutes(std::string m) {
	setMinutes(getInt(m));
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

Duration& Duration::setSeconds(std::string s) {
	setSeconds(getInt(s));
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

Duration& Duration::setFracSec(std::string f) {
	setFracSec(getInt(f));
}

int Duration::getFracSec() {
	if (hasFracSec())
		return frac_sec;
	return 0;
}

bool Duration::hasFracSec() {
	return ((flags & has_fracsec) != 0);	 
}
	
bool Duration::isNeg() {
	return neg;	 
}

std::string Duration::toString() {
	std::stringstream ss;
	std::string ret;
	
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

