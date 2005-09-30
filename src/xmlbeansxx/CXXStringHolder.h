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
#ifndef XMLBEANSXX_CXXSTRINGHOLDER_INCLUDED
#define XMLBEANSXX_CXXSTRINGHOLDER_INCLUDED
#include <xmlbeansxx/BoostAssert.h>
#include <log4cxx/logger.h>
#include <xmlbeansxx/XmlObjectBase.h>
namespace xmlbeansxx {
class CXXStringHolder_I: public XmlObjectBase_I {
private:
    static log4cxx::LoggerPtr LOG;

    //String _value;
protected:
    virtual String compute_text() {
        return get_store_text();
        //return _value;
    }
    virtual void set_text(const String &s) {
        set_store_text(s);
        //_value = s;
    }
};
}
#endif //XMLBEANSXX_CXXSTRINGHOLDER_INCLUDED
