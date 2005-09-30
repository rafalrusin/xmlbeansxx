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
    
#ifndef XMLBEANSXX_NULL_INCLUDED
#define XMLBEANSXX_NULL_INCLUDED
#include "BoostAssert.h"
#include "cast.h"
namespace xmlbeansxx {
    /*
template <typename T, typename Enable = void>
class NullTraits {
    public:
    T operator()() const {
        return T();
    }
};*/

/**
 * Class with behaviour of NULL object. Converts to any object by invoking it's no parameter constructor. 
 * It uses NullTraits to get a specific Null conversion. NullTraits can be specialized.
 */
class Null {
    public:
    template <typename T>
    operator T() const {
        return T();
    }
};

template<typename TO>
class cast_traits<TO, Null, void> {
    public:
    TO operator ()(Null from) const {
        return TO();
    }
};

template<typename X>
bool operator == (const X& x, const Null &y) {
    return x == (X)y;
}

template<typename X>
bool operator != (const X& x, const Null &y) {
    return x != (X)y;
}


}
#endif //XMLBEANSXX_NULL_INCLUDED

