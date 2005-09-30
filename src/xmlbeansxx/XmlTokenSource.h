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

#ifndef XMLBEANSXX_XMLTOKENSOURCE_INCLUDED
#define XMLBEANSXX_XMLTOKENSOURCE_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "Mutex.h"
#include "String.h"
#include "XmlOptions.h"
namespace xmlbeansxx {
class XmlCursor_I;
class XmlCursor;
class XmlTokenSource_I;
BEGIN_CLASS(XmlTokenSource, XmlTokenSource_I)
END_CLASS()
class XmlTokenSource_I {
private:
    static log4cxx::LoggerPtr LOG;
public:
    virtual Mutex &mutex() = 0;
    virtual String xmlText(const XmlOptions &options) = 0;
    virtual XmlCursor newCursor() = 0;
    virtual ~XmlTokenSource_I() {}
};
}
#endif //XMLBEANSXX_XMLTOKENSOURCE_INCLUDED
