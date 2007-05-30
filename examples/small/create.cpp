#include "c.h"
#include <iostream>

using namespace std;

using namespace com::p4::mind::mytest;
using namespace xmlbeansxx;

int main() {
    try {
        //xml tree creation using generated classes from c.xsd
        ContentDocument root=ContentDocument::Factory::newInstance();
        ContentType ct=ContentType::Factory::newInstance();
        root.setContent(ct);

        Fullpersoninfo p=Fullpersoninfo::Factory::newInstance();
        ct.setEmployee(p);
        p.setCity(77);
        p.xsetAddress(XmlString("_addr_"));
        p.xsetLastname(XmlString("_ln<>_"));
        p.xsetFirstname(XmlString("_fn_"));
        p.setDt("\n\n 2004-01-30T22:50:11  ");
        XmlArray<XmlString> tab=p.dgetTableArray();
        tab.cgetArray(0);
        tab.cgetArray(1);
		p.dsetTableArray(tab);
		p.addNewTable();
  
        p.xsetCar(CarType("2"));
        p.setCar2(CarType("3").getSimpleContent());

        p.xsetAge(XmlInt(20));
        p.xsetMoney(AmountType("10.256789"));

        root.serialize(cout);
	cout << "\n";

	Personinfo p2 = Personinfo::Factory::newInstance();
	
        p2.xsetLastname(XmlString("Pawel"));
        p2.xsetFirstname(XmlString("Stawicki"));

        ct.setEmployee(p2);
        root.serialize(cout);
	cout << "\n";

	cout << p.toString() << "\n";
//		cout<<root.toString();

        //cout<<"tab:";
        //tab.serialize(cout);

    } catch (BeansException ex) {
        cout<<"Exception: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
