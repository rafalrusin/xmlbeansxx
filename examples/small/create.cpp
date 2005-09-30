#include "c.h"
#include <iostream>

using namespace std;

using namespace com::p4::mind::mytest;
using namespace xmlbeansxx;

int main() {
    try {
        //xml tree creation using generated classes from c.xsd
        ContentDocument root(ContentDocument::Factory::newInstance());
        ContentType ct(ContentType::Factory::newInstance());
        root->setContent(ct);
        Fullpersoninfo p(Fullpersoninfo::Factory::newInstance());
        ct->setEmployee(p);
        p->setCity(77);
        p->xsetAddress(newInstance<XmlString>("_addr_"));
        p->xsetLastname(newInstance<XmlString>("_ln<>_"));
        p->xsetFirstname(newInstance<XmlString>("_fn_"));
        p->xsetDt(newInstance<XmlDateTime>("\n\n 2004-01-30T22:50:11  "));
        Array<String> tab = p->getTableArray();
        tab.push_back("");
        tab.push_back("");
		p->setTableArray(tab);
		p->addNewTable();
        p->xsetCar(newInstance<CarType>("2"));
        p->setCarAttr(3);

        p->xsetAgeAttr(newInstance<XmlInt>(20));
        p->xsetMoneyAttr(newInstance<AmountType>("10.256789"));
		cout<<root->toString();

    } catch (BeansException ex) {
        cout<<"Exception: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
