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
#ifndef XMLBEANSXX_MAP_INCLUDED
#define XMLBEANSXX_MAP_INCLUDED
#include "BoostAssert.h"
#include <ext/hash_map>
namespace xmlbeansxx {
/*
template<typename U, typename V>
class Map {
public:
    typedef __gnu_cxx::hash_map<U, V> type;
};*/
template<typename KEY, typename VALUE, typename HASHFN = __gnu_cxx::hash<KEY>, typename EQFN = std::equal_to<KEY> >
class Map {
public:
    typedef __gnu_cxx::hash_map<KEY, VALUE, HASHFN, EQFN> type;
};
}
#endif //XMLBEANSXX_MAP_INCLUDED
