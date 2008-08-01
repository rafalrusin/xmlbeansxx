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
#ifndef WSCLIENT_H_
#define WSCLIENT_H_

#include "HttpClient.h"
#include <xmlbeansxx/xmlbeansxx.h>
#include <soapEnvelope.h>

class SoapFaultException: public Exception {
public:
    SoapFaultException(const std::string& msg,org::xmlsoap::schemas::soap::envelope::FaultDocument & fault) : Exception(msg), fault(fault) {};
    virtual ~SoapFaultException() throw() {}
    org::xmlsoap::schemas::soap::envelope::FaultDocument fault;
};

class WsClientException: public Exception {
public:
	WsClientException(const std::string& msg): Exception(msg) {}
};


class WsClient;
typedef boost::shared_ptr<WsClient> WsClientPtr;

class WsClient
{
public:
	WsClient();
	HttpClientPtr httpClient;
	virtual xmlbeansxx::XmlObject invoke(Url url, xmlbeansxx::XmlObjectDocument request,const std::string &SOAPAction) = 0;
	virtual ~WsClient();
	static WsClientPtr create();
};

#endif /*WSCLIENT_H_*/
