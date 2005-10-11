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
#ifndef XMLBEANSXX_TYPESTOREVISITOR_INCLUDED
#define XMLBEANSXX_TYPESTOREVISITOR_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "TypeStoreUser.h"
#include "QName.h"
namespace xmlbeansxx {
/**
* This interface is used by the TypeStore to visit every element in order
* to compute nillable flags and default values.
*/
class TypeStoreVisitor_I {
public:
    virtual ~TypeStoreVisitor_I();

    virtual bool visit(const QName &name) = 0;
};
BEGIN_CLASS(TypeStoreVisitor, TypeStoreVisitor_I)
END_CLASS()
}
#endif //XMLBEANSXX_TYPESTOREVISITOR_INCLUDED
