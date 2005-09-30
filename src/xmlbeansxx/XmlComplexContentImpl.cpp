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
#include "XmlComplexContentImpl.h"
#include <log4cxx/logger.h>
#include "TypeStoreImpl.h"
#include "Null.h"
#include "XmlBeans.h"
#include "defs.h"
namespace xmlbeansxx {
log4cxx::LoggerPtr XmlComplexContentImpl_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.XmlComplexContentImpl");
XmlComplexContentImpl_I::XmlComplexContentImpl_I() {
}


TypeStoreUser XmlComplexContentImpl_I::cget_helper(const QName &name, int index, const SchemaType &type) {
    TypeStore store = get_store();
    TypeStoreUser user = store->find_element(name, index);
    if (user == Null()) {
        user = cast<TypeStoreUser>(XmlBeans::getContextTypeLoader()->newInstance(type, XmlOptions::New()));
        store->set_element(name, index, user);
    }
    return user;
}
    
void XmlComplexContentImpl_I::arraySetterHelper(const QName &name, const Array<XmlObject> &elements) {
    TypeStore store = get_store();
    store->remove_all_elements(name);
    FOREACH(it, elements) {
        store->add_element(name, cast<TypeStoreUser>(*it));
    }
}

}
