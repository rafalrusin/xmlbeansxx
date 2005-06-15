#include "EasyPO.h"
#include <iostream>
#include <fstream>
//#include <vector>
//#include <log4cxx/logger.h>
//#include <log4cxx/basicconfigurator.h>

using namespace std;
using namespace xmlbeansxx;
using namespace xmlbeansxx::samples::enumeration::schemaenum::easypo;

int main() {

    try {
        fstream in("easypo.xml", ios::in);
        PurchaseOrderDocumentPtr purchaseOrderDoc(PurchaseOrderDocument::Factory::parse(in));

        LineItemPtr newLineItem = purchaseOrderDoc->getPurchaseOrder()->addNewLineItem();
        //cout << newLineItem << "\n";
        //LineItemPtr newLineItem = LineItem::Factory::newInstance();
        //conversion to boolean tests contents
        if (!newLineItem)
            cout << "newLineItem contains null" << "\n";
        newLineItem->setDescription(string("new book"));
        newLineItem->setPrice(11.5);
        newLineItem->setQuantity(10);
        newLineItem->setPerUnitOunces(10);
        //purchaseOrderDoc->getPurchaseOrder()
        //purchaseOrder->serialize(cout);
        //cout << "/n";
        xmlbeansxx::shared_array<LineItemPtr> arr = purchaseOrderDoc->getPurchaseOrder()->getLineItemArray();
        //double totalWeight = 0;
        //double weight = 0;
        for(int i=0; i < arr.size() ; i++) {
            cout << "item: " << i << "\n";
            cout << " - description:     " << arr[i]->getDescription() << "\n";
            cout << " - quantity:        " << arr[i]->getQuantity() << "\n";
            cout << " - price:           " << arr[i]->getPrice() << "\n";
            cout << " - amount:          " << arr[i]->getQuantity() * arr[i]->getPrice() << "\n";
        }
        //cout << "---------------------------------" << "\n";
        //cout << "total weight: " << totalWeight << "\n";
        cout << "Xml:\n" << purchaseOrderDoc->toString() << "\n";
    } catch (BeansException ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
