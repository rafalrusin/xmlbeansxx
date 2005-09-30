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

#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include <sstream>
#include <cstdio>
#include "CoreTypes.h"


#define TRACER2(a,b) 
#define LOG4CXX_DEBUG2(a,b)


using namespace std;
XERCES_CPP_NAMESPACE_USE


namespace xmlbeansxx {

  log4cxx::LoggerPtr TextUtils::log = log4cxx::Logger::getLogger(String("xmlbeansxx.TextUtils"));

  TextUtils::TextUtils() {}


  String TextUtils::intToString(int i) {
    ostringstream ss;
    ss<<i;
    return ss.str();
  }

  String TextUtils::floatToString(float f) {
    ostringstream ss;
    ss<<f;
    return ss.str();
  }

  String TextUtils::doubleToString(double d) {
    ostringstream ss;
    ss<<d;
    return ss.str();
  }

  String TextUtils::ptrToString(const void *ptr) {
    char buf[20];
    snprintf(buf,20,"%p",ptr);
    return string(buf);
  }

  String TextUtils::boolToString(bool b) {
    if (b==false) return "false";
    else return "true";
  }

  bool TextUtils::isWhite(char ch) {
    return ch=='\n' || ch==' ' || ch=='\t';
  }

  String TextUtils::collapse(const String &str) {
    TRACER2(log,"collapse")
    LOG4CXX_DEBUG2(log,"str:"+str);
    int l=str.size();
    int a=0,b=l-1;
    while (a<l && isWhite(str[a])) a++;
    while (b>=0 && isWhite(str[b])) b--;
    if (b<a) {
      LOG4CXX_DEBUG2(log,"ret:");
      return String();
    } else {
      LOG4CXX_DEBUG2(log,"ret:"+str.substr(a,b-a+1));
      return str.substr(a,b-a+1);
    }
  }

  String TextUtils::exchangeEntities(const String& str) {
    std::ostringstream o;
    for ( unsigned int i = 0; i < str.size(); i++ ) {
      switch (str[i]) {
      case '\'' :
        o << "&apos;";
        break;
      case '<' :
        o << "&lt;";
        break;
      case '>' :
        o << "&gt;";
        break;
      case '&' :
        o << "&amp;";
        break;
      case '"' :
        o << "&quot;";
        break;
      default :
        o << str[i];
        break;
      }
    }
    return o.str();
  }

  String TextUtils::exchangeEntitiesWithCDATA(const String& str) {
    return String("<![CDATA[") + str + String("]]>");
  }

  bool TextUtils::checkInteger(const String &num2) {
    if (num2.size()<1) return false;
    String num;
    if (num2[0] == '-' || num2[0] == '+')
        num = num2.substr(1);
    else
        num = num2;
    FOREACH(it,num) {
      if (!((*it)>='0' && (*it)<='9')) return false;
    }
    return true;
  }

  bool TextUtils::checkDecimal(const String &num2) {
    if (num2.size()<1) return false;
    String num;
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

  bool TextUtils::areDigits(const String &d) {
    bool r=true;
    FOREACH(it,d) r=r && TextUtils::isDigit(*it);
    return r;
  }

  bool TextUtils::checkDate(const String &date) {
    if (date.size()!=10) return false;
    return TextUtils::areDigits(date.substr(0,4))
      && TextUtils::areDigits(date.substr(5,2))
      && TextUtils::areDigits(date.substr(8,2))
      && date[4]=='-' && date[7]=='-';
  }

  xmlbeansxx::Array<unsigned char> TextUtils::base64Decode(const String &what) {
    unsigned int outLen;
    XMLByte *buf=Base64::decode((XMLByte *)what.c_str(),&outLen);
    xmlbeansxx::Array<unsigned char> a(outLen);
    FOR(i,int(outLen)) {
      a[i]=buf[i];
    }
    XMLString::release(&buf);
    return a;
  }

  String TextUtils::base64Encode(Array<byte> what) {
    unsigned int outLen;
    byte *buf0 = new byte[what.size()];
    FOR(i, what.size()) {
        buf0[i] = what[i];
    }
    XMLByte *buf=Base64::encode(buf0, what.size(), &outLen);
    String s;
    FOR(i,int(outLen)) {
      s+=buf[i];
    }
    delete buf0;
    XMLString::release(&buf);
    return s;
  }


  String TextUtils::istreamToString(std::istream &in) {
    String s,s2;
    while (true) {
        in>>s2;
        if (in.eof())
            break;
        s+=s2;
    }
    return s;
}
}
