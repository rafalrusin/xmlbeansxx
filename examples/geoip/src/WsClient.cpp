#include "WsClient.h"
#include <soapEnvelope.h>
#include <sstream>
#include <fstream>
#include <xmlbeansxx/XmlCursor.h>
using namespace xmlbeansxx;
using namespace org::xmlsoap::schemas::soap::envelope;
using namespace std;

class WsClientImpl: public WsClient {
    static log4cxx::LoggerPtr log;
    
	virtual xmlbeansxx::XmlObject invoke(Url url, XmlObject request,const std::string &SOAPAction) {
        std::string SOAPActionHeader="SOAPAction: \"" + SOAPAction + "\"";
        std::vector<std::string> headers(2);
        
        headers[0]=SOAPActionHeader;
        headers[1]="Content-Type: text/xml;charset=UTF-8";

		EnvelopeDocument doc = EnvelopeDocument::Factory::newInstance();
		Body body = doc.cgetEnvelope().cgetBody();
		Contents::Walker::ElemsType e= Contents::Walker::getElems(request);
		Contents::Walker::appendElem(body,e[0].first,e[0].second);

        string responseString;
        responseString = httpClient->post(url, doc.toString() ,headers);
        
        LOG4CXX_INFO(log,"response String: " + responseString);
        try {
    		EnvelopeDocument outDocument(EnvelopeDocument::Factory::parse(responseString));
            try {  //test for soap::fault answer
                FaultDocument fault(outDocument.getEnvelope().getBody());
                LOG4CXX_ERROR(log,"SOAP fault answer:" + fault.toString());
                std::string opis;
                try{
                    opis = fault.getFault().getFaultstring();
                } catch(...){
                ;
                }
                throw SoapFaultException("Soap fault.\nOpis: " + opis,fault);
            } catch(xmlbeansxx::BeansException &e){}  //soap::fault not recognised
            
            return outDocument.getEnvelope().getBody();
            
        } catch(xmlbeansxx::BeansException &e) { //not a soap answer
            LOG4CXX_ERROR(log,"error in response parsing.");
            throw HttpClientException("HTTP Error. 'Got a not correct SOAP response'.");
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
