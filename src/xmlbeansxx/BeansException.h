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

#include <string>
#include <exception>
#include <log4cxx/logger.h>


namespace xmlbeansxx {

/** xmlbeansxx throw only exceptions of this kind (but some other exceptions may be thrown out of xmlbeansxx, eg. std::bad_alloc). */
class BeansException: public std::exception {
private:
    static log4cxx::LoggerPtr log;
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

/** Exception says that some feature was not implemented. */
class NotImplementedException: public BeansException {
    public:
    NotImplementedException(const std::string msg);
};

/** Exception says that Xml error occured. */
class XmlException: public BeansException {
public:
    XmlException(const std::string msg);
};

/** Exception says that error occured during parsing of xml. */
class XmlParseException: public XmlException {
public:
    XmlParseException(const std::string msg);
};

/** Exception says that error occured during serializing of xml. */
class XmlSerializeException: public XmlException {
public:
    XmlSerializeException(const std::string msg);
};

/** Exception says that invalid value was set into some XmlObject (eg. "abc" into XmlInteger). */
class XmlIllegalValueException:public XmlException {
    public:
    XmlIllegalValueException(std::string schemaTypeName,std::string setValue);
};

/** Exception says that exception occured during access to xml.*/
class XmlSimpleTypeException: public XmlException {
    public:
    XmlSimpleTypeException(std::string message,std::string simpleContent);
};

/** Exception raises while java_cast did not succeed. */
class ClassCastException:public BeansException {
    public:
    ClassCastException(std::string msg);
};

/** Exception raises during access to XmlObject contents */
class IllegalArgumentsException:public BeansException {
    public:
    IllegalArgumentsException(std::string msg);
};

/** Exception says that internal error occured */
class IllegalStateException:public BeansException {
    public:
    IllegalStateException(std::string msg=std::string());
};

/** Exception says that BOOST_ASSERT failed. */
class AssertionFailedException:public BeansException {
    public:
    AssertionFailedException(std::string msg);
};
}

#endif//_XMLBEANSXX_EXCEPTION_H_
