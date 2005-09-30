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

#ifndef XMLBEANSXX_XMLCURSOR_INCLUDED
#define XMLBEANSXX_XMLCURSOR_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "XmlTokenSource.h"
#include "QName.h"
namespace xmlbeansxx {
class XmlObject;
class XmlCursor_I;
BEGIN_CLASS(XmlCursor, XmlCursor_I)
END_CLASS()
class XmlCursor_I: public virtual XmlTokenSource_I {
private:
    static log4cxx::LoggerPtr LOG;
public:
    virtual String xmlText(const XmlOptions &options) = 0;
    virtual void save(std::ostream &output, const XmlOptions &options) = 0;
    virtual bool isAttr() = 0;

    virtual XmlObject getObject() = 0;

    virtual void dispose() = 0;
    virtual bool toNextSibling() = 0;

    virtual void insertChars(const String &text) = 0;
    virtual void beginElement(const QName &name) = 0;
    virtual void insertAttributeWithValue(const QName &name, const String &value) = 0;
    virtual void insertAttributeWithValue(const QName &name, const QName &value) = 0;
    
    virtual void push() = 0;
    virtual bool pop() = 0;
    virtual ~XmlCursor_I() {}
};
}
#endif //XMLBEANSXX_XMLCURSOR_INCLUDED
