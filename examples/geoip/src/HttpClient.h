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
    limitations under the License. 
  */
#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include "Exception.h"

class HttpClientException: public Exception {
public:
	HttpClientException(const std::string& msg): Exception(msg) {}
};

class HttpClient;
typedef boost::shared_ptr<HttpClient> HttpClientPtr;
typedef std::vector<std::string> headers_type;

class Url {
public:
	std::string url;
	std::string user;
	std::string password;
	
	Url() {}
	Url(std::string url, std::string user, std::string password): url(url), user(user), password(password) {}
};

class HttpClient
{
public:
	boost::mutex crit;
	std::string cacert;
	
	HttpClient();
	virtual ~HttpClient();
	
	virtual std::string post(const Url &url, std::string in, const headers_type &headers = headers_type()) = 0;
	
	static HttpClientPtr create();
};

#endif /*HTTPCLIENT_H_*/
