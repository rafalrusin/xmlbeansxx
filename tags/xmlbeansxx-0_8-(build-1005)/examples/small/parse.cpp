#include <xmlbeansxx/xmlbeansxx.h>
#include <iostream>
using namespace std;
using namespace xmlbeansxx;

int main() {
    //not typed document parsing
    try {
    
	string str= XmlInteger(10213).toString();
	cout << "str:\n" << str << "\n";
    	XmlInteger i=XmlInteger::Factory::parse(str);
	cout << "pased:\n" << i.toString() << "\n";
	
        i=XmlInteger(i+XmlInteger(1));
	cout << "after add:" << i.getSimpleContent() << "\n";
        i.serialize(cout);

        /*
        boost::shared_ptr<XmlObject> root(new XmlAnyDocument());
        boost::shared_ptr<XmlOptions> opts(new XmlOptions());
        opts->setTypedParsing(false);
        root->parse(cin,opts);
        root->serialize(cout);
        */
    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
