#include <xmlbeansxx/xmlbeansxx.h>
#include <iostream>
using namespace std;
using namespace xmlbeansxx;

int main() {
    //document parsing
    try {
		string str=newInstance<XmlInteger>(BigInteger(10213))->toString();
		cout << "document to parse:" << endl << str << endl;
        XmlInteger i = XmlInteger::Factory::parse(str);
		cout << "value: " << i->getBigIntegerValue() << endl;
    } catch (BeansException ex) {
        cout << "BeansException: " << ex.getMessage() << endl;
    }
    return 0;
}
