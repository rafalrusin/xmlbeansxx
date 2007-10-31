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


#include "XmlParser.h"

#undef HAVE_LIBXML2
#include "config_auto.h"

#ifdef HAVE_LIBXML2
#include "LibXMLParser.h"
#endif

#ifdef HAVE_LIBXERCES_C
#include "XercesParser.h"
#endif

namespace xmlbeansxx {

    
XmlParserPtr XmlParser::Factory::newInstance() {
#ifdef HAVE_LIBXML2
    return XmlParserPtr(new LibXMLParser());
#else
    return XmlParserPtr(new XercesParser());
#endif
/*#ifdef HAVE_LIBXERCES_C
    return XmlParserPtr(new XercesParser());
#else
    return XmlParserPtr(new LibXMLParser());
#endif
*/
}

XmlParserPtr XmlParser::Factory::newInstance(const XmlOptions o) {
#ifdef HAVE_LIBXML2
    return XmlParserPtr(new LibXMLParser(o));
#else
    return XmlParserPtr(new XercesParser(o));
#endif

/*#ifdef HAVE_LIBXERCES_C
    return XmlParserPtr(new XercesParser(o));
#else
    return XmlParserPtr(new LibXMLParser(o));
#endif
*/
}


/*
XmlParser XmlParser::create(const XmlOptions &opts) {
    return new LibXMLParser(opts);
}

*/



// return namespace number bound to prefix or default namespace if
// no prefix was given
QName EmptyParser::nsSplit(const std::string &str, bool isAttr) {
    std::string::size_type pos=str.find(':');
    if (pos == std::string::npos) {
        if (isAttr) {
            return QName(std::string(""), str);
        } else {
            return QName(xmlContext.getNamespaceURI(""), str);
        }
    } else {
        return QName(xmlContext.getNamespaceURI(str.substr(0,pos)) ,str.substr(pos+1));
    }

}

QName EmptyParser::getQName(const char *prefix, const char *localname, bool isAttr) {
    if (prefix == NULL) {
    	if (isAttr) return QName("", localname);
	else return QName(xmlContext.getNamespaceURI(""), localname);
    } else return QName(xmlContext.getNamespaceURI(prefix), localname,prefix);
}

std::pair<std::string, std::string> EmptyParser::tagSplit(const std::string &str) {
    std::string::size_type pos = str.find(':');
    if (pos==std::string::npos) {
        return std::pair<std::string, std::string>("",str);
    } else {
        return std::pair<std::string, std::string>(str.substr(0,pos),str.substr(pos+1));
    }
}


}

