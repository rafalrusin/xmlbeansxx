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
#include "WsClient.h"
#include <soapEnvelope.h>
#include <sstream>
#include <fstream>
#include <xmlbeansxx/XmlCursor.h>
#include <iostream>
#include <log4cxx/logger.h>
using namespace xmlbeansxx;
using namespace org::xmlsoap::schemas::soap::envelope;
using namespace std;

class WsClientImpl: public WsClient {
    static log4cxx::LoggerPtr log;
    
    virtual xmlbeansxx::XmlObject invoke(Url url, XmlObjectDocument request,const std::string &SOAPAction) {

        std::vector<std::string> headers(2);
        std::string SOAPActionHeader="SOAPAction: \"" + SOAPAction + "\"";
        headers[0]=SOAPActionHeader;
        headers[1]="Content-Type: text/xml;charset=UTF-8";

	EnvelopeDocument doc = EnvelopeDocument::Factory::newInstance();
	Body body = doc.cgetEnvelope().cgetBody();
	
	try {
		body.newCursor().insertDocument(request);
	} catch (...) {
                throw WsClientException("WsClientException: request is empty.");
	}

        string responseString;
        LOG4CXX_INFO(log,string("sending request String: ") + doc.toString() + " to url " + url.url );
        responseString = httpClient->post(url, doc.toString() ,headers);
        
        LOG4CXX_INFO(log,string("response String: ") + responseString);
        try {
    		EnvelopeDocument outDocument(EnvelopeDocument::Factory::parse(responseString));
            try {  //test for soap::fault answer 
                FaultDocument fault(outDocument.getEnvelope().getBody());
                LOG4CXX_ERROR(log,"SOAP fault answer:" + fault.toString());
                std::string opis;
                try{
                    opis = fault.getFault().getFaultstring();
                } catch(...){ }
                throw SoapFaultException("SoapFaultException: " + opis,fault);
            } catch(xmlbeansxx::BeansException &e){}  //soap::fault not recognised
            
            return outDocument.getEnvelope().getBody();
            
        } catch(xmlbeansxx::BeansException &e) { //not a soap answer
            LOG4CXX_ERROR(log,"error in response parsing.");
            throw HttpClientException("HttpClientException: 'Got a not correct SOAP response'.");
        } 
    }
};

log4cxx::LoggerPtr WsClientImpl::log = log4cxx::Logger::getLogger("WsClient");
WsClient::WsClient()
{
}

WsClient::~WsClient()
{
}

WsClientPtr WsClient::create() {
	return WsClientPtr(new WsClientImpl());
}
