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
    
#ifndef XMLBEANSXX_STACK_INCLUDED
#define XMLBEANSXX_STACK_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include <stack>
namespace xmlbeansxx {
template <typename T>
class Stack {
    public:
    typedef std::stack<T> type;
};
}
#endif //XMLBEANSXX_STACK_INCLUDED
