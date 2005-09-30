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
    
#ifndef XMLBEANSXX_SCHEMATYPELOADERBASE_INCLUDED
#define XMLBEANSXX_SCHEMATYPELOADERBASE_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "Map.h"
#include "QName.h"
#include "SchemaType.h"
#include "SchemaTypeImpl.h"
#include "SchemaTypeLoader.h"
namespace xmlbeansxx {
class SchemaTypeLoaderBase_I;
typedef boost::shared_ptr<SchemaTypeLoaderBase_I> SchemaTypeLoaderBase;
class SchemaTypeLoaderBase_I: public SchemaTypeLoader_I {
    private:
    static log4cxx::LoggerPtr LOG;

    Map<QName, SchemaTypeImpl, QName::Hash>::type typeCreators;

    public:
    virtual SchemaType findType(const QName &name);
    virtual XmlObject newInstance(const SchemaType &type, const XmlOptions &options);
    /** Registers given type in TypeSystem */
    virtual void addType(const SchemaType &st);
    /** Removes given type in TypeSystem */
    virtual void removeType(const SchemaType &st);
};
}
#endif //XMLBEANSXX_SCHEMATYPELOADERBASE_INCLUDED
