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
#include "CXXBooleanHolder.h"
#include <log4cxx/logger.h>
#include <xmlbeansxx/XmlErrorCodes.h>
namespace xmlbeansxx {
log4cxx::LoggerPtr CXXBooleanHolder_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.CXXBooleanHolder");
/*
String CXXBooleanHolder_I::compute_text() {
    if (_value) return "true"; else return "false";
}


void CXXBooleanHolder_I::set_text(const String &s) {
    _value = validateLexical(s, _voorVc);
}

bool CXXBooleanHolder_I::validateLexical(const String &v, const ValidationContext &context) {
    if (v == "0" || v == "false")
        return false;
    if (v == "1" || v == "true")
        return true;
    context->invalid(XmlErrorCodes::BOOLEAN, v);
    return false;
}


bool CXXBooleanHolder_I::getBooleanValue() {
    return _value;
}

void CXXBooleanHolder_I::setBooleanValue(const bool &v) {
    _value = v;
}*/

String CXXBooleanHolder_I::compute_text() {
    return get_store_text();
}


void CXXBooleanHolder_I::set_text(const String &s) {
    set_store_text(s);
}

bool CXXBooleanHolder_I::validateLexical(const String &v, const ValidationContext &context) {
    if (v == "0" || v == "false")
        return false;
    if (v == "1" || v == "true")
        return true;
    context->invalid(XmlErrorCodes::BOOLEAN, v);
    return false;
}


bool CXXBooleanHolder_I::getBooleanValue() {
    return validateLexical(get_store_text(), _voorVc);
}

void CXXBooleanHolder_I::setBooleanValue(const bool &value) {
    if (value) set_store_text("true"); else set_store_text("false");
}

}
