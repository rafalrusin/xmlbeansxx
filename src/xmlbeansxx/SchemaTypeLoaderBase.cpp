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
    
#include "SchemaTypeLoaderBase.h"
#include "XmlTypesGen.h"
#include <log4cxx/logger.h>
#include "XmlObjectBase.h"
#include "TypeStoreImpl.h"
namespace xmlbeansxx {
log4cxx::LoggerPtr SchemaTypeLoaderBase_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.SchemaTypeLoaderBase");

SchemaType SchemaTypeLoaderBase_I::findType(const QName &name) {
    return typeCreators[name];
}
XmlObject SchemaTypeLoaderBase_I::newInstance(const SchemaType &type, const XmlOptions &options) {
    BOOST_ASSERT(cast<SchemaTypeImpl>(type)->createRawXmlObject != NULL);
    XmlObject n = cast<SchemaTypeImpl>(type)->createRawXmlObject();
    XmlObjectBase b = cast<XmlObjectBase>(n);
    b->attach_store(TypeStoreImpl::create(type));
    return n;
}
void SchemaTypeLoaderBase_I::addType(const SchemaType &st) {
    typeCreators[st->getName()] = cast<SchemaTypeImpl>(st);
}
void SchemaTypeLoaderBase_I::removeType(const SchemaType &st) {
    typeCreators.erase(st->getName());
}

}
