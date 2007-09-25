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




#include <xmlbeansxx/XmlObjectDocument.h>
#include <xmlbeansxx/XmlParser.h>
#include <xmlbeansxx/XmlArray.h>

using namespace xmlbeansxx;

/*XmlObject XmlObjectDocument::Factory::parse(std::istream &in,xmlbeansxx::XmlOptions options) {
  XmlObject doc=XmlObject::Factory::newInstance();
  xmlbeansxx::LibXMLParser p(options);
  p.parse(in,doc);
  return doc;
}
XmlObject XmlObjectDocument::Factory::parse(const std::string &str,xmlbeansxx::XmlOptions options) { 
  std::istringstream in(str);
  return parse(in,options);
}
*/

xmlbeansxx::SchemaType XmlObjectDocument::initSchemaType() {
  xmlbeansxx::SchemaType st(typeid(xmlbeansxx::XmlObjectDocument));
  st.createFn=xmlbeansxx::XmlObjectDocument::Factory::newInstanceXmlObject;
  st.createArrayFn=xmlbeansxx::XmlObjectDocument::Factory::newXmlArrayInstance;
  st.whitespaceRule=xmlbeansxx::SchemaType::WS_UNSPECIFIED;
  st.className="xmlbeansxx::XmlObjectDocument";
  st.contentType=xmlbeansxx::SchemaType::ELEMENT_CONTENT;
  st.processContents=true;
  return st;
}
XmlObjectDocument::XmlObjectDocument(const xmlbeansxx::Void &v):xmlbeansxx::XmlObject(v) { }
XmlObjectDocument::XmlObjectDocument():xmlbeansxx::XmlObject(xmlbeansxx::Void()) {  }
XmlObjectDocument::XmlObjectDocument(const xmlbeansxx::ContentsPtr& p):xmlbeansxx::XmlObject(p) { } 

XmlObjectDocument::XmlObjectDocument(const xmlbeansxx::XmlObject& p) {
  setXmlObject(p);
}
void XmlObjectDocument::setXmlObject(const xmlbeansxx::XmlObject& p) {
  swapContents(p.contents);
}
const xmlbeansxx::SchemaType *XmlObjectDocument::getOrginSchemaType() const {
  return XmlObjectDocument::type();
}
const xmlbeansxx::SchemaType *XmlObjectDocument::type() {
  static xmlbeansxx::SchemaType schemaType=initSchemaType();
  return &schemaType; 
}
xmlbeansxx::XmlObjectDocument XmlObjectDocument::Factory::newInstance() {
  xmlbeansxx::XmlObjectDocument obj;
  obj.createContents();
  return obj;
} 
xmlbeansxx::XmlObjectPtr XmlObjectDocument::Factory::newInstanceXmlObject() {
  return xmlbeansxx::XmlObjectPtr(new XmlObjectDocument());
}
xmlbeansxx::XmlObjectPtr XmlObjectDocument::Factory::newXmlArrayInstance() { return xmlbeansxx::XmlObjectPtr(new xmlbeansxx::XmlArray<XmlObjectDocument >()); }
XmlObjectDocument XmlObjectDocument::Factory::parse(std::istream &in,xmlbeansxx::XmlOptions options) {
  XmlObjectDocument doc;
  xmlbeansxx::XmlParserPtr p = XmlParser::Factory::newInstance(options);
  p->parse(in,doc);
  return doc;
}

XmlObjectDocument XmlObjectDocument::Factory::parse(const std::string &str,xmlbeansxx::XmlOptions options) { 
  std::istringstream in(str);
  return parse(in,options);
}
void XmlObjectDocument::serialize(std::ostream &out,xmlbeansxx::XmlOptions opts) const{
  xmlbeansxx::Contents::Walker::serializeDocument(*this,out,opts,type());
}

namespace   {
class TypesExistence {
  public:
  TypesExistence() {
    xmlbeansxx::XmlObjectDocument::type();
  }
};
static TypesExistence te;
}//--namespace


