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


#ifndef _XMLBEANSXX_XMLOBJECTDOCUMENT_H_
#define _XMLBEANSXX_XMLOBJECTDOCUMENT_H_


#include <xmlbeansxx/XmlTypesGen.h>


namespace xmlbeansxx {

class XmlObjectDocument {
public:
  class Factory {
  public:
    //Object Creation using parsing
    static XmlObject parse(std::istream &in,xmlbeansxx::XmlOptions options=xmlbeansxx::XmlOptions());
    static XmlObject parse(const std::string &str,xmlbeansxx::XmlOptions options=xmlbeansxx::XmlOptions());
  };
  
};

};
#endif

