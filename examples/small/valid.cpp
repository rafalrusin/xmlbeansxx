#include "c.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;

int main() {
    //Xsd validation
    try {
        XmlParserPtr p(XmlParser::create());
        ContentDocumentPtr doc(ContentDocument::Factory::newInstance());
        p->getXmlOptions()->setValidation(true);
        p->loadGrammar("c.xsd");
        p->parse(cin,doc.get());
        doc->serialize(cout);
		
    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
