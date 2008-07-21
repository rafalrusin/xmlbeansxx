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

void XmlObjectDocument::_setXmlObject(const xmlbeansxx::XmlObject& p, bool castTest, const char* toString) {
  if(!castTest) {
    xmlbeansxx::Contents::Walker::ElemsType elements = xmlbeansxx::Contents::Walker::getElems(p);
    if(elements.size()==1) { 
      XmlObject e = elements[0].second;
      createContents();
      xmlbeansxx::Contents::Walker::setElem(*this,getSchemaType()->documentElementName,e.contents);
      return;
    }

    throw xmlbeansxx::ClassCastException(p.getSchemaType()->className + " to " + toString);
  }
  swapContents(p.contents);
}


xmlbeansxx::SchemaType XmlObjectDocument::initSchemaType() {
  xmlbeansxx::SchemaType st(typeid(xmlbeansxx::XmlObjectDocument));
  st.createFn=xmlbeansxx::XmlObjectDocument::Factory::newInstanceXmlObject;
  st.whitespaceRule=xmlbeansxx::SchemaType::WS_UNSPECIFIED;
  st.className="xmlbeansxx::XmlObjectDocument";
  st.contentType=xmlbeansxx::SchemaType::ELEMENT_CONTENT;
  st.name = xmlbeansxx::QName::store("http://xmlbeansxx.touk.pl/xmlbeansxx/innerType", "xmlbeansxx__XmlObjectDocument");
  st.processContents=true;
  return st;
}
XmlObjectDocument::XmlObjectDocument():xmlbeansxx::XmlObject() {  }
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
XmlObjectDocument XmlObjectDocument::Factory::parse(std::istream &in,xmlbeansxx::XmlOptions options) {
  XmlObjectDocument doc=XmlObjectDocument::Factory::newInstance();
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
    xmlbeansxx::globalTypeSystem()->addDocumentType(xmlbeansxx::XmlObjectDocument::type());

  }
};
static TypesExistence te;
}//--namespace


