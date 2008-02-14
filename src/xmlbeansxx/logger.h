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


#include <xmlbeansxx/config.h>

#ifdef  XMLBEANSXX_WITH_LOG4CXX

#include <log4cxx/logger.h>

// undefining log4cxx definitions
#undef VERSION
#undef PACKAGE_VERSION
#undef PACKAGE_TARNAME
#undef PACKAGE_STRING
#undef PACKAGE_NAME
#undef PACKAGE_BUGREPORT
#undef PACKAGE


#define XMLBEANSXX_LOGGING 1


#define XMLBEANSXX_LOGGER_PTR(name) log4cxx::LoggerPtr name
#define XMLBEANSXX_STATIC_LOGGER_PTR(name) static XMLBEANSXX_LOGGER_PTR(name)
#define XMLBEANSXX_GET_LOGGER(string) log4cxx::Logger::getLogger(string)
#define XMLBEANSXX_STATIC_LOGGER_PTR_SET(name,string) XMLBEANSXX_STATIC_LOGGER_PTR(name) = XMLBEANSXX_GET_LOGGER(string)
#define XMLBEANSXX_LOGGER_PTR_SET(name,string) XMLBEANSXX_LOGGER_PTR(name) = XMLBEANSXX_GET_LOGGER(string)

#define XMLBEANSXX_FATAL(logger,string)  LOG4CXX_FATAL(logger,string)
#define XMLBEANSXX_DEBUG(logger,string)  LOG4CXX_DEBUG(logger,string)
#define XMLBEANSXX_INFO(logger,string)   LOG4CXX_INFO(logger,string)
#define XMLBEANSXX_ERROR(logger,string)  LOG4CXX_ERROR(logger,string)
#define XMLBEANSXX_WARN(logger,string)  LOG4CXX_WARN(logger,string)

    
#else // XMLBEANSXX_WITH_LOG4CXX

#undef XMLBEANSXX_LOGGING

#define XMLBEANSXX_LOGGER_PTR(name)
#define XMLBEANSXX_STATIC_LOGGER_PTR(name)
#define XMLBEANSXX_GET_LOGGER(string)
#define XMLBEANSXX_STATIC_LOGGER_PTR_SET(name,string)
#define XMLBEANSXX_LOGGER_PTR_SET(name,string)

#define XMLBEANSXX_FATAL(logger,string)
#define XMLBEANSXX_DEBUG(logger,string)
#define XMLBEANSXX_INFO(logger,string) 
#define XMLBEANSXX_ERROR(logger,string)
#define XMLBEANSXX_WARN(logger,string) 




#endif // XMLBEANSXX_WITH_LOG4CXX


#endif
