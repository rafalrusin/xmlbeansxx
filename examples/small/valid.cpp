#include "c.h"
#include <iostream>
#include <fstream>
#include <xmlbeansxx/LibXMLParser.h>
//#include <log4cxx/basicconfigurator.h>

using namespace std;
using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;

int main() {
    //Xsd validation
//      log4cxx::BasicConfigurator::configure();
    try {
        XmlOptions opt;
    	opt.setValidation(true);
    	LibXMLParser p(opt);

        ContentDocument doc=ContentDocument::Factory::newInstance();
        p.loadGrammar("c.xsd");
        p.parse(
	cin

/*
"<?xml version='1.0' encoding='UTF-8'?> "
"<content xmlns='http://mind.p4.com/mytest' xmlns:xs='http://www.w3.org/2001/XMLSchema-instance'> "
"<employee> "
"  <firstname> "
"      name "
"  </firstname> "
"  <lastname> "
"       namel        "
"  </lastname>  "
"</employee>   "
"</content>"
*/
	,doc);
        doc.serialize(cout);

    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    } catch (xmlbeansxx::XmlParseException* ex) {
    	cout << ex->getMessage() <<"\n";
    }
    return 0;
}
