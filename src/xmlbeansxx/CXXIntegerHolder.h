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
    
#ifndef XMLBEANSXX_CXXINTEGERHOLDER_INCLUDED
#define XMLBEANSXX_CXXINTEGERHOLDER_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "Ptr.h"
#include "XmlObjectBase.h"
namespace xmlbeansxx {
class CXXIntegerHolder_I;
BEGIN_CLASS(CXXIntegerHolder, CXXIntegerHolder_I)
END_CLASS()
class CXXIntegerHolder_I: public XmlObjectBase_I {
    private:
    static log4cxx::LoggerPtr LOG;
    public:
protected:
    virtual String compute_text() {
        return get_store_text();
    }
    virtual void set_text(const String &s) {
        set_store_text(s);
    }

public:
    virtual void setStringValue(const String &v) {
        set_store_text(v); 
        setBigIntegerValue(getBigIntegerValue());
    }
};
}
#endif //XMLBEANSXX_CXXINTEGERHOLDER_INCLUDED
