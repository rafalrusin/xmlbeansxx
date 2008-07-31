#include <geoip_xsd.h>
#include <WsClient.h>
#include <iostream>
using namespace std;
using namespace net::webservicex;
using namespace xmlbeansxx;
int main() {
    WsClientPtr wsClient(WsClient::create());
    HttpClientPtr httpClient(HttpClient::create());
    wsClient->httpClient = httpClient;

    GetGeoIPDocument getGeoIP = GetGeoIPDocument::Factory::newInstance();
    getGeoIP.cgetGetGeoIP().setIPAddress("64.233.167.99");

    XmlObject response1 = wsClient->invoke(Url("http://www.webservicex.net/geoipservice.asmx", "", ""), getGeoIP, "http://www.webservicex.net/GetGeoIP");
    GetGeoIPResponseDocument response(response1);
    GeoIP geoIPResult = response.getGetGeoIPResponse().getGetGeoIPResult();
    cout<<"country name: "<<geoIPResult.getCountryName()<<endl; 
    cout<<"country code: "<<geoIPResult.getCountryCode()<<endl;
    return 0;
}
