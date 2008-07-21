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


#include "XmlArray.h"

#include "XmlBeans.h"

	
namespace xmlbeansxx {

XMLBEANSXX_LOGGER_PTR_SET(XmlArray_log,"xmlbeansxx.XmlArray");

const QName AnyXmlArray::Names::elementName = QName::store("http://xmlbeans.apache.org/definitions","e");


SchemaType AnyXmlArray::initSchemaType() {
      	xmlbeansxx::SchemaType st(typeid(AnyXmlArray));
      	st.createFn=AnyXmlArray::Factory::newInstanceXmlObject;
      	st.whitespaceRule=xmlbeansxx::SchemaType::WS_COLLAPSE;
      	st.className="xmlbeansxx::XmlArray";
      	st.name=QName::store(XmlBeans::innerType_ns(),"XmlArray");
	st.isArray=true;
	st.processContents=true;
	st.elements[QName::store("http://xmlbeans.apache.org/definitions","e")]=xmlbeansxx::SchemaPropertyPtr(new xmlbeansxx::SchemaProperty(1 ,XmlObject::type()->name, std::string()));
	return st;
}



AnyXmlArray AnyXmlArray::Factory::newInstance() {
    AnyXmlArray obj;
    obj.createContents();
    return obj;
}

xmlbeansxx::XmlObjectPtr AnyXmlArray::Factory::newInstanceXmlObject() {
    return xmlbeansxx::XmlObjectPtr((xmlbeansxx::XmlObject *) new AnyXmlArray());
}	

    
    
const xmlbeansxx::SchemaType * AnyXmlArray::getOrginSchemaType() const {
	return AnyXmlArray::type();
}

const xmlbeansxx::SchemaType * AnyXmlArray::type() {
        static xmlbeansxx::SchemaType schemaType = initSchemaType();
	return &schemaType; 
}






}

