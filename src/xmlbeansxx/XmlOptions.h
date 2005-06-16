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
#include <boost/shared_ptr.hpp>



namespace xmlbeansxx {

/** This class is used to remember various xml options. It corresponds to Java XmlBeans XmlOptions class. */
class XmlOptions {
    bool v,d,tp;
public:
    XmlOptions():v(false),d(true),tp(true) {}

    void setValidation(bool v) {
        XmlOptions::v=v;
    }
    bool getValidation() {
        return v;
    }
    /** Specifies serialization option which prints '<?xml ... ?>' at the beginning of document. */
    void setPrintXmlDeclaration(bool d) {
        XmlOptions::d=d;
    }
    bool getPrintXmlDeclaration() {
        return d;
    }

    /**
     * @deprecated
     */
    bool getTypedParsing() {
        return tp;
    }
    
    /** 
     * @deprecated This option specifies that objects created during parsing of xml will have appropriate class.
     * If it is not set, then all created objects are XmlObject.
     */
    void setTypedParsing(bool v) {
        tp=v;
    }
};
typedef boost::shared_ptr<XmlOptions> XmlOptionsPtr;

}
#endif//_XMLBEANSXX_XMLOPTIONS_H_
