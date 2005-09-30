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
#ifndef XMLBEANSXX_CXXBOOLEANHOLDER_INCLUDED
#define XMLBEANSXX_CXXBOOLEANHOLDER_INCLUDED
#include <xmlbeansxx/BoostAssert.h>
#include <log4cxx/logger.h>
#include <xmlbeansxx/String.h>
#include <xmlbeansxx/XmlObjectBase.h>
namespace xmlbeansxx {
class CXXBooleanHolder_I: public XmlObjectBase_I {
private:
    static log4cxx::LoggerPtr LOG;

    //bool _value;
protected:
    virtual String compute_text();
    virtual void set_text(const String &s);

public:
    static bool validateLexical(const String &v, const ValidationContext &context);

    virtual bool getBooleanValue();
    virtual void setBooleanValue(const bool &v);
    
    virtual void setStringValue(const String &v) {
        set_store_text(v); 
        setBooleanValue(getBooleanValue());
    }
};
}
#endif //XMLBEANSXX_CXXBOOLEANHOLDER_INCLUDED
