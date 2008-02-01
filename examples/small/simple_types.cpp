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
	
	cout << "\n************************* TEST1 (XmlString) *************************\n";
		XmlString sx(XmlAnySimpleType("xxx"));
		XmlBoolean bx(std::string("0"));
		XmlString s1(XmlString("a")),s2(XmlString("a"));
		cout<<(s1==s2)<<"\n";

        XmlString s3("a"),s4("bleble");
	XmlString s5=s4;
	s3.toString();
        cout<<"s4:"<<s3.toString()<<"\n";
        cout<<"s5:"<<s5.toString()<<"\n";
        cout<<"sx:"<<sx.toString()<<"\n";
        cout<<"s1:"<<s1.toString()<<"\n";

	cout << "\n************************* TEST2 (XmlDecimal) *************************\n";
        XmlDecimal a(double(12));
        XmlDecimal b(double(15));
        XmlDecimal c(a+b);
        cout<<c.getSchemaType()->className<<":"<<c.getSimpleContent()<<"\n";

        XmlObjectPtr l=globalTypeSystem()->createByName(QName("http://www.w3.org/2001/XMLSchema","integer"));
	l->setSimpleContent("123");
        cout<<std::string(l->getSchemaType()->name)<<":"<<l->getSimpleContent()<<"\n"<<l->toString()<<"\n";


	cout << "\n************************* TEST3 (XmlBoolean) *************************\n";

        //testing fundamental types
        XmlBoolean boolean1(std::string("false")),boolean2(std::string("true"));
	cout<<"boolean1:"<<boolean1.getBooleanValue()<<"\n";
	cout<<"boolean2:"<<boolean2.getBooleanValue()<<"\n";
	cout<<"boolean1:"<<boolean1.toString()<<"\n";
	cout<<"boolean2:"<<boolean2.toString()<<"\n";
        cout<<"boolean1==boolean2:"<<(boolean1==boolean2).toString()<<"\n";
        cout<<"boolean1==!boolean2:"<<(boolean1==!boolean2).toString()<<"\n";
        cout<<"boolean1!=boolean2:"<<(boolean1==!boolean2).toString()<<"\n";
	

		
	XmlObject obj=XmlPositiveInteger("10");
	XmlInteger obj2=XmlInteger(obj);
	cout<<"obj2:"<<obj2.toString()<<"\n";
	XmlDecimal obj3=XmlDecimal(obj);
	cout<<"obj3:"<<obj3.toString()<<"\n";
        }
        {
	    cout << "\n************************* TEST4 (XmlString) *************************\n";

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
	    cout << "\n************************* TEST5 (XmlDateTime) *************************\n";
            //XmlDateTime
            XmlDateTime a(" 2004-01-12T23:50:22 \n\n\n");
            XmlDateTime b("   2004-01-12T23:50:22 \n");
            XmlDateTime c("   2004-01-12T23:50:24 \n");
            XmlDateTime d("   2004-01-11T23:50:24 \n");
            cout<<"a:'"<<a.getSimpleContent()<<"'\n";
            cout<<"b:'"<<b.getSimpleContent()<<"'\n";
            cout<<"c:'"<<c.getSimpleContent()<<"'\n";
            cout<<"d:'"<<d.getSimpleContent()<<"'\n";
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
