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


#ifndef _XMLBEANSXX_GEUM_TYPES_H_
#define _XMLBEANSXX_GEUM_TYPES_H_

#include "XmlException.h"



namespace xmlbeansxx {

/**
 * std::string  comparision struct used by XpathsMap
 */
typedef struct ltstr {
    bool operator()(const std::string s1, const std::string s2) const {
        return s1.compare(s2) < 0;
    }
};

/**
 * XpathsMap struct definition. 
 * Contains xpaths and their values of current xml message.
 */
typedef std::map<std::string, std::string, ltstr> XpathsMap;

/**
 * XpathsCounterMap struct definition. Holds counters for every tag xpath.
 */
typedef std::map<std::string, int, ltstr> XpathsCounterMap;

/**
 * Encapsulate an XML parse error or warning
 */
class XpathException: public XmlException {
public:

    static const int INVALID_XPATH_FORMAT = 1;
    static const int XPATH_NOT_FOUND = 2;
    static const int INVALID_XPATH_ROOT_ELEMENT = 3;

private:
    int code;

public:
    /**
     * Constructs new XpathException instance with message
     * @param msg exception message
     */
    XpathException(std::string msg, int code = 0)
            : XmlException(msg),code(code) {}

    /**
     * Constructs new XpathException instance with message
     * @param msg exception message
     */
    XpathException(int code = 0)
            : XmlException("Xpath exception"), code(code) {}

    int getCode() const {
        return code;
    }
};

}

#endif
