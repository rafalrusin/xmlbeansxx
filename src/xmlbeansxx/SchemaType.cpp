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


#include "SchemaType.h"
#include "XmlTypesGen.h"


namespace xmlbeansxx {

Map_QName_SchemaPropertyPtr::Map_QName_SchemaPropertyPtr() {}
Map_QName_SchemaPropertyPtr::Map_QName_SchemaPropertyPtr(const Map_QName_SchemaPropertyPtr &b): std::map<QName,SchemaPropertyPtr>(b) {}

void SchemaType::init() {
    fractionDigits=-1;
    contentType=MIXED_CONTENT;
    isArray=false;
}

SchemaType::SchemaType(const std::type_info &classTypeInfo): classTypeInfo(classTypeInfo) {
    init();
}

XmlObjectPtr SchemaType::createSubObject(const QName &name) const {
    SchemaPropertyPtr sp;
    Map_QName_SchemaPropertyPtr::const_iterator it=elements.find(name);
    if (it!=elements.end()) {
        return it->second->schemaType->createFn();
    } else {
    	//searching for a Document type
	XmlObjectPtr root = globalTypeSystem()->createDocumentByName(name);
	//Document type not found (returning an XmlObject)
	if(!root)  return XmlObject::Factory::newInstanceXmlObject(); 

	//Get the inner type of the document
	QName documentElementName = root->getSchemaType()->documentElementName;
	return root->getSchemaType()->createSubObject(documentElementName);
	
	
    }
}
    
SchemaPropertyPtr SchemaType::findAttribute(const QName &name) const {
    Map_QName_SchemaPropertyPtr::const_iterator it=attributes.find(name);
    if (it==attributes.end()) return SchemaPropertyPtr();
    else return it->second;
}

SchemaPropertyPtr SchemaType::findElement(const QName &name) const {
    Map_QName_SchemaPropertyPtr::const_iterator it=elements.find(name);
    if (it==elements.end()) return SchemaPropertyPtr();
    else return it->second;
}

SchemaType::CONTENT_TYPE SchemaType::getContentType() const {
    return contentType;
}

}
