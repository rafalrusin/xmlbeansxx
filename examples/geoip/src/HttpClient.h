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
