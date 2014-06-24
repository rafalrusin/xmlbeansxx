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


#ifndef _XMLBEANSXX_CORE_TYPES_H_
#define _XMLBEANSXX_CORE_TYPES_H_


#include "config.h"

#include "BoostAssert.h"
#include <boost/shared_array.hpp>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

#ifdef XMLBEANSXX_WITH_GMPXX
#include <gmp.h>
#include <gmpxx.h>
#endif

#include <sstream>

#include "QName.h"


namespace xmlbeansxx {

#ifdef XMLBEANSXX_WITH_GMPXX
    typedef ::mpz_class mpz_class;
    typedef ::mpf_class mpf_class;

    inline bool mpzFitsToInt(const mpz_class &v) {
    	return v.fits_sint_p();
    }
    inline int mpzToInt(const mpz_class &v){
    	return v.get_si();
    }
    inline mpz_class stringToMpz(const std::string &v) {
    	mpz_class r(v);
//    	std::stringstream ss(v);
//	ss >> r;
	return r;
    }
    inline mpf_class stringToMpf(const std::string &v) {
    	mpf_class r(v);
//    	std::stringstream ss(v);
//	ss >> r;
	return r;
    }
//XMLBEANSXX_WITH_GMPXX
#else
// dont use gmpxx

    typedef long long mpz_class;
    typedef long double mpf_class;

    inline bool mpzFitsToInt(const mpz_class &v) {
    	return true;
    }
    inline int mpzToInt(const mpz_class &v){
    	return v;
    }
    inline mpz_class stringToMpz(const std::string &v) {
    	mpz_class r;
    	std::stringstream ss(v);
	ss >> r;
	return r;
    }
    inline mpf_class stringToMpf(const std::string &v) {
    	mpf_class r;
    	std::stringstream ss(v);
	ss >> r;
	return r;
    }
#endif //XMLBEANSXX_WITH_GMPXX


    typedef boost::shared_ptr<QName> QNamePtr;

    class GDate {
    };
    typedef boost::shared_ptr<GDate> GDatePtr;

    class List {
    };
    typedef boost::shared_ptr<List> ListPtr;

    class Object {
        public:
	virtual ~Object(){}
        virtual std::string toString() const { return ""; }
    };
    typedef boost::shared_ptr<Object> ObjectPtr;


}

#endif //_XMLBEANSXX_CORE_TYPES_H_


