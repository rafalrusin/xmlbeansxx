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

#ifndef XMLBEANSXX_EXISTENCE_INCLUDED
#define XMLBEANSXX_EXISTENCE_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include <boost/shared_ptr.hpp>
namespace xmlbeansxx {
class Existence_I;
typedef boost::shared_ptr<Existence_I> Existence;
/** A base class for every static initializers/destructors */
class Existence_I {
public:
    virtual ~Existence_I() {}
};

}
#endif //XMLBEANSXX_EXISTENCE_INCLUDED
