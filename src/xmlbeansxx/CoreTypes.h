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


#ifndef _XMLBEANSXX_CORE_TYPES_H_
#define _XMLBEANSXX_CORE_TYPES_H_

#include "BoostAssert.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <gmp.h>
#include <gmpxx.h>
#include "String.h"
#include "StoreString.h"
#include "QName.h"
#include <boost/shared_ptr.hpp>

namespace xmlbeansxx {
typedef mpf_class BigDecimal;
typedef mpz_class BigInteger;
typedef unsigned char byte;
typedef BigInteger Long;
typedef bool boolean;

class Calendar {};
class Date {};
class GDate {};
class GDuration {};
class List {};
class Object {};
class StringEnumAbstractBase {};
}

#endif //_XMLBEANSXX_CORE_TYPES_H_


