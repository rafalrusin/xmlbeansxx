#include <xmlbeansxx/xmlbeansxx.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace xmlbeansxx;

int main() {
    try {
        {
        /*
        XmlInteger i(101),j(102),k(i+j);
        cout<<k.toString()<<"\n";
        */
		XmlString sx(XmlAnySimpleType("xxx"));
		XmlBoolean bx(std::string("0"));
		XmlStringPtr s1(new XmlString("a")),s2(new XmlString("a"));
		cout<<(s1==s2)<<"\n";

        XmlString s3("a"),s4("b"),s5(s3);
        cout<<"s5:"<<s5.toString()<<"\n";

        boost::shared_ptr<XmlDecimal> a(new XmlDecimal(12));
        boost::shared_ptr<XmlDecimal> b(new XmlDecimal(15));
        boost::shared_ptr<XmlDecimal> c(new XmlDecimal(*a+*b));
        cout<<c->getSchemaType()->className<<":"<<c->getSimpleContent()<<"\n";

        boost::shared_ptr<XmlObject> l=globalTypeSystem()->createByName("integer",globalTypeSystem()->getNamespace("http://www.w3.org/2001/XMLSchema"));
        cout<<l->getSchemaType()->xsdTypeName<<":"<<l->getSimpleContent()<<"\n"<<l->toString()<<"\n";


        //testing fundamental types
        XmlBoolean boolean1(std::string("false")),boolean2(std::string("true"));
		cout<<"boolean1:"<<boolean1.getBooleanValue()<<"\n";
		cout<<"boolean2:"<<boolean2.getBooleanValue()<<"\n";
		cout<<"boolean1:"<<boolean1.toString()<<"\n";
		cout<<"boolean2:"<<boolean2.toString()<<"\n";
        cout<<"boolean1==boolean2:"<<(boolean1==boolean2).toString()<<"\n";
        cout<<"boolean1==!boolean2:"<<(boolean1==!boolean2).toString()<<"\n";
        cout<<"boolean1!=boolean2:"<<(boolean1==!boolean2).toString()<<"\n";
		
		XmlObjectPtr obj=XmlObjectPtr(new XmlPositiveInteger("10"));
		XmlIntegerPtr obj2=java_cast<XmlInteger>(obj);
		cout<<"obj2:"<<obj2->toString()<<"\n";
		//XmlDecimalPtr obj3=java_cast<XmlDecimal>(obj);
        }
        {
            //XmlString
            XmlString s("abcdefghijk"),s2=s.substring(5,7),s3=s.substring(-3,-1),s4=s.substring(-4);
            cout<<"s:"<<s.getSimpleContent()
                <<" s2:"<<s2.getSimpleContent()
                <<" s3:"<<s3.getSimpleContent()
                <<" s4:"<<s4.getSimpleContent()
                <<"\n";
            cout<<"s endsWith:ijk: "<<XmlBoolean(s.endsWith(XmlString("ijk"))).getSimpleContent()<<"\n";
            cout<<"s endsWith:lijk: "<<XmlBoolean(s.endsWith(XmlString("lijk"))).getSimpleContent()<<"\n";
            cout<<"abc+def: "<<(XmlString("abc")+XmlString("def")).getSimpleContent()<<"\n";
        }

        {
            //XmlDateTime
            XmlDateTime a(" 2004-01-12T23:50:22 \n\n\n");
            XmlDateTime b("   2004-01-12T23:50:22 \n");
            XmlDateTime c("   2004-01-12T23:50:24 \n");
            XmlDateTime d("   2004-01-11T23:50:24 \n");
            cout<<"a:'"<<a.getSimpleContent()<<"'\n";
            cout<<"a.datePart:'"<<a.datePart().getSimpleContent()<<"'\n";
            cout<<"a.timePart:'"<<a.timePart().getSimpleContent()<<"'\n";
            cout<<"a==b:"<<XmlBoolean(a==b).getSimpleContent()<<"\n";
            cout<<"a<b:"<<XmlBoolean(a<b).getSimpleContent()<<"\n";
            cout<<"a<c:"<<XmlBoolean(a<c).getSimpleContent()<<"\n";
            cout<<"a<d:"<<XmlBoolean(a<d).getSimpleContent()<<"\n";
            cout<<"a>b:"<<XmlBoolean(a>b).getSimpleContent()<<"\n";
            cout<<"a>c:"<<XmlBoolean(a>c).getSimpleContent()<<"\n";
            cout<<"a>d:"<<XmlBoolean(a>d).getSimpleContent()<<"\n";
        }
		
    } catch (BeansException ex) {
        cout<<"Exception: "<<ex.getMessage()<<"\n";
    }
    return 0;
}
