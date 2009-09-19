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
  
#include <geoip_xsd.h>
#include <WsClient.h>
#include <iostream>
#include <log4cxx/stream.h>


using namespace std;
using namespace net::webservicex;
using namespace xmlbeansxx;

log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("geoip");

int main(int argc, char *argv[]) {
    if (argc != 2) {
        LOG4CXX_INFO(logger, "Usage: "<<argv[0]<<" ip_address");
        return 1;
    }

	// create an instance of wsClient and httpClient
    WsClientPtr wsClient(WsClient::create());
    HttpClientPtr httpClient(HttpClient::create());
    wsClient->httpClient = httpClient;
    
    
       // create a request: GetGeoIPDocument
    GetGeoIPDocument getGeoIP = GetGeoIPDocument::Factory::newInstance();
    getGeoIP.cgetGetGeoIP().setIPAddress(argv[1]);  // this is the google.com ip

       // invoke the request
    LOG4CXX_INFO(logger, "Please wait...");
    XmlObject response1 = wsClient->invoke(Url("http://www.webservicex.net/geoipservice.asmx", "", ""), getGeoIP, "http://www.webservicex.net/GetGeoIP");
    
       // convert the response to GetGeoIPResponseDocument 
       // (it should throw if the response is not of type GetGeoIPResponseDocument)
    GetGeoIPResponseDocument response(response1);
    
       // print out some data
    GeoIP geoIPResult = response.getGetGeoIPResponse().getGetGeoIPResult();
    LOG4CXX_INFO(logger, "ip address: "<<argv[1]);
    LOG4CXX_INFO(logger, "country name: "<<geoIPResult.getCountryName()); 
    LOG4CXX_INFO(logger, "country code: "<<geoIPResult.getCountryCode());
    return 0;
}
