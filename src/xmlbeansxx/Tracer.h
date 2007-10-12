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


#ifndef _XMLBEANSXX_TRACER_H_
#define _XMLBEANSXX_TRACER_H_


#include "logger.h"


//#ifdef HAVE_LIBLOG4CXX
#define XMLBEANSXX_LOGGING
//#endif //HAVE_LIBLOG4CXX


#ifndef XMLBEANSXX_LOGGING
#define TRACER(log,method)
#undef LOG4CXX_DEBUG
#define LOG4CXX_DEBUG(log,text)

#else  // XMLBEANSXX_LOGGING

#define TRACER(log,method) Tracer tracer(log,method)


#include <string>
#include <log4cxx/mdc.h>


namespace xmlbeansxx {

class Tracer {
public:
    Tracer(log4cxx::LoggerPtr logger, const std::string &text) : logger(logger), text(text) {
        LOG4CXX_DEBUG(logger,std::string("entering ") + text);
        previous = log4cxx::MDC::get("method");
        log4cxx::MDC::put("method", text);
    };

    Tracer(const std::string &text) {
        previous = log4cxx::MDC::get
                       ("method");
        log4cxx::MDC::put("method", text);
    };

    ~Tracer() {
        log4cxx::MDC::put("method", previous);
        if (logger)
            LOG4CXX_DEBUG(logger,std::string("leaving ") + text);
    };

protected:
    std::string previous;
    log4cxx::LoggerPtr logger;
    std::string text;
};

}

#endif // XMLBEANSXX_LOGGING

#endif//_XMLBEANSXX_TRACER_H_

