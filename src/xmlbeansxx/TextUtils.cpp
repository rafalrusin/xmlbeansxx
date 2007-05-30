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


#include "TextUtils.h"
#include "Tracer.h"
#include "defs.h"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <string>


#include <libxml/entities.h>

#include <sstream>
#include <cstdio>


#define TRACER2(a,b) 
#define LOG4CXX_DEBUG2(a,b)


using namespace std;
using namespace boost::archive::iterators;

//XERCES_CPP_NAMESPACE_USE


namespace xmlbeansxx {

  LOGGER_PTR_SET(TextUtils::log,"xmlbeansxx.TextUtils");

  TextUtils::TextUtils() {}


  std::string TextUtils::intToString(int i) {
    ostringstream ss;
    ss<<i;
    return ss.str();
  }

  std::string TextUtils::floatToString(float f) {
    ostringstream ss;
    ss<<f;
    return ss.str();
  }

  std::string TextUtils::doubleToString(double d) {
    ostringstream ss;
    ss<<d;
    return ss.str();
  }

  std::string TextUtils::ptrToString(const void *ptr) {
    char buf[20];
    snprintf(buf,20,"%p",ptr);
    return string(buf);
  }

  std::string TextUtils::boolToString(bool b) {
    if (b==false) return "false";
    else return "true";
  }

  bool TextUtils::isWhite(char ch) {
    return ch=='\n' || ch==' ' || ch=='\t';
  }

  std::string TextUtils::collapse(const std::string &str) {
    TRACER2(log,"collapse");
    LOG4CXX_DEBUG2(log,"str:"+str);
    int l=str.size();
    int a=0,b=l-1;
    while (a<l && isWhite(str[a])) a++;
    while (b>=0 && isWhite(str[b])) b--;
    if (b<a) {
      LOG4CXX_DEBUG2(log,"ret:");
      return std::string();
    } else {
      LOG4CXX_DEBUG2(log,"ret:"+str.substr(a,b-a+1));
      return str.substr(a,b-a+1);
    }
  }

    std::string TextUtils::exchangeEntities(const std::string& str, TextUtils::EscapeFlags escapeFlag) {
    
    xmlChar *retu;
    if(escapeFlag == AttrEscapes)
		retu = xmlEncodeSpecialChars(NULL,(xmlChar*)str.c_str());
    else	retu = xmlEncodeEntitiesReentrant(NULL,(xmlChar*)str.c_str());
    	
    std::string s((char*)retu);
    xmlFree(retu);
    return s;    
/*        XMLCh* toFormat = XMLString::transcode (str.c_str ());
        
        MemBufFormatTarget target;
        XMLFormatter formatter (XMLUni::fgXMLChEncodingString, &target, escapeFlag);
        formatter << toFormat;
        
        XMLString::release(&toFormat);
            
        const XMLByte* bytes = target.getRawBuffer ();
        char * s = XMLString::transcode ((XMLCh*) bytes);
        string res(s);
        XMLString::release(&s);
        
        LOG4CXX_DEBUG(log,"res:" << res);
        return res; 
  */  }

  std::string TextUtils::exchangeEntitiesWithCDATA(const std::string& str) {
    return std::string("<![CDATA[") + str + std::string("]]>");
  }

  bool TextUtils::checkInteger(const std::string &num2) {
    if (num2.size()<1) return false;
    std::string num;
    if (num2[0] == '-' || num2[0] == '+')
        num = num2.substr(1);
    else
        num = num2;
    FOREACH(it,num) {
      if (!((*it)>='0' && (*it)<='9')) return false;
    }
    return true;
  }

  bool TextUtils::checkDecimal(const std::string &num2) {
    if (num2.size()<1) return false;
    std::string num;
    if (num2[0] == '-' || num2[0] == '+')
        num = num2.substr(1);
    else
        num = num2;

    int dots=0;
    FOREACH(it,num) {
      if (!((*it)>='0' && (*it)<='9' || (*it)=='.')) return false;
      if ((*it)=='.') {
        dots++;
        if (dots>1) return false;
      }
    }
    return true;
  }

  bool TextUtils::isDigit(char c) {
    return c>='0' && c<='9';
  }

  bool TextUtils::areDigits(const std::string &d) {
    bool r=true;
    FOREACH(it,d) r=r && TextUtils::isDigit(*it);
    return r;
  }

  bool TextUtils::checkDate(const std::string &date) {
    if (date.size()!=10) return false;
    return TextUtils::areDigits(date.substr(0,4))
      && TextUtils::areDigits(date.substr(5,2))
      && TextUtils::areDigits(date.substr(8,2))
      && date[4]=='-' && date[7]=='-';
  }

typedef base64_from_binary< transform_width<string::const_iterator, 6, 8> > base64_t;
typedef transform_width< binary_from_base64<string::const_iterator>, 8, 6 > binary_t;



  xmlbeansxx::shared_array<unsigned char> TextUtils::base64Decode(const std::string &what) {
    string dec(binary_t(what.begin()),binary_t(what.end()));
    unsigned int outLen=dec.length();
    xmlbeansxx::shared_array<unsigned char> a(outLen);
    FOR(i,int(outLen)) {
      a[i]=dec[i];
    }
    return a;
  }

  std::string TextUtils::base64Encode(xmlbeansxx::shared_array<unsigned char> what) {
    unsigned int outLen;
    string str((char*)what.get(),what.size());
    str.append((3-((str.length())%3)) % 3 ,'\0');
    string enc(base64_t(str.begin()), base64_t(str.end()));
    return enc;
  }

}
