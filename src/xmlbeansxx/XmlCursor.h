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


#ifndef _XMLBEANSXX_XMLCURSOR_H_
#define _XMLBEANSXX_XMLCURSOR_H_


#include "XmlTypesGen.h"
#include "XmlBeans.h"
#include <stack>

namespace xmlbeansxx {

class XmlCursor : public  XmlObject {
public:
	enum TokenType {
	/** No token. */
	INT_NONE      = 0,
        /** The start-document token. */ 
        INT_STARTDOC  = 1,
        /** The end-document token.  */ 
        INT_ENDDOC    = 2,
        /** The start-element token. */ 
        INT_START     = 3,
        /** The end-element token.  */ 
        INT_END       = 4,
        /** The text token.  */ 
        INT_TEXT      = 5,
        /** The attribute token.  */ 
        INT_ATTR      = 6,
        /** The namespace declaration token. */ 
        INT_NAMESPACE = 7,
        /** The comment token.  (not used) */ 
        INT_COMMENT   = 8,
        /** The processing instruction token. (not used)  */ 
        INT_PROCINST  = 9
	};
		
	
	        /** True if no token. */
        bool isNone      ( ) const { return currentTokenType() == INT_NONE;      }
        /** True if is start-document token. */
        bool isStartdoc  ( ) const { return currentTokenType() == INT_STARTDOC;  }
        /** True if is end-document token. */
        bool isEnddoc    ( ) const { return currentTokenType() == INT_ENDDOC;    }
        /** True if is start-element token. */
        bool isStart     ( ) const { return currentTokenType() == INT_START;     }
        /** True if is end-element token. */
        bool isEnd       ( ) const { return currentTokenType() == INT_END;       }
        /** True if is text token. */
        bool isText      ( ) const { return currentTokenType() == INT_TEXT;      }
        /** True if is attribute token. */
        bool isAttr      ( ) const { return currentTokenType() == INT_ATTR;      }
        /** True if is namespace declaration token. */
        bool isNamespace ( ) const { return currentTokenType() == INT_NAMESPACE; }
        /** True if is comment token. */
        bool isComment   ( ) const { return currentTokenType() == INT_COMMENT;   }
        /** True if is processing instruction token. */
        bool isProcinst  ( ) const { return currentTokenType() == INT_PROCINST;  }

        /** True if is start-document or start-element token */
        bool isContainer ( ) const { return currentTokenType() == INT_STARTDOC  || currentTokenType() == INT_START; }
        /** True if is end-document or end-element token */
        bool isFinish    ( ) const { return currentTokenType() == INT_ENDDOC    || currentTokenType() == INT_END;   }
        /** True if is attribute or namespace declaration token */
        bool isAnyAttr   ( ) const { return currentTokenType() == INT_NAMESPACE || currentTokenType() == INT_ATTR;  }


	XmlCursor(const XmlObject& o): XmlObject(o) , pos(-1) {};

	
	QName getName() const ;	
	TokenType currentTokenType() const ;
	XmlObject getObject();
	void beginElement(const QName &name) ;
	int comparePosition(const XmlCursor &cursor) const ;
	std::string getAttributeText(const QName &attrName) const ;
	bool hasNextToken() const ;
	
	bool hasPrevToken() const ;
	int insertAttributeWithValue(const QName &name,const XmlAnySimpleType &o) ;	
	int insertAttributeWithValue(const QName &name,const std::string &value) ;
	int insertElement(const QName &name,const XmlObject &o);
	int insertNamespace(std::string prefix, std::string namespaceURI) ;
//	bool removeAttribute(const QName& attrName) ;
	void setName(const QName &name) ;
	bool toChild(int index=1) ;
	bool toChild(const QName &name, int index=1) ;	
	TokenType toEndToken() ;
	bool toFirstAttribute() ;
	bool toFirstChild() ;
	
	TokenType toFirstContentToken() ;
	bool toLastAttribute() ;
	bool toLastChild() ;
	bool toNextAttribute() ;
	bool toNextSibling(int index = 1) ;
	bool toNextSibling(const QName &name,int index = 1) ;
	TokenType toNextToken() ;
	TokenType toPrevToken() ;
	
	bool toParent() ;
	bool toPrevAttribute() ;
	bool toPrevSibling(int index=1) ;
	void toStartDoc() ;
	
	std::string getTextValue() ;
	void setTextValue(const std::string& s) ;
	void insertChars(const std::string& text);
	bool removeXml();



//private:

	typedef int posType;
	typedef std::stack<std::pair<posType,ContentsPtr> > stackType;	
	stackType stack;	
	posType pos;

	TokenType getAttributeType(const QName& name) const ;
	TokenType getElementType(const QName& name) const ;
	void remember();
	void restore();
	void rememberAndSwap(ContentsPtr c);
	
	ElemDict::value_type &getPos() const ;

	int countElems() const ;
	int countAttrs() const ;

	
};


}


#endif //XMLBEANSPP_XMLCURSOR_H_

