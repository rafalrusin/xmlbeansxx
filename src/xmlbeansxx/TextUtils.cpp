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


#define TRACER2(a,b) 
#define LOG4CXX_DEBUG2(a,b)


using namespace std;
XERCES_CPP_NAMESPACE_USE


namespace xmlbeansxx {

  log4cxx::LoggerPtr TextUtils::log = log4cxx::Logger::getLogger(std::string("xmlbeansxx.TextUtils"));

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
    TRACER2(log,"collapse")
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

  std::string TextUtils::exchangeEntities(const std::string& str) {
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

  xmlbeansxx::shared_array<unsigned char> TextUtils::base64Decode(const std::string &what) {
    unsigned int outLen;
    XMLByte *buf=Base64::decode((XMLByte *)what.c_str(),&outLen);
    xmlbeansxx::shared_array<unsigned char> a(outLen);
    FOR(i,int(outLen)) {
      a[i]=buf[i];
    }
    XMLString::release(&buf);
    return a;
  }

  std::string TextUtils::base64Encode(xmlbeansxx::shared_array<unsigned char> what) {
    unsigned int outLen;
    XMLByte *buf=Base64::encode(what.get(),what.size(),&outLen);
    std::string s;
    FOR(i,int(outLen)) {
      s+=buf[i];
    }
    XMLString::release(&buf);
    return s;
  }

}
