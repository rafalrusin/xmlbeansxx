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


#ifndef _XMLBEANSXX_BEANSEXCEPTION_H_
#define _XMLBEANSXX_BEANSEXCEPTION_H_

#include "BoostAssert.h"
#include <string>
#include <exception>
#include <log4cxx/logger.h>
#include "String.h"

namespace xmlbeansxx {

/** xmlbeansxx throw only exceptions of this kind (but some other exceptions may be thrown out of xmlbeansxx, eg. std::bad_alloc). */
class BeansException: public std::exception {
private:
    static log4cxx::LoggerPtr LOG;
    const String msg;
public:

    BeansException(const String msg) throw();
    virtual const char *what() const throw();
    virtual ~BeansException() throw();

    /**
     * @return String containg exception message
     */
    virtual String getMessage() const;
};

/** Exception says that some feature was not implemented. */
class NotImplementedException: public BeansException {
public:
    NotImplementedException(const String msg);
    NotImplementedException();
};

/** Exception says that Xml error occured. */
class XmlException: public BeansException {
public:
    XmlException(const String msg);
};

/** Exception says that error occured during parsing of xml. */
class XmlParseException: public XmlException {
public:
    XmlParseException(const String msg);
};

/** Exception says that error occured during serializing of xml. */
class XmlSerializeException: public XmlException {
public:
    XmlSerializeException(const String msg);
};

/** Exception says that invalid value was set into some XmlObject (eg. "abc" into XmlInteger). */
class XmlIllegalValueException:public XmlException {
public:
    XmlIllegalValueException(String schemaTypeName,std::string setValue);
};

/** Exception says that exception occured during access to xml.*/
class XmlSimpleTypeException: public XmlException {
public:
    XmlSimpleTypeException(String message,std::string simpleContent);
};

/** Exception raises while java_cast did not succeed. */
class ClassCastException:public BeansException {
public:
    ClassCastException(String msg);
};

/** Exception raises during access to XmlObject contents */
class IllegalArgumentException:public BeansException {
public:
    IllegalArgumentException(String msg);
};

/** Exception says that internal error occured */
class IllegalStateException:public BeansException {
public:
    IllegalStateException(String msg=std::string());
};

/** Exception says that BOOST_ASSERT failed. */
class AssertionFailedException:public BeansException {
public:
    AssertionFailedException(String msg);
};

class XmlValueOutOfRangeException:public XmlException {
public:
    XmlValueOutOfRangeException();
    XmlValueOutOfRangeException(const String &code, const String &args);
};
}

#endif//_XMLBEANSXX_EXCEPTION_H_
