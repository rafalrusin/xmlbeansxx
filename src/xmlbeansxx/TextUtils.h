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

#include <string>
#include <log4cxx/logger.h>
#include <xmlbeansxx/shared_array.h>


namespace xmlbeansxx {
/**
 * Contains some utility functions for strings.
 */
class TextUtils {
private:
    static log4cxx::LoggerPtr log;
    TextUtils();

public:
    static std::string intToString(int i);
    static std::string floatToString(float f);
    static std::string doubleToString(double d);
    static std::string ptrToString(const void *ptr);
    static std::string boolToString(bool b);
    /** Checks whether given character is a whitespace ('\t', ' ' or '\n').  */
    static bool isWhite(char ch);
    /** Returns whitespace collapsed string (as does xml schema "whiteSpace" rule "collapse") */
    static std::string collapse(const std::string &str);
    /** Exchanges a string to xml representation. Eg. exchanges '<' to '&lt;'. */
    static std::string exchangeEntities(const std::string& str);
    /** Exchanges a string to xml CDATA representation. */
    static std::string exchangeEntitiesWithCDATA(const std::string& str);

    static bool isDigit(char c);
    static bool areDigits(const std::string &d);

    /** Returns true if given string is a valid xml schema integer (xs:integer) */
    static bool checkInteger(const std::string &num);
    /** Returns true if given string is a valid xml schema decimal (xs:decimal) */
    static bool checkDecimal(const std::string &num);
    /** Returns true if given string is a valid xml schema date (xs:date) */
    static bool checkDate(const std::string &date);

    /** Decodes given base64 string to array of characters. */
    static xmlbeansxx::shared_array<unsigned char> base64Decode(const std::string &what);
    /** Encodes given array of characters to base64 string. */
    static std::string base64Encode(xmlbeansxx::shared_array<unsigned char> what);
};

}

#endif//_XMLBEANSXX_TEXTUTILS_H_
