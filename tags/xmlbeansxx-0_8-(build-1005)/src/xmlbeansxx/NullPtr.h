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


#ifndef _XMLBEANSXX_NULLPTR_H_
#define _XMLBEANSXX_NULLPTR_H_

#include "BoostAssert.h"
#include <boost/shared_ptr.hpp>



namespace xmlbeansxx {

class NullPtr: public boost::shared_ptr<void> {
public:
    template <class T>
    operator boost::shared_ptr<T>() const {
        return boost::shared_ptr<T>();
    }
};

}

#endif//_XMLBEANSXX_NULLPTR_H_

