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


#include "XmlCursor.h"
#include "ContentsImpl.h"

namespace xmlbeansxx {




	
	// position in the current XmlObject 
	// |                       |atributes| = A                      |                              |elements| = B                            |
	// |------------------------------------------------------------|------------------------------------------------------------------------|----------------|
	// |pos=0(first attr)    pos=i(i-th attr)     pos=A-1(last attr)|pos=A (first elem)   pos=A+i (i-th elem)            pos=A+B-1(last elem)|pos=A+B(INT_END)|
	//
	// (pos < 0 || pos > A+B) == INT_NONE
	
	
	// for empty stack (stack.empty() == true)
	// pos == -1   INT_STARTDOC
	// pos == A+B  INT_END


XmlCursor::TokenType XmlCursor::getAttributeType(const QName& name) const {
		if(name->first==XmlBeans::xmlns() ) return INT_NAMESPACE;
		else return INT_ATTR;
}
XmlCursor::TokenType XmlCursor::getElementType(const QName& name) const {
		if(name == XmlBeans::textElementName()) return INT_TEXT;
		else return INT_START;
}
void XmlCursor::remember(){
		stack.push(std::make_pair(pos,contents));
}
void XmlCursor::restore() {
		pos = stack.top().first;
		contents.swap(stack.top().second);
		stack.pop();
}
void XmlCursor::rememberAndSwap(ContentsPtr c) {
		remember();
		contents.swap(c);
		pos = 0;
}

	
ElemDict::value_type& XmlCursor::getPos() const {
		int A = countAttrs();
		int B = countElems();
		if(pos < 0 || pos >= A + B)  throw IllegalStateException("XmlCursor: pos out of range ");
		if(pos <  A) return contents->attrs.contents.at(pos);
		return contents->elems.contents.at(pos-A);
}
int XmlCursor::countElems() const {
		return contents->elems.contents.size();
}
int XmlCursor::countAttrs() const {
		return contents->attrs.contents.size();
}

QName XmlCursor::getName() const {
		try {
			ElemDict::value_type v = getPos();
			return v.name;
		} catch (...) {
			return XmlBeans::xml_fragment();
		}
}
	
XmlCursor::TokenType XmlCursor::currentTokenType() const {
		int A = countAttrs();
		int B = countElems();
		if(stack.empty()) {
			if(pos ==  -1) return INT_STARTDOC;
			if(pos == A+B) return INT_ENDDOC;
		}
		if(pos < 0 || pos > A+B) return INT_NONE;
		if(pos == A + B)  return INT_END;
		if(pos <  A) return getAttributeType(getName());
		//pos >= A 
		return getElementType(getName());
}

XmlObject XmlCursor::getObject(){
		ElemDict::value_type v = getPos();
		if(v.name == XmlBeans::textElementName()) return XmlString(v.value->getSimpleContent());1;
		return XmlObject(v.value);
}
void XmlCursor::beginElement(const QName &name) {
		insertElement(name,XmlObject::Factory::newInstance());
		toNextToken();
}
int XmlCursor::comparePosition(const XmlCursor &cursor) const {
		throw NotImplementedException("XmlCursor: comparePosition");
}
std::string XmlCursor::getAttributeText(const QName &attrName) const {
		ContentsPtr c = contents->getAttr(attrName);
		if(c) return c->getSimpleContent();
		return std::string();
}
bool XmlCursor::hasNextToken() const {
		if(stack.empty() && isFinish()) return false;
		return true;
}
	
bool XmlCursor::hasPrevToken() const {
		if(stack.empty() && isContainer()) return false;
		return true;
}

int XmlCursor::insertAttributeWithValue(const QName &name,const XmlAnySimpleType &o) {
		int A = countAttrs();
		int count = pos;
		if(count >A) count = A;
		contents->attrs.contents.insert(contents->attrs.contents.begin()+count,ElemDict::value_type(name,o.contents));
		return count;
}
	
int XmlCursor::insertAttributeWithValue(const QName &name,const std::string &value) {
		return insertAttributeWithValue(name,XmlString(value));
}

int XmlCursor::insertElement(const QName &name,const XmlObject &o){ 
		int A = countAttrs();
		int B = countElems();
		int count = pos - A;
		if(count <0) count = 0;
		if(count >B) count = B;
		contents->elems.contents.insert(contents->elems.contents.begin()+count,ElemDict::value_type(name,o.contents));
		return count+A;
}
int XmlCursor::insertNamespace(std::string prefix, std::string namespaceURI) {
		return insertAttributeWithValue(QName(XmlBeans::xmlns(),prefix),namespaceURI);
} 
/*	bool removeAttribute(const QName& attrName) {
		if(contents->getAttr(attrName)) {
			contents->setAttr(attrName,ContentsPtr());
			return true;
		}
		return false;
	}
*/
void XmlCursor::setName(const QName &name) {
		ElemDict::value_type &v = getPos();
		if(v.name == XmlBeans::textElementName()) 
			throw IllegalStateException("XmlCursor: setName (INT_TEXT)");
		v.name = name;
}
bool XmlCursor::toChild(int index) {
		int _pos = pos;
		int A = countAttrs();
		int B = countElems();
		for(pos=A;pos<A+B;pos++) {
			if(getPos().name!=XmlBeans::textElementName()) {
				if(!(--index)) return true;
			}
		}
		pos=_pos;
		return false;
}

bool XmlCursor::toChild(const QName &name, int index) {
		int _pos = pos;
		int A = countAttrs();
		int B = countElems();
		for(pos=A;pos<A+B;pos++) {
			if(getPos().name==name) {
				if(!(--index)) return true;
			}
		}
		pos=_pos;
		return false;
}
	
XmlCursor::TokenType XmlCursor::toEndToken() {
		int A = countAttrs();
		int B = countElems();
	
		pos = A+B;
		return currentTokenType();
}

bool XmlCursor::toFirstAttribute() {
		int A = countAttrs();
		if(A<=0) return false;
		pos = 0;
		return true;
}
bool XmlCursor::toFirstChild() {
		return toChild();
}
	
XmlCursor::TokenType XmlCursor::toFirstContentToken() {
		if(isStart() && !(getPos().value)) return INT_NONE;
		if(isContainer()) {
			toNextToken();
			int A = countAttrs();
			pos = A;
			return currentTokenType();
		}
		return INT_NONE;
}

bool XmlCursor::toLastAttribute() {
		int A = countAttrs();
		if(A<=0) return false;
		pos = A-1;
		return true;
}

bool XmlCursor::toLastChild() {
	 	int _pos=pos;
		toEndToken();
		if(toPrevSibling())
			return true;
		pos=_pos;
		return false;
}
bool XmlCursor::toNextAttribute() {
		int A = countAttrs();
		if(pos < A-1) {
			pos++;
			return true;
		}
		return false;
}
bool XmlCursor::toNextSibling(int index) {
		int _pos = pos;
		int A = countAttrs();
		int B = countElems();
		if(pos>=A+B-1) return false;
		pos++;
		if(pos < A) pos = A;
		for(;pos<A+B;pos++) {
			if(getPos().name!=XmlBeans::textElementName()) {
				if(!(--index)) return true;
			}
		}
		pos=_pos;
		return false;	
}

bool XmlCursor::toNextSibling(const QName &name,int index) {
		int _pos = pos;
		int A = countAttrs();
		int B = countElems();
		if(pos>=A+B-1) return false;
		pos++;
		if(pos < A) pos = A;
		for(pos;pos<A+B;pos++) {
			if(getPos().name==name) {
				if(!(--index)) return true;
			}
		}
		pos=_pos;
		return false;	
}
XmlCursor::TokenType XmlCursor::toNextToken() {
		int A = countAttrs();
		int B = countElems();
		if(pos < 0) {
			pos++;
			return currentTokenType();
		}
		if(pos >= A+B) {
			pos = A+B;
			toParent();
			pos++;
			return currentTokenType();
		}
		if(isContainer() && (getPos().value)){
			rememberAndSwap(getPos().value);
		}
		else pos++;
	
		return currentTokenType();
}

XmlCursor::TokenType XmlCursor::toPrevToken() {
		if(pos == 0) {
			if(!toParent())pos--;
			return currentTokenType();
		}
		pos --;
		if(isContainer() && (getPos().value)){
			rememberAndSwap(getPos().value);
			int A = countAttrs();
			int B = countElems();
			pos = A+B;
		}
	
		return currentTokenType();
}

	
bool XmlCursor::toParent() {
		if(stack.empty()) return false;
		restore();
		return true;
}

bool XmlCursor::toPrevAttribute()  {
		int A = countAttrs();
		if(A <=0 || pos == 0) return false;
		if(pos > A) pos = A;
		pos --;
		return true;		
}

bool XmlCursor::toPrevSibling(int index) {
		int _pos = pos;
		int A = countAttrs();
		int B = countElems();
		if(pos <=1) return false;
		pos--;
		if(pos > A+B-1) pos=A+B-1;
		for(pos;pos>=A;pos--) {
			if(getPos().name!=XmlBeans::textElementName()) {
				if(!(--index)) return true;
			}
		}
		pos=_pos;
		return false;	
		
}
void XmlCursor::toStartDoc() {
		while(toParent()){}
		pos=0;
}
	
std::string XmlCursor::getTextValue() {
		int A = countAttrs();
		int B = countElems();
		if(pos < 0 || pos >= A + B) return std::string();
		return getPos().value->getSimpleContent();
}
void XmlCursor::setTextValue(const std::string& s) {
		int A = countAttrs();
		int B = countElems();
		if(pos < 0 || pos >= A + B) return;
		return getPos().value->setSimpleContent(s);
}


bool XmlCursor::removeXml() {
		int A = countAttrs();
		int B = countElems();
		if(pos<0 || pos >= A+B) return false;
		if(pos < A) {
			int count = pos;
			contents->attrs.contents.erase(contents->attrs.contents.begin()+count);
		} else {
			int count = pos-A;
			contents->elems.contents.erase(contents->elems.contents.begin()+count);
		}
		return true;
}
	






void XmlCursor::insertChars(const std::string& text) {
	int A = countAttrs();
	int B = countElems();
	if(pos < A || pos > A+B) throw IllegalStateException("XmlCursor: insertChars, insertion is not allowed at the cursor's location");
	int count = pos- A;
	contents->elems.contents.insert(contents->elems.contents.begin()+count,ElemDict::value_type(XmlBeans::textElementName(),ContentsPtr(new StringContents(text))));
}








} // namespace xmlbeansxx
