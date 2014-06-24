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


#ifndef _XMLBEANSXX_BEANSEXCEPTION_H_
#define _XMLBEANSXX_BEANSEXCEPTION_H_

#include <string>
#include <exception>
#include "logger.h"


namespace xmlbeansxx {

class BeansException: public std::exception {
private:
    XMLBEANSXX_STATIC_LOGGER_PTR(log);
    const std::string msg;
public:

    BeansException(const std::string msg) throw();
	virtual const char *what() const throw();
    virtual ~BeansException() throw();

    /**
     * @return std::string containg exception message
     */
    virtual std::string getMessage() const;
};

class NotImplementedException: public BeansException {
    public:
    NotImplementedException(const std::string msg);
};

class XmlException: public BeansException {
public:
    XmlException(const std::string msg);
};

class XmlParseException: public XmlException {
public:
    XmlParseException(const std::string msg);
};

class XmlSerializeException: public XmlException {
public:
    XmlSerializeException(const std::string msg);
};

class XmlIllegalValueException:public XmlException {
    public:
    XmlIllegalValueException(std::string schemaTypeName,std::string setValue);
};

class XmlSimpleTypeException: public XmlException {
    public:
    XmlSimpleTypeException(std::string message,std::string simpleContent);
};

class ClassCastException:public BeansException {
    public:
    ClassCastException(std::string msg);
};

class CalendarException:public BeansException {
    public:
    CalendarException(std::string msg);
};


class IllegalArgumentsException:public BeansException {
    public:
    IllegalArgumentsException(std::string msg);
};

class IllegalStateException:public BeansException {
    public:
    IllegalStateException(std::string msg=std::string());
};

class AssertionFailedException:public BeansException {
    public:
    AssertionFailedException(std::string msg);
};

class NullPtrException:public BeansException {
    public:
    NullPtrException(std::string msg);
};
}

#endif//_XMLBEANSXX_EXCEPTION_H_
