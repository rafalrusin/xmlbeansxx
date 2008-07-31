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


class WsClient;
typedef boost::shared_ptr<WsClient> WsClientPtr;

class WsClient
{
public:
	WsClient();
	HttpClientPtr httpClient;
	virtual xmlbeansxx::XmlObject invoke(Url url, xmlbeansxx::XmlObject request,const std::string &SOAPAction) = 0;
	virtual ~WsClient();
	static WsClientPtr create();
};

#endif /*WSCLIENT_H_*/
