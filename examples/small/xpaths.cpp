#include <xmlbeansxx/xmlbeansxx.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace xmlbeansxx;

int main() {
    try {
		/*
        XmlParser p;
        fstream in("c.xml",ios::in);
        boost::shared_ptr<XmlOptions> opts(new XmlOptions);
        opts->setTypedParsing(false);
        boost::shared_ptr<XmlObject> root(new XmlAnyDocument());
        root->parse(in,opts);

        //geum-like xpath:
        //cout<<"address:"<<root->getXpathValue("/employee/address")<<"\n";

        root->serialize(cout);
		*/
    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
