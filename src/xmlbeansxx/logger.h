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


#ifndef _XMLBEANSXX_LOGGER_H_
#define _XMLBEANSXX_LOGGER_H_


#include "config_auto.h"

#ifdef HAVE_LIBLOG4CXX

#include <log4cxx/logger.h>

#define LOGGER_PTR(name) log4cxx::LoggerPtr name
#define STATIC_LOGGER_PTR(name) static LOGGER_PTR(name)
#define GET_LOGGER(string) log4cxx::Logger::getLogger(string)
#define STATIC_LOGGER_PTR_SET(name,string) STATIC_LOGGER_PTR(name) = GET_LOGGER(string)
#define LOGGER_PTR_SET(name,string) LOGGER_PTR(name) = GET_LOGGER(string)
    
#else // HAVE_LIBLOG4CXX

#define LOGGER_PTR(name) 
#define STATIC_LOGGER_PTR(name) 
#define GET_LOGGER(string)
#define STATIC_LOGGER_PTR_SET(name,string) 
#define LOGGER_PTR_SET(name,string)


#define TRACER(logger,string)
#define LOG4CXX_FATAL(logger,string)
#define LOG4CXX_DEBUG(logger,string)
#define LOG4CXX_INFO(logger,string)
#define LOG4CXX_ERROR(logger,string)


#endif


#endif
