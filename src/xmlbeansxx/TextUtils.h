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


#ifndef _XMLBEANSXX_TEXTUTILS_H_
#define _XMLBEANSXX_TEXTUTILS_H_

#include "BoostAssert.h"
#include <string>
#include <log4cxx/logger.h>
#include <xmlbeansxx/Array.h>
#include <xercesc/framework/XMLFormatter.hpp>
#include "String.h"


namespace xmlbeansxx {
/**
* Contains some utility functions on strings.
*/
class TextUtils {
private:
    static log4cxx::LoggerPtr log;
    TextUtils();

public:
    static String intToString(int i);
    static String floatToString(float f);
    static String doubleToString(double d);
    static String ptrToString(const void *ptr);
    static String boolToString(bool b);
    /** Checks whether given character is a whitespace ('t', ' ' or 'n').  */
    static bool isWhite(char ch);
    /** Returns whitespace collapsed string (as does xml schema "whiteSpace" rule "collapse") */
    static String collapse(const String &str);
    /** Exchanges a string to xml representation. Eg. exchanges '<' to '&lt;'. */
    static String exchangeEntities(const String& str, XERCES_CPP_NAMESPACE::XMLFormatter::EscapeFlags escapeFlag  = XERCES_CPP_NAMESPACE::XMLFormatter::StdEscapes);
    /** Exchanges a string to xml CDATA representation. */
    static String exchangeEntitiesWithCDATA(const String& str);

    static bool isDigit(char c);
    static bool areDigits(const String &d);

    /** Returns true if given string is a valid xml schema integer (xs:integer) */
    static bool checkInteger(const String &num);
    /** Returns true if given string is a valid xml schema decimal (xs:decimal) */
    static bool checkDecimal(const String &num);
    /** Returns true if given string is a valid xml schema date (xs:date) */
    static bool checkDate(const String &date);

    /** Decodes given base64 string to array of characters. */
    static xmlbeansxx::Array<unsigned char> base64Decode(const String &what);
    /** Encodes given array of characters to base64 string. */
    static String base64Encode(xmlbeansxx::Array<unsigned char> what);

    /** Reads whole istream into string. */
    static String istreamToString(std::istream &in);
};

}

#endif//_XMLBEANSXX_TEXTUTILS_H_
