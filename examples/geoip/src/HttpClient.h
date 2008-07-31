#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include "Exception.h"

/**
 * Wyjątek wyrzucany w trakcie komunikacji z wykorzystaniem interfejsu HttpClient.
 */
class HttpClientException: public Exception {
public:
	HttpClientException(const std::string& msg): Exception(msg) {}
};

class HttpClient;
typedef boost::shared_ptr<HttpClient> HttpClientPtr;
typedef std::vector<std::string> headers_type;

/**
 * Klasa przechowująca podstawowe dane do połączenia: URL, login i hasło.
 */
class Url {
public:
	std::string url;
	std::string user;
	std::string password;
	
	Url() {}
	Url(std::string url, std::string user, std::string password): url(url), user(user), password(password) {}
};

/** Obsługa zapytań http. Implementacja używa biblioteki cURL */
class HttpClient
{
public:
	boost::mutex crit;
	std::string cacert;
	
	HttpClient();
	virtual ~HttpClient();
	
        /** Wykonanie metody HTTP POST */
	virtual std::string post(const Url &url, std::string in, const headers_type &headers = headers_type()) = 0;
	
	static HttpClientPtr create();
};

#endif /*HTTPCLIENT_H_*/
