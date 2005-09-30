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


#include "BeansException.h"
using namespace std;

	
namespace xmlbeansxx {

log4cxx::LoggerPtr BeansException::LOG = log4cxx::Logger::getLogger(String("xmlbeansxx.BeansException"));

BeansException::BeansException(const String msg) throw() :msg(msg) { 
    LOG4CXX_DEBUG(LOG, msg);
}

const char *BeansException::what() const throw() {
	return msg.c_str();
}
BeansException::~BeansException() throw() {}
string BeansException::getMessage() const {
    return msg;
}

NotImplementedException::NotImplementedException():BeansException("Not implemented") {}
NotImplementedException::NotImplementedException(const String msg):BeansException("Not implemented:"+msg) {}

XmlException::XmlException(const String msg):BeansException("Xml:"+msg) {}
XmlParseException::XmlParseException(const String msg):XmlException(string("While parsing: ")+msg) {}
XmlSerializeException::XmlSerializeException(const String msg):XmlException(string("While serializing: ")+msg) {}
XmlIllegalValueException::XmlIllegalValueException(String schemaTypeName,std::string setValue):XmlException("Setting invalid value '"+setValue+"' into object of type '"+schemaTypeName+"'") {}
ClassCastException::ClassCastException(String msg): BeansException("Invalid Class Cast: "+msg) {}
    
XmlSimpleTypeException::XmlSimpleTypeException(String message,std::string simpleContent):XmlException(message+" in '"+simpleContent+"'") {}
IllegalArgumentException::IllegalArgumentException(String msg): BeansException(std::string("Illegal arguments: ")+msg) {}
IllegalStateException::IllegalStateException(String msg): BeansException(std::string("Illegal state ")+msg) {}

AssertionFailedException::AssertionFailedException(String msg): BeansException("Assertion Failed: "+msg) {}

XmlValueOutOfRangeException::XmlValueOutOfRangeException(): XmlException("Value out of range. ") {}
XmlValueOutOfRangeException::XmlValueOutOfRangeException(const String &code, const String &args): XmlException("Value out of range. " + code + "  " + args) {}
}
