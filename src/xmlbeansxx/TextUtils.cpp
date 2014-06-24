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


#include "TextUtils.h"
#include "Tracer.h"
#include "defs.h"
#include "SchemaType.h"


#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <string>
#include <iomanip>


#include "config.h"

#ifdef XMLBEANSXX_WITH_LIBXML2
#include <libxml/entities.h>
#endif

#include <sstream>
#include <cstdio>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>

#define TRACER2(a,b)
#define XMLBEANSXX_DEBUG2(a,b)


using namespace std;
using namespace boost::archive::iterators;

XERCES_CPP_NAMESPACE_USE




namespace xmlbeansxx {


static int precision = XMLBEANSXX_TEXTUTILS_DEFAULT_PRECISION;


  XMLBEANSXX_LOGGER_PTR_SET(TextUtils::log,"xmlbeansxx.TextUtils");

  TextUtils::TextUtils() {}

  int TextUtils::getPrecision(){
  	return precision;
  }
  void TextUtils::setPrecision(int p){
  	precision = p;
#ifdef XMLBEANSXX_WITH_GMPXX
	mpf_set_default_prec(8*p);
#endif
  }


namespace {
template<class T>
std::string decimalToString(T f,int _p) {
    ostringstream ss;
    int p=TextUtils::getPrecision();
    if(_p>=0) {
    	p = _p;
    	ss << std::fixed;
    }
    ss << std::setprecision(p) << f;
    return ss.str();
}
}


  std::string TextUtils::intToString(int i) {
    ostringstream ss;
    ss << i;
    return ss.str();
  }

  std::string TextUtils::mpzToString(const mpz_class & i) {
    ostringstream ss;
    ss << i;
    return ss.str();
  }

  std::string TextUtils::floatToString(float f) {
  	return TextUtils::floatToString(f,-1);
  }
  std::string TextUtils::doubleToString(double d) {
  	return TextUtils::doubleToString(d,-1);
  }
  std::string TextUtils::longDoubleToString(long double d) {
  	return TextUtils::longDoubleToString(d,-1);
  }
  std::string TextUtils::mpfToString(const mpf_class & d) {
  	return TextUtils::mpfToString(d,-1);
  }
  std::string TextUtils::floatToString(float d,int _p) {
  	return decimalToString<float>(d,_p);
  }

  std::string TextUtils::doubleToString(double d,int _p) {
  	return decimalToString<double>(d,_p);
  }

  std::string TextUtils::longDoubleToString(long double d,int _p) {
  	return decimalToString<long double>(d,_p);
  }

  std::string TextUtils::mpfToString(const mpf_class & d,int _p) {
  	return decimalToString<const mpf_class &>(d,_p);
  }


  std::string TextUtils::ptrToString(const void *ptr) {
    char buf[100];
    sprintf(buf,"%p",ptr);
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
    XMLBEANSXX_DEBUG2(log,std::string("str:")+str);
    int l=str.size();
    int a=0,b=l-1;
    while (a<l && isWhite(str[a])) a++;
    while (b>=0 && isWhite(str[b])) b--;
    if (a>=l || b<a) {
      XMLBEANSXX_DEBUG2(log,"ret:");
      return std::string("");
    } else {
      XMLBEANSXX_DEBUG2(log,std::string("ret:")+str.substr(a,b-a+1));
      return str.substr(a,b-a+1);
    }
  }

    std::string TextUtils::exchangeEntities(const std::string& str, TextUtils::EscapeFlags escapeFlag) {

#ifndef XMLBEANSXX_WITH_LIBXERCES_C

    xmlChar *retu;
    if(str.size()<=0) return std::string("");
    if(escapeFlag == AttrEscapes)
		retu = xmlEncodeSpecialChars(NULL,(xmlChar*)str.c_str());
    else	retu = xmlEncodeEntitiesReentrant(NULL,(xmlChar*)str.c_str());

    std::string s((char*)retu);
    xmlFree(retu);
    return s;

#else

        XMLCh* toFormat = XMLString::transcode (str.c_str ());

        MemBufFormatTarget target;
	XMLFormatter::EscapeFlags flags;
	if(escapeFlag == AttrEscapes) {
		flags = XMLFormatter::AttrEscapes;
	} else {
		flags = XMLFormatter::CharEscapes;
	}
        XMLFormatter formatter (XMLUni::fgXMLChEncodingString, &target, flags);
        formatter << toFormat;

        XMLString::release(&toFormat);

        const XMLByte* bytes = target.getRawBuffer ();
        char * s = XMLString::transcode ((XMLCh*) bytes);
        string res(s);
        XMLString::release(&s);

        return res;
#endif
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
    XMLBEANSXX_FOREACH(std::string::iterator,it,num) {
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
    XMLBEANSXX_FOREACH(std::string::iterator,it,num) {
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
    XMLBEANSXX_FOREACH(std::string::const_iterator,it,d) r=r && TextUtils::isDigit(*it);
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

bool _inBase64(char c){
	return 	(c>='0' && c<='9') ||
		(c>='A' && c<='Z') ||
		(c>='a' && c<='z') ||
		 c=='+' ||
		 c=='/' ||
		 c=='=' ;
};

std::string _toCanolicalBase64(const std::string &s) {
	std::string retu;
	for(unsigned i=0;i<s.size();i++) {
		if(_inBase64(s[i]))
			retu += s[i];
	}
	return retu;
}

xmlbeansxx::shared_array<unsigned char> TextUtils::base64Decode(const std::string &what2) {

    std::string what = _toCanolicalBase64(what2);
    int size = what.size();
    int pads = 0;
    if(size>2){
	for(int i=0;i<2;i++)
		if(what[--size] == '=') {
			pads++;
			what[size]='0';
		}
    }

    string dec(binary_t(what.begin()),binary_t(what.end()));
    unsigned int outLen=dec.length();
    outLen -= pads;
    xmlbeansxx::shared_array<unsigned char> a(outLen);
    for(unsigned int i=0;i<outLen;i++) {
      a[i]=dec[i];
    }
    return a;
  }

  std::string TextUtils::base64Encode(xmlbeansxx::shared_array<unsigned char> what) {
    string str((char*)what.get(),what.size());
    int pads = (3-((str.length())%3)) % 3;
    str.append(pads ,'\0');
    string enc(base64_t(str.begin()), base64_t(str.end()));
    for(int i=1;i<=pads;i++){
    	enc[enc.size()-i]='=';
    }
    return enc;
  }

std::string TextUtils::applyContentTypeRules(const std::string & value,const SchemaType * st) {
	SchemaType::CONTENT_TYPE ct=st->getContentType();
    	if (ct==SchemaType::SIMPLE_CONTENT || ct==SchemaType::MIXED_CONTENT || ct==SchemaType::NOT_COMPLEX_TYPE) {
	        if (st->whitespaceRule==SchemaType::WS_COLLAPSE) {
        	    return TextUtils::collapse(value);
	        } else {
        	    return value;
	        }
	}
	return std::string();
}





}
