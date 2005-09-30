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


#ifndef _XMLBEANSXX_XMLOPTIONS_H_
#define _XMLBEANSXX_XMLOPTIONS_H_

#include "BoostAssert.h"
#include "Ptr.h"

namespace xmlbeansxx {
class XmlOptions_I;
BEGIN_CLASS(XmlOptions, XmlOptions_I)
END_CLASS()

/** This class is used to remember various xml options. It corresponds to Java org.apache.xmlbeans.XmlOptions class. */
class XmlOptions_I {
    private:
    bool _saveXmlDecl;    
    bool _validation;
    
public:

    XmlOptions_I() {
        _saveXmlDecl = true;
        _validation = false;
    }
    
    XmlOptions copy() {
        XmlOptions n = XmlOptions::New();
        (*n.ptr) = (*this);
        return n;
    }

    /** Specifies serialization option which prints '<?xml ... ?>' at the beginning of document. */
    void setSaveXmlDecl(bool d) {
        _saveXmlDecl = d;
    }
    
    bool getSaveXmlDecl() {
        return _saveXmlDecl;
    }

    bool getValidation() {
        return _validation;
    }

    void setValidation(bool validation) {
        _validation = validation;
    }
};
}
#endif//_XMLBEANSXX_XMLOPTIONS_H_
