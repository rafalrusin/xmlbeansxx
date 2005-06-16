#include "EasyPO.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace xmlbeansxx;
using namespace xmlbeansxx::samples::enumeration::schemaenum::easypo;

int main() {

    try {
        fstream in("easypo.xml", ios::in);
        PurchaseOrderDocumentPtr poDoc(PurchaseOrderDocument::Factory::parse(in));

        LineItemPtr newLineItem = poDoc->getPurchaseOrder()->addNewLineItem();
	newLineItem->setDescription(string("new book"));
        newLineItem->setPrice(11.5);
        newLineItem->setQuantity(10);
        newLineItem->setPerUnitOunces(10);
        
        xmlbeansxx::shared_array<LineItemPtr> arr = poDoc->getPurchaseOrder()->getLineItemArray();
        for(int i=0; i < arr.size() ; i++) {
            cout << "item: " << i << "\n";
            cout << " - description:     " << arr[i]->getDescription() << "\n";
            cout << " - quantity:        " << arr[i]->getQuantity() << "\n";
            cout << " - price:           " << arr[i]->getPrice() << "\n";
            cout << " - amount:          " << arr[i]->getQuantity() * arr[i]->getPrice() << "\n";
        }
        
	cout << "Xml:\n" << poDoc->toString() << "\n";
	
    } catch (BeansException &ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
