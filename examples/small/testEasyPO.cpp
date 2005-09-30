#include "EasyPO.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace xmlbeansxx;
using namespace xmlbeansxx::samples::enumeration::schemaenum::easypo;

int main() {
    try {
        fstream in("easypo.xml", ios::in);
        PurchaseOrderDocument poDoc(PurchaseOrderDocument::Factory::parse(in));

        LineItem giftLineItem = poDoc->getPurchaseOrder()->addNewLineItem();
        giftLineItem->setDescription("Calendar");
        giftLineItem->setPrice(0);
        giftLineItem->setQuantity(1);
        giftLineItem->setPerUnitOunces(10);

        Array<LineItem> arr = poDoc->getPurchaseOrder()->getLineItemArray();
        for(int i=0; i < arr.size() ; i++) {
            cout << "item: " << i << endl;
            cout << " - description:     " << arr[i]->getDescription() << endl;
            cout << " - quantity:        " << arr[i]->getQuantity() << endl;
            cout << " - price:           " << arr[i]->getPrice() << endl;
            cout << " - amount:          " << arr[i]->getQuantity() * arr[i]->getPrice() << endl;
        }

        cout << "Xml:" << endl << poDoc->toString() << endl;

    } catch (BeansException &ex) {
        cout << "BeansException: " << ex.getMessage() << endl;
    }
    return 0;
}
