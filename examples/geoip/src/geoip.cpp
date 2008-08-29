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


using namespace std;
using namespace net::webservicex;
using namespace xmlbeansxx;

int main(int argc, char *argv[]) {

    if (argc != 2) {
      cerr<<"Usage: "<<argv[0]<<" ip_address"<<endl;
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
    cout<<"Please wait..."<<endl;
    XmlObject response1 = wsClient->invoke(Url("http://www.webservicex.net/geoipservice.asmx", "", ""), getGeoIP, "http://www.webservicex.net/GetGeoIP");
    
       // convert the response to GetGeoIPResponseDocument 
       // (it should throw if the response is not of type GetGeoIPResponseDocument)
    GetGeoIPResponseDocument response(response1);
    
       // print out some data
    GeoIP geoIPResult = response.getGetGeoIPResponse().getGetGeoIPResult();
    cout<<"ip address: "<<argv[1]<<endl;
    cout<<"country name: "<<geoIPResult.getCountryName()<<endl; 
    cout<<"country code: "<<geoIPResult.getCountryCode()<<endl;
    return 0;
}
