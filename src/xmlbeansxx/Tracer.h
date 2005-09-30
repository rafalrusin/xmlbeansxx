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

#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include <log4cxx/mdc.h>
#include <string>

#define TRACER(log,method) Tracer tracer(log,method);

namespace xmlbeansxx {

/** Provides tracing method calls. */
class Tracer {
protected:
    String previous;
    log4cxx::LoggerPtr logger;
    String text;
public:
    Tracer(log4cxx::LoggerPtr logger, const String &text) : logger(logger), text(text) {
        LOG4CXX_DEBUG(logger,"entering " + text);
        previous = log4cxx::MDC::get("method");
        log4cxx::MDC::put("method", text);
    };

    Tracer(const String &text) {
        previous = log4cxx::MDC::get
                       ("method");
        log4cxx::MDC::put("method", text);
    };

    ~Tracer() {
        log4cxx::MDC::put("method", previous);
        if (logger)
            LOG4CXX_DEBUG(logger,"leaving " + text);
    };

};

}
#endif//_XMLBEANSXX_TRACER_H_

