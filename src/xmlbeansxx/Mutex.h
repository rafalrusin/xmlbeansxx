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

#ifndef XMLBEANSXX_MUTEX_INCLUDED
#define XMLBEANSXX_MUTEX_INCLUDED
#include "BoostAssert.h"
#include <boost/config.hpp>
#ifdef BOOST_HAS_THREADS
#include <boost/thread/detail/lock.hpp>
#include <boost/thread/recursive_mutex.hpp>
#endif

namespace xmlbeansxx {
#ifdef BOOST_HAS_THREADS
typedef boost::recursive_mutex Mutex;
typedef boost::detail::thread::scoped_lock<Mutex> Lock;
#else
class Mutex {}
;
class Lock {
    public:
    Lock(Mutex &mutex) {}
}
;
#endif
}
#endif //XMLBEANSXX_MUTEX_INCLUDED
