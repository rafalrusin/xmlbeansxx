#include "c.h"
#include <iostream>

using namespace std;

using namespace com::p4::mind::mytest;
using namespace xmlbeansxx;

int main() {
    try {
        //xml tree creation using generated classes from c.xsd
        ContentDocumentPtr root(new ContentDocument());
        //root->cgetContent()->set
        ContentTypePtr ct(new ContentType());
        root->setContent(ct);
        FullpersoninfoPtr p(new Fullpersoninfo());
        ct->setEmployee(p);
        p->setCity(77);
        p->xsetAddress(XmlStringPtr(new XmlString("_addr_")));
        p->xsetLastname(XmlStringPtr(new XmlString("_ln<>_")));
        p->xsetFirstname(XmlStringPtr(new XmlString("_fn_")));
        p->setDt("\n\n 2004-01-30T22:50:11  ");
        boost::shared_ptr<XmlArray<XmlString> > tab=p->dgetTableArray();
        tab->cgetArray(0);
        tab->cgetArray(1);
		p->dsetTableArray(tab);
		p->addNewTable();
        p->xsetCar(CarTypePtr(new CarType("2")));
        p->setCarAttr(CarTypePtr(new CarType("3"))->getSimpleContent());

        p->xsetAgeAttr(XmlIntPtr(new XmlInt(20)));
        p->xsetMoneyAttr(AmountTypePtr(new AmountType("10.256789")));
        //root->serialize(cout);
		cout<<root->toString();

        //cout<<"tab:";
        //tab->serialize(cout);

    } catch (BeansException ex) {
        cout<<"Exception: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
