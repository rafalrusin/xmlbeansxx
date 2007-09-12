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
#include <xmlbeansxx/shared_array.h>
#include "logger.h"


namespace xmlbeansxx {
class TextUtils {
private:
    STATIC_LOGGER_PTR(log);

    TextUtils();

public:

    enum EscapeFlags  {StdEscapes,AttrEscapes};

    static std::string intToString(int i);
    static std::string floatToString(float f);
    static std::string doubleToString(double d);
    static std::string ptrToString(const void *ptr);
    static std::string boolToString(bool b);
    static bool isWhite(char ch);
    static std::string collapse(const std::string &str);
    static std::string exchangeEntities(const std::string& str, TextUtils::EscapeFlags escapeFlag  = StdEscapes);
    static std::string exchangeEntitiesWithCDATA(const std::string& str);

    static bool isDigit(char c);
    static bool areDigits(const std::string &d);

    static bool checkInteger(const std::string &num);
    static bool checkDecimal(const std::string &num);
    static bool checkDate(const std::string &date);

    static xmlbeansxx::shared_array<unsigned char> base64Decode(const std::string &what);
    static std::string base64Encode(xmlbeansxx::shared_array<unsigned char> what);
};

}

#endif//_XMLBEANSXX_TEXTUTILS_H_
