/*
    Copyright 2004-2008 TouK sp. z o.o. s.k.a.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */


#ifndef _XMLBEANSXX_TRACER_H_
#define _XMLBEANSXX_TRACER_H_


#include "logger.h"


#ifndef XMLBEANSXX_LOGGING
#define TRACER(log,method)

#else  // XMLBEANSXX_LOGGING

#define TRACER(log,method) Tracer<log4cxx::LoggerPtr> tracer(log,method)


#include <string>


namespace xmlbeansxx {

template <class LoggerPtr_type>
class Tracer {
public:
    Tracer(LoggerPtr_type logger, const std::string &text) : logger(logger), text(text) {
        XMLBEANSXX_DEBUG(logger,std::string("entering ") + text);
    };

    ~Tracer() {
	XMLBEANSXX_DEBUG(logger,std::string("leaving ") + text);
    };

protected:
    LoggerPtr_type logger;
    std::string text;
};

}

#endif // XMLBEANSXX_LOGGING

#endif//_XMLBEANSXX_TRACER_H_

