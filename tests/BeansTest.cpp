#include "BeansTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/xmlbeansxx.h>
#include <boost/shared_array.hpp>

using namespace xmlbeansxx;
using namespace boost::posix_time;
using namespace boost::gregorian;

CPPUNIT_TEST_SUITE_REGISTRATION( BeansTest );

void namespaceTests();

XMLBEANSXX_LOGGER_PTR_SET(logger,"test.BeansTest");

void BeansTest::beansTest() {
    try {
        namespaceTests();
    } catch (xmlbeansxx::BeansException e) {
        XMLBEANSXX_ERROR(logger,"Exception: "+std::string(e.getMessage()));
        throw e;
    }
}


void testBase64(std::string cmp,std::string cmp2)
{	//base64

		xmlbeansxx::shared_array<unsigned char> cmp3(cmp2.size());
		for(unsigned i=0;i<cmp2.size();i++) cmp3[i]=cmp2[i];

       		XMLBEANSXX_DEBUG(logger,"base64 string cmp:" +cmp);
       		XMLBEANSXX_DEBUG(logger,"base64 string cmp2:" +cmp2);

		xmlbeansxx::shared_array<unsigned char> a = TextUtils::base64Decode(cmp);
		std::string b = TextUtils::base64Encode(cmp3);

		std::string a_str= std::string((char*)a.get(),a.size());

       		XMLBEANSXX_DEBUG(logger,"base64 string a:" +a_str);
       		XMLBEANSXX_DEBUG(logger,"base64 string b:" +b);

		CPPUNIT_ASSERT_EQUAL(cmp2,a_str);
		CPPUNIT_ASSERT_EQUAL(cmp,b);

}




void namespaceTests() {
    {
        //StoreString
        StoreString::store("abcdefgh");
        StoreString::store("abcdefgh");
        CPPUNIT_ASSERT(std::string().c_str()!=NULL);

        StoreString str[100],str2[100];

        for(int i=0;i<100;i++) {
            str[i]=StoreString(std::string("abc")+std::string("defgh"));
            str2[i]=StoreString(std::string("abc")+std::string("defghi"));
        }
        for(int i=1;i<100;i++) {
            CPPUNIT_ASSERT(str[i-1].c_str()==str[i].c_str());
            CPPUNIT_ASSERT(str[i]==std::string("abcdefgh"));
            CPPUNIT_ASSERT(str2[i]==std::string("abcdefghi"));
        }
        for(int i=1;i<100;i++) {
            CPPUNIT_ASSERT(str2[i-1].c_str()!=str2[i].c_str());

            CPPUNIT_ASSERT(str[i].hashCode()==CStrHashFn()("abcdefgh"));
            CPPUNIT_ASSERT(str[i].hashCode()!=CStrHashFn()("abcdefghi"));

            CPPUNIT_ASSERT(str2[i].hashCode()!=CStrHashFn()("abcdefgh"));
            CPPUNIT_ASSERT(str2[i].hashCode()==CStrHashFn()("abcdefghi"));
        }
        {
            StoreString s("abcdef");
            XMLBEANSXX_DEBUG(logger,std::string("HashCode for ") + s + "=" + TextUtils::intToString(s.hashCode()));
        }

    }
    {
        /*
        XmlInteger i(101),j(102),k(i+j);
        cout<<k.toString()<<"\n";
        */
        XmlString sx(XmlAnySimpleType("xxx"));
        CPPUNIT_ASSERT(sx==XmlString("xxx"));

        XmlBoolean bx(std::string("0"));
        XmlString s1("a"),s2("a");
        CPPUNIT_ASSERT(sx==XmlString("xxx"));
        CPPUNIT_ASSERT(s1==s2);
        //cout<<(s1==s2)<<"\n";

        XmlString s3("a"),s4("b"),s5(s3);
        CPPUNIT_ASSERT(s3!=s4);
        //cout<<"s5:"<<s5.toString()<<"\n";

        XmlDecimal a(double(-12));
        XmlDecimal b(double(+15));
        XmlDecimal c(a+b);
        std::cout << a.getSimpleContent() << " + "
                  << b.getSimpleContent() << " = "
                  << c.getSimpleContent() << std::endl;
        //cout<<c->className()<<":"<<c->getSimpleContent()<<"\n";

        XmlObjectPtr l=globalTypeSystem()->createByName(QName("http://www.w3.org/2001/XMLSchema","integer"));
        //cout<<l->xsdTypeName()<<":"<<l->getSimpleContent()<<"\n"<<l->toString()<<"\n";


        //testing fundamental types
        XmlBoolean boolean1(std::string("false")),boolean2(std::string("true"));
        CPPUNIT_ASSERT(boolean1.getBooleanValue()==false);
        CPPUNIT_ASSERT(boolean2.getBooleanValue()==true);
        CPPUNIT_ASSERT(!(boolean1==boolean2).getBooleanValue());
        CPPUNIT_ASSERT((boolean1==!boolean2).getBooleanValue());

        XmlObject obj=XmlPositiveInteger("10");
        XmlInteger obj2=obj;
        XMLBEANSXX_DEBUG(logger,"obj2: "+obj2.getSimpleContent());
        CPPUNIT_ASSERT(obj2.getSimpleContent()=="10");
        //XmlDecimalPtr obj3=java_cast<XmlDecimal>(obj);

    }
    {
        //XmlInteger
        CPPUNIT_ASSERT(((int)XmlInteger("125"))==125);
        CPPUNIT_ASSERT((XmlInteger("-3")+XmlInteger("-5"))==XmlInteger("-8"));
	XMLBEANSXX_DEBUG(logger,"-3.5 + (-5.7) = " + (XmlDecimal("-3.5")+XmlDecimal("-5.7")).toString());
        CPPUNIT_ASSERT((XmlDecimal("-3.5")+XmlDecimal("-5.7"))==XmlDecimal("-9.2"));
        XmlInteger a(10),b(11);
        a+=b;
        CPPUNIT_ASSERT(a==XmlInteger(21));
        int x=21;
        CPPUNIT_ASSERT(x==a.getMpzValue());

        try {
            XmlInteger c("10"),d("abc");
            CPPUNIT_ASSERT((c+d)==XmlInteger(10));
            CPPUNIT_ASSERT(false);
        } catch (BeansException e) {
        }
    }

    {
        //XmlString
        XmlString s("abcdefghijk"),s2=s.substring(5,7),s3=s.substring(-3,-1),s4=s.substring(-4);
        CPPUNIT_ASSERT(s.getSimpleContent()=="abcdefghijk");
        CPPUNIT_ASSERT(s2.getSimpleContent()=="fg");
        CPPUNIT_ASSERT(s3.getSimpleContent()=="ij");
        CPPUNIT_ASSERT(s4.getSimpleContent()=="hijk");
        CPPUNIT_ASSERT(s.endsWith(XmlString("ijk")));
        CPPUNIT_ASSERT(!s.endsWith(XmlString("lijk")));
        CPPUNIT_ASSERT((XmlString("abc")+XmlString("def")).getSimpleContent()=="abcdef");
    }

    {
    	//XmlTime
	{
		ptime pt(date(2002,Jan,10),time_duration(1,2,3));
		XmlTime t(pt);
		XMLBEANSXX_DEBUG(logger,std::string("time: ") + t.getSimpleContent());
		CPPUNIT_ASSERT(t.getSimpleContent()=="01:02:03");
		CPPUNIT_ASSERT(t.getTimeValue()==time_duration(1,2,3));
		CPPUNIT_ASSERT(t.getCalendarValue().getTime()==ptime(date(),time_duration(1,2,3)));
	}
	{
		XmlTime t("	01:02:03  ");
		XMLBEANSXX_DEBUG(logger,std::string("time: ") + t.getSimpleContent());
		CPPUNIT_ASSERT(t.getSimpleContent()=="01:02:03");
		CPPUNIT_ASSERT(t.getTimeValue()==time_duration(1,2,3));
		CPPUNIT_ASSERT(t.getCalendarValue().getTime()==ptime(date(),time_duration(1,2,3)));
	}


    }

    {
        //XmlDateTime
	{
	        XmlDateTime a(" 2004-01-12T23:50:22 \n\n\n");
	        XmlDateTime b("   2004-01-12T23:50:22Z \n");
	        XmlDateTime c("   2004-01-12T23:50:24 \n");
	        XmlDateTime d("   2004-01-11T23:50:24 \n");
					XmlDateTime e(XmlAnySimpleType(" 2004-01-11T23:50:24 "));
					CPPUNIT_ASSERT(a.getSimpleContent()=="2004-01-12T23:50:22");
	        CPPUNIT_ASSERT(a.datePart().getSimpleContent()=="2004-01-12");
	        CPPUNIT_ASSERT(a.timePart().getSimpleContent()=="23:50:22");
	        CPPUNIT_ASSERT(a.datePart().getDateValue()==date(2004,01,12));
	        CPPUNIT_ASSERT(a.timePart().getTimeValue()==time_duration(23,50,22));
	        CPPUNIT_ASSERT(a==b);
	        CPPUNIT_ASSERT(!(a<b));
	        CPPUNIT_ASSERT(a<c);
	        CPPUNIT_ASSERT(d<a);
	        CPPUNIT_ASSERT(e==d);
	}
	{
	        XmlDateTime a(ptime(date(2004,01,12),time_duration(23,50,22)));
	        CPPUNIT_ASSERT(a.getSimpleContent()=="2004-01-12T23:50:22");
	        CPPUNIT_ASSERT(a.datePart().getSimpleContent()=="2004-01-12");
	        CPPUNIT_ASSERT(a.timePart().getSimpleContent()=="23:50:22");
	        CPPUNIT_ASSERT(a.datePart().getDateValue()==date(2004,01,12));
	        CPPUNIT_ASSERT(a.timePart().getTimeValue()==time_duration(23,50,22));
	}
    }
    {
        //XmlDate
	{
	        XmlDate a("   2001-03-24");
	        boost::gregorian::date d=a.getDateValue();
	        CPPUNIT_ASSERT(d==boost::gregorian::from_string("2001-3-24"));
	        XMLBEANSXX_DEBUG(logger,a.daysFrom(XmlDate(" 2001-01-24\n")).getSimpleContent());
	        CPPUNIT_ASSERT(a.daysFrom(XmlDate(" 2001-01-24\n"))==XmlInteger("59"));
	        CPPUNIT_ASSERT(XmlDate("2003-01-01").daysFrom(XmlDate(" 2002-12-30\n"))==XmlInteger("2"));
	}
	{
	        XmlDate a(date(2001,03,24));
	        boost::gregorian::date d=a.getDateValue();
	        CPPUNIT_ASSERT(d==boost::gregorian::from_string("2001-3-24"));
	        XMLBEANSXX_DEBUG(logger,a.daysFrom(XmlDate(" 2001-01-24\n")).getSimpleContent());
	        CPPUNIT_ASSERT(a.daysFrom(XmlDate(" 2001-01-24\n"))==XmlInteger("59"));
	        CPPUNIT_ASSERT(XmlDate("2003-01-01").daysFrom(XmlDate(" 2002-12-30\n"))==XmlInteger("2"));
	}

    }

    {
        //serialization/parsing
        XmlPositiveInteger x("101"),y;
        XMLBEANSXX_DEBUG(logger,x.toString());
        y=XmlPositiveInteger::Factory::parse(x.toString());
        CPPUNIT_ASSERT(x.getSimpleContent()==y.getSimpleContent());

        XmlArray<XmlPositiveInteger> ar=XmlArray<XmlPositiveInteger>::Factory::newInstance(),ar2;
        ar.setArray(0,XmlPositiveInteger("102"));
        ar.setArray(1,XmlPositiveInteger("103"));
        ar.setArray(2,XmlPositiveInteger("104"));
        XMLBEANSXX_DEBUG(logger,"XmlArray ar: " + ar.toString(XmlOptions::serializeTypes()));
        XMLBEANSXX_DEBUG(logger,"!a");
/*        ar2=XmlObject::Factory::parse(ar.toString(XmlOptions::serializeTypes()));
        XMLBEANSXX_DEBUG(logger,"XmlArray ar2: " + ar2.toString());

	XMLBEANSXX_DEBUG(logger,"XmlArray ar2 dump: " + Contents::Walker::dump(ar2.contents));
        XMLBEANSXX_DEBUG(logger,"!b");
        CPPUNIT_ASSERT(ar2.getArray(0).getSimpleContent()=="102");
        CPPUNIT_ASSERT(ar2.getArray(1).getSimpleContent()=="103");
        CPPUNIT_ASSERT(ar2.getArray(2).getSimpleContent()=="104");
 */   }
    {
        //array cast
      	XmlArray<XmlInteger>  a=XmlArray<XmlInteger>::Factory::newInstance();
	XmlArray<XmlPositiveInteger> b;
        XmlArray<XmlObject>  c;
        a.append(XmlInteger("101"));
        a.append(XmlPositiveInteger("102"));
        a.append(XmlInteger("103"));
        XMLBEANSXX_DEBUG(logger,"!1");
        c=xmlarray_java_cast<XmlObject>(a);
        XMLBEANSXX_DEBUG(logger,"!2");
        b=xmlarray_java_cast<XmlPositiveInteger>(a);
        XMLBEANSXX_DEBUG(logger,"!3");
        XMLBEANSXX_DEBUG(logger,a.toString());
        XMLBEANSXX_DEBUG(logger,"!4");
        XMLBEANSXX_DEBUG(logger,b.toString());
        XMLBEANSXX_DEBUG(logger,std::string(a.getArray(0).getSchemaType()->className));
        XMLBEANSXX_DEBUG(logger,std::string(a.getArray(1).getSchemaType()->className));
        XMLBEANSXX_DEBUG(logger,std::string(a.getArray(2).getSchemaType()->className));
        CPPUNIT_ASSERT(std::string(a.getArray(0).getSchemaType()->className)=="xmlbeansxx::XmlInteger");
        CPPUNIT_ASSERT(std::string(a.getArray(1).getSchemaType()->className)=="xmlbeansxx::XmlPositiveInteger");
        CPPUNIT_ASSERT(std::string(a.getArray(2).getSchemaType()->className)=="xmlbeansxx::XmlInteger");
    }
    {
        /*
        //digest
        boost::shared_ptr<XmlArray<XmlInteger> > a(new XmlArray<XmlInteger>()),b(new XmlArray<XmlInteger>);
        a->append(XmlIntegerPtr(new XmlInteger("101")));
        a->append(XmlPositiveIntegerPtr(new XmlPositiveInteger("102")));
        a->append(XmlIntegerPtr(new XmlInteger("103")));

        b->append(XmlIntegerPtr(new XmlInteger("101")));
        b->append(XmlPositiveIntegerPtr(new XmlPositiveInteger("102")));
        b->append(XmlIntegerPtr(new XmlInteger("103")));

        XMLBEANSXX_DEBUG(logger,a->digest().getSimpleContent());
        XMLBEANSXX_DEBUG(logger,b->digest().getSimpleContent());
        CPPUNIT_ASSERT(a->digest()==b->digest());
        b->append(XmlIntegerPtr(new XmlInteger("103")));
        CPPUNIT_ASSERT(a->digest()!=b->digest());
        */
    }
    {
        //Base64
        shared_array<unsigned char> a(3);
        a[0]='a';
        a[1]='b';
        a[2]='c';

        std::string s=TextUtils::base64Encode(a);
        XMLBEANSXX_DEBUG(logger,"str size:" + TextUtils::intToString(a.size()));
        XMLBEANSXX_DEBUG(logger,"Base64 size:" + TextUtils::intToString(s.size()));
        XMLBEANSXX_DEBUG(logger,"Base64:" + s);
        CPPUNIT_ASSERT(TextUtils::collapse(s)==std::string("YWJj"));

        XmlBase64Binary b=XmlBase64Binary::Factory::newInstance();
        b.setByteArrayValue(a);
        XMLBEANSXX_DEBUG(logger,"Base64 xml:" + b.toString());
        CPPUNIT_ASSERT(b.getByteArrayValue()[0]=='a');
        CPPUNIT_ASSERT(b.getByteArrayValue()[1]=='b');
        CPPUNIT_ASSERT(b.getByteArrayValue()[2]=='c');

	//TODO:
/*	XmlBase64Binary b2=xmlbeansxx::XmlBase64Binary::Factory::parse("<a>OOOOOOOOOO&#xA;    \n \n \n \n  PPPP       PPPPPPPPPPP      </a>");
	XMLBEANSXX_DEBUG(logger,b2.toString(XmlOptions::serializeTypes()));
	XmlString s2=xmlbeansxx::XmlString::Factory::parse("<a>OOOOOOOOOO&#xA;    \n \n \n \n  PPPP       PPPPPPPPPPP      </a>");
	XMLBEANSXX_DEBUG(logger,s2.toString(XmlOptions::serializeTypes()));
	CPPUNIT_ASSERT(false);
  */
    }
    {
        //xmlbeansxx::shared_array
        shared_array<int> c = shared_array<int>();

        shared_array<int> b = shared_array<int>(3);
        b[0] = 10;
        b[1] = 11;
        b[2] = 12;

        shared_array<int> a(b);
        CPPUNIT_ASSERT(a[0] == 10);
        CPPUNIT_ASSERT(a[1] == 11);
        CPPUNIT_ASSERT(a[2] == 12);
    }
    {
    	//bool (==NULL)

    	XmlString test;
    	XmlString test2("test");
        XMLBEANSXX_DEBUG(logger,"NULL ptr:" + std::string(bool(test)? "true" : "false"));
        CPPUNIT_ASSERT(!test);
        XMLBEANSXX_DEBUG(logger,"NULL ptr:" + std::string(bool(test2)? "true" : "false"));
        CPPUNIT_ASSERT(test2);

    }

    	{
	       	XMLBEANSXX_DEBUG(logger,"testing Cast:");
		XmlBoolean b(true);
		XmlString s1(b);
		XmlObject o(b);
		XmlString s2(o);
	       	XMLBEANSXX_DEBUG(logger,"s1 string:" + s2.toString());
		CPPUNIT_ASSERT_EQUAL(s1.toString(),s2.toString());
	}
	{
	       	XMLBEANSXX_DEBUG(logger,"testing boolean true:");
		XmlString b("TruE");
		XmlObject o(b);
		XmlBoolean s2(o);
	       	XMLBEANSXX_DEBUG(logger,"s1 string:" + s2.toString());
		CPPUNIT_ASSERT_EQUAL(bool(s2),true);
	}
	{
	       	XMLBEANSXX_DEBUG(logger,"testing boolean false:");
		XmlString b("FalSe");
		XmlObject o(b);
		XmlBoolean s2(o);
	       	XMLBEANSXX_DEBUG(logger,"s1 string:" + s2.toString());
		CPPUNIT_ASSERT_EQUAL(bool(s2),false);
	}

	{ //Decimal

		{
			TextUtils::setPrecision(13);
			XmlDecimal d(0.1234567891234);
			XmlDecimal d2=XmlDecimal::Factory::parse(d.toString());
		       	XMLBEANSXX_DEBUG(logger,"decimal string:" + d2.toString());
			CPPUNIT_ASSERT_EQUAL(d2.getSimpleContent(),std::string("0.1234567891234"));
		}


#ifdef XMLBEANSXX_WITH_GMPXX
		{
			TextUtils::setPrecision(100);
			XmlDecimal d("1.000000000001");
			XmlDecimal d2=d*d*d*d*d;
	       		XMLBEANSXX_DEBUG(logger,"decimal string:" + d2.toString());
			CPPUNIT_ASSERT_EQUAL(d2.getSimpleContent(),std::string("1.000000000005000000000010000000000010000000000005000000000001"));
		}
#endif

		XmlFloat f;
		f.setFloatValue(1./3.);
	       	XMLBEANSXX_DEBUG(logger,"decimal string:" + f.toString());

	}

	{ //base64
		testBase64("UGF3ZWwgU3Rhd2lja2k=",     "Pawel Stawicki");
		testBase64("UGF3ZWwgU3Rhd2lja2ky",     "Pawel Stawicki2");
		testBase64("", "");
		testBase64("YQ==", "a");
		testBase64("YWE=", "aa");
		testBase64("YWFh", "aaa");
		testBase64("YWFhYQ==", "aaaa");



	}

}

