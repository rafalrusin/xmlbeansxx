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
    limitations under the License. 
  */


#include "BeansException.h"

using namespace std;

	
namespace xmlbeansxx {

LOGGER_PTR_SET(BeansException::log,"xmlbeansxx.BeansException");

BeansException::BeansException(const std::string msg) throw() :msg(msg) { 
    LOG4CXX_DEBUG(log,msg);
}

const char *BeansException::what() const throw() {
	return msg.c_str();
}
BeansException::~BeansException() throw() {}
string BeansException::getMessage() const {
    return msg;
}

NotImplementedException::NotImplementedException(const std::string msg):BeansException("Not implemented:"+msg) {}

XmlException::XmlException(const std::string msg):BeansException("Xml:"+msg) {}
XmlParseException::XmlParseException(const std::string msg):XmlException(string("While parsing: ")+msg) {}
XmlSerializeException::XmlSerializeException(const std::string msg):XmlException(string("While serializing: ")+msg) {}
XmlIllegalValueException::XmlIllegalValueException(std::string schemaTypeName,std::string setValue):XmlException("Setting invalid value '"+setValue+"' into object of type '"+schemaTypeName+"'") {}
ClassCastException::ClassCastException(std::string msg): BeansException("Invalid Class Cast: "+msg) {}
    
XmlSimpleTypeException::XmlSimpleTypeException(std::string message,std::string simpleContent):XmlException(message+" in '"+simpleContent+"'") {}
IllegalArgumentsException::IllegalArgumentsException(std::string msg): BeansException(std::string("Illegal arguments: ")+msg) {}
IllegalStateException::IllegalStateException(std::string msg): BeansException(std::string("Illegal state ")+msg) {}

AssertionFailedException::AssertionFailedException(std::string msg): BeansException("Assertion Failed: "+msg) {}
NullPtrException::NullPtrException(std::string msg): BeansException("NUllPtr exception: "+msg) {}
}
