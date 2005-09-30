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
#ifndef XMLBEANSXX_XMLCOMPLEXCONTENTIMPL_INCLUDED
#define XMLBEANSXX_XMLCOMPLEXCONTENTIMPL_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "XmlObjectBase.h"
#include "TypeStoreUser.h"
namespace xmlbeansxx {
class XmlComplexContentImpl_I: public XmlObjectBase_I {
private:
    static log4cxx::LoggerPtr LOG;
public:
    XmlComplexContentImpl_I();

    TypeStoreUser cget_helper(const QName &name, int index, const SchemaType &type);
    void arraySetterHelper(const QName &name, const Array<XmlObject> &elements);

    virtual void set_text(const String &text) {}
    virtual String compute_text() { return String(); }
};
}
#endif //XMLBEANSXX_XMLCOMPLEXCONTENTIMPL_INCLUDED
