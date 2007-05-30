#include <string>

#include <xmlbeansxx/logger.h>
#include <fstream>
#include <sstream>

void namespaceTests();

LOGGER_PTR_SET(logger,"mem03");

//using namespace xmlbeansxx;
using namespace std;

/*
double currentTime() {
    struct timeval tv;
    if (gettimeofday(&tv,NULL)!=0) throw IllegalStateException();
    return double(tv.tv_sec) + double(tv.tv_usec)/1000000.0;
}*/


int main()
{
/*	XmlString a;
	string s=
		"<d:xml-fragment xmlns:d='http://xmlbeans.apache.org/definitions'"
		" xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'"
		" xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
			"<d:element xsi:type='xs:string'> Ola ma kota  </d:element>"
			"</d:xml-fragment>";
	a=XmlString::Factory::parse(s);
*/
	do{
	LOG4CXX_DEBUG(logger,"mem leak Test:");		
	} while(true);
}
