#include "c.h"
#include <iostream>

using namespace std;

using namespace com::p4::mind::mytest;
using namespace xmlbeansxx;

int main() {
    try {
        //xml tree creation using generated classes from c.xsd
        ContentDocumentPtr root(ContentDocument::Factory::newInstance());
        //root->cgetContent()->set
        ContentTypePtr ct(ContentType::Factory::newInstance());
        root->setContent(ct);
        FullpersoninfoPtr p(Fullpersoninfo::Factory::newInstance());
        ct->setEmployee(p);
        p->setCity(77);
        p->xsetAddress(XmlString::Factory::newInstance("_addr_"));
        p->xsetLastname(XmlString::Factory::newInstance("_ln<>_"));
        p->xsetFirstname(XmlString::Factory::newInstance("_fn_"));
        p->setDt("\n\n 2004-01-30T22:50:11  ");
        boost::shared_ptr<XmlArray<XmlString> > tab=p->dgetTableArray();
        tab->cgetArray(0);
        tab->cgetArray(1);
		p->dsetTableArray(tab);
		p->addNewTable();
        p->xsetCar(CarType::Factory::newInstance("2"));
        p->setCarAttr(3);

        p->xsetAgeAttr(XmlInt::Factory::newInstance(20));
        p->xsetMoneyAttr(AmountType::Factory::newInstance("10.256789"));
        //root->serialize(cout);
		cout<<root->toString();

        //cout<<"tab:";
        //tab->serialize(cout);

    } catch (BeansException ex) {
        cout<<"Exception: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
