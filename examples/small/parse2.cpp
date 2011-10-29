#include "c.h"
#include <iostream>
#include <fstream>
#include <xmlbeansxx/LibXMLParser.h>

using namespace std;
using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;

int main() {
    //log4cxx::BasicConfigurator::configure();
    //typed document parsing (using classes generated from c.xsd)
    try {
        LibXMLParser p;
        //boost::shared_ptr<ContentDocument> root(new ContentDocument());
        fstream in("c.xml",ios::in);
        ContentDocument root=ContentDocument::Factory::parse(in);
        cout<<"firstname:"<<root.getContent().getEmployee().getFirstname()<<"\n";
        cout<<"lastname:"<<root.getContent().getEmployee().getLastname()<<"\n";
        cout<<"age:"<<root.getContent().getEmployee().getAge()<<"\n";
        cout<<"firstname as xml:\n"<<root.getContent().getEmployee().xgetFirstname().toString()<<"\n";
	cout<<"serializing:\n";
        root.serialize(cout);
	cout<<"\n";
    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
