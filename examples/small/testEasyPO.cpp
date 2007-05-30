#include "EasyPO.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace xmlbeansxx;
using namespace xmlbeansxx::samples::enumeration::schemaenum::easypo;

int main() {

    try {
        fstream in("easypo.xml", ios::in);
        PurchaseOrderDocument poDoc=PurchaseOrderDocument::Factory::parse(in);

        LineItem giftLineItem = poDoc.getPurchaseOrder().addNewLineItem();
	giftLineItem.setDescription(string("Calendar"));
        giftLineItem.setPrice(3);
        giftLineItem.setQuantity(6);
        giftLineItem.setPerUnitOunces(10);
        
        vector<LineItem> arr = poDoc.getPurchaseOrder().getLineItemArray();
        for(unsigned i=0; i < arr.size() ; i++) {
            cout << "item: " << i << "\n";
            cout << " - description:     " << arr[i].getDescription() << "\n";
            cout << " - quantity:        " << arr[i].getQuantity() << "\n";
            cout << " - price:           " << arr[i].getPrice() << "\n";
            cout << " - amount:          " << arr[i].getQuantity() * arr[i].getPrice() << "\n";
        }
        
	cout << "Xml:\n" << poDoc.toString() << "\n";
	
    } catch (BeansException &ex) {
        cout<<"BeansException: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
