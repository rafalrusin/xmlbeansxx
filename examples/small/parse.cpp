#include <xmlbeansxx/xmlbeansxx.h>
#include <iostream>
using namespace std;
using namespace xmlbeansxx;

int main() {
    //not typed document parsing
    try {
		string str=XmlInteger::Factory::newInstance(10213)->toString();
		cout<<"str:\n"<<str<<"\n";
        XmlIntegerPtr i=XmlInteger::Factory::parse(str);
        i=XmlInteger::Factory::newInstance((*i)+(*XmlInteger::Factory::newInstance(1)));
		cout<<"after add:"<<i->getSimpleContent()<<"\n";
        i->serialize(cout);
    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
