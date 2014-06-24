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
#include "HttpClient.h"

#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <log4cxx/logger.h>

using namespace boost;
using namespace std;

namespace {

size_t curl_stream_result(bool result, size_t requestedLength) {
	if (requestedLength == 0) {
		return !result;
	} else {
		return result ? requestedLength : 0;
	}
}

size_t my_write_func(void *ptr, size_t size, size_t nmemb, std::ostream *stream)
{
	stream->write((char *) ptr, size*nmemb);
	return curl_stream_result(!stream->bad(), size*nmemb);
}

size_t my_read_func(void *ptr, size_t size, size_t nmemb, std::istream *stream)
{
	if ((size * nmemb) == 0)
		return 0;

	stream->read((char *) ptr, size * nmemb);
	int count = stream->gcount();
	return count;
}

}


HttpClient::HttpClient()
{
}

HttpClient::~HttpClient()
{
}

class CurlHttpClient: public HttpClient {
    static log4cxx::LoggerPtr log;
public:


	CurlHttpClient() {
	}

	virtual ~CurlHttpClient() {
	}

	virtual std::string post(const Url &url, std::string _in,const headers_type &headers) {
		ostringstream out;
		istringstream in(_in);
        int inSize = _in.length();

		string ca;
		{
			ca = cacert;
		}




		CURL *curl;
        struct curl_slist *slist=NULL;
		char curl_errbuf[CURL_ERROR_SIZE];
		curl = curl_easy_init();

        for(headers_type::size_type i=0;i<headers.size();i++) {
            slist = curl_slist_append(slist, headers[i].c_str());
        }


        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

		curl_easy_setopt(curl, CURLOPT_URL, url.url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0);
		curl_easy_setopt(curl, CURLOPT_CAINFO, ca.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
		curl_easy_setopt(curl, CURLOPT_READDATA, &in);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
	    curl_easy_setopt(curl, CURLOPT_READFUNCTION, my_read_func);

#ifdef DEBUG
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#else
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
#endif
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, inSize);
	    string userpassword = url.user + ":" + url.password;
	    if (url.user != "") {
		    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		    curl_easy_setopt(curl, CURLOPT_USERPWD, userpassword.c_str());
	    }

	    CURLcode result = curl_easy_perform(curl);

         curl_slist_free_all(slist); /* free the list again */

        std::ostringstream s_result;
        s_result << result;
        LOG4CXX_INFO(log,"result: " +s_result.str() );
	    curl_easy_cleanup(curl);
		if (result != CURLE_OK) {
            LOG4CXX_ERROR(log,"HttpClientException: " +string(curl_errbuf) );
            std::string error = "HTTP communication error.";
            if(string(curl_errbuf).size() > 2) error += "\nDescription: " + string(curl_errbuf);
			throw HttpClientException(error);
		}
        return out.str();
	}
};

log4cxx::LoggerPtr CurlHttpClient::log = log4cxx::Logger::getLogger("CurlHttpClient");

HttpClientPtr HttpClient::create() {
	return HttpClientPtr(new CurlHttpClient());
}

