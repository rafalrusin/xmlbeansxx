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


#ifndef _XMLBEANSXX_XMLCURSOR_H_
#define _XMLBEANSXX_XMLCURSOR_H_


#include "XmlTypesGen.h"

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
        INT_PROCINST  = 9,
	};
	
	TokenType _currentTokenType;
	
	TokenType currentTokenType() {
		return _currentTokenType;
	}
	
	
	        /** True if no token. */
        bool isNone      ( ) { return currentTokenType() == INT_NONE;      }
        /** True if is start-document token. */
        bool isStartdoc  ( ) { return currentTokenType() == INT_STARTDOC;  }
        /** True if is end-document token. */
        bool isEnddoc    ( ) { return currentTokenType() == INT_ENDDOC;    }
        /** True if is start-element token. */
        bool isStart     ( ) { return currentTokenType() == INT_START;     }
        /** True if is end-element token. */
        bool isEnd       ( ) { return currentTokenType() == INT_END;       }
        /** True if is text token. */
        bool isText      ( ) { return currentTokenType() == INT_TEXT;      }
        /** True if is attribute token. */
        bool isAttr      ( ) { return currentTokenType() == INT_ATTR;      }
        /** True if is namespace declaration token. */
        bool isNamespace ( ) { return currentTokenType() == INT_NAMESPACE; }
        /** True if is comment token. */
        bool isComment   ( ) { return currentTokenType() == INT_COMMENT;   }
        /** True if is processing instruction token. */
        bool isProcinst  ( ) { return currentTokenType() == INT_PROCINST;  }

        /** True if is start-document or start-element token */
        bool isContainer ( ) { return currentTokenType() == INT_STARTDOC  || currentTokenType() == INT_START; }
        /** True if is end-document or end-element token */
        bool isFinish    ( ) { return currentTokenType() == INT_ENDDOC    || currentTokenType() == INT_END;   }
        /** True if is attribute or namespace declaration token */
        bool isAnyAttr   ( ) { return currentTokenType() == INT_NAMESPACE || currentTokenType() == INT_ATTR;  }


	XmlCursor(const XmlOcject& o,TokenType c = INT_START): XmlObject(o) , _currentTokenType(c) {};

	p
	vector

};


}


#endif //XMLBEANSPP_XMLCURSOR_H_

