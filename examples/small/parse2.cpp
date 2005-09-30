#include "c.h"
#include <iostream>
#include <fstream>
#include <log4cxx/logger.h>
#include <xmlbeansxx/XmlParser.h>

using namespace std;
using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;

int main() {
    //parsing with validation
    try {
        XmlParser parser(XmlParser::create());
        fstream in("d.xml", ios::in);
        ContentDocument root(ContentDocument::Factory::newInstance());
        parser->getXmlOptions()->setValidation(true);
        parser->loadGrammar("c.xsd");
        parser->parse(in, root);
        root->save(cout);
    } catch (BeansException ex) {
        cout << "BeansException: " << ex.getMessage() << endl;
    }
    return 0;
}
