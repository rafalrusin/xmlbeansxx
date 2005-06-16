#include "BeansTest.h"
#include <string>

#include "log4cxx/logger.h"
#include <xmlbeansxx/xmlbeansxx.h>
#include <boost/shared_array.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( BeansTest );

void namespaceTests();

log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(std::string("test.BeansTest"));

void BeansTest::beansTest() {
    try {
        namespaceTests();
    } catch (xmlbeansxx::BeansException e) {
        LOG4CXX_ERROR(logger,"Exception: "+std::string(e.getMessage()));
        throw e;
    }
}


using namespace xmlbeansxx;

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
            LOG4CXX_DEBUG(logger,std::string("HashCode for ") + s + "=" + TextUtils::intToString(s.hashCode()));
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
        XmlStringPtr s1(new XmlString("a")),s2(new XmlString("a"));
        CPPUNIT_ASSERT(sx==XmlString("xxx"));
        CPPUNIT_ASSERT((*s1)==(*s2));
        //cout<<(s1==s2)<<"\n";

        XmlString s3("a"),s4("b"),s5(s3);
        CPPUNIT_ASSERT(s3!=s4);
        //cout<<"s5:"<<s5.toString()<<"\n";

        boost::shared_ptr<XmlDecimal> a(new XmlDecimal(-12));
        boost::shared_ptr<XmlDecimal> b(new XmlDecimal(+15));
        boost::shared_ptr<XmlDecimal> c(new XmlDecimal(*a+*b));
        std::cout << a->getSimpleContent() << " + "
                  << b->getSimpleContent() << " = "
                  << c->getSimpleContent() << std::endl;
        //cout<<c->className()<<":"<<c->getSimpleContent()<<"\n";

        boost::shared_ptr<XmlObject> l=globalTypeSystem()->createByName("integer",globalTypeSystem()->getNamespace("http://www.w3.org/2001/XMLSchema"));
        //cout<<l->xsdTypeName()<<":"<<l->getSimpleContent()<<"\n"<<l->toString()<<"\n";


        //testing fundamental types
        XmlBoolean boolean1(std::string("false")),boolean2(std::string("true"));
        CPPUNIT_ASSERT(boolean1.getBooleanValue()==false);
        CPPUNIT_ASSERT(boolean2.getBooleanValue()==true);
        CPPUNIT_ASSERT(!(boolean1==boolean2).getBooleanValue());
        CPPUNIT_ASSERT((boolean1==!boolean2).getBooleanValue());

        XmlObjectPtr obj=XmlObjectPtr(new XmlPositiveInteger("10"));
        XmlIntegerPtr obj2=java_cast<XmlInteger>(obj);
        logger->debug("obj2: "+obj2->getSimpleContent());
        CPPUNIT_ASSERT(obj2->getSimpleContent()=="10");
        //XmlDecimalPtr obj3=java_cast<XmlDecimal>(obj);

    }
    {
        //XmlInteger
        CPPUNIT_ASSERT(((int)XmlInteger("125"))==125);
        CPPUNIT_ASSERT((XmlInteger("-3")+XmlInteger("-5"))==XmlInteger("-8"));
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
        //XmlDateTime
        XmlDateTime a(" 2004-01-12T23:50:22 \n\n\n");
        XmlDateTime b("   2004-01-12T23:50:22 \n");
        XmlDateTime c("   2004-01-12T23:50:24 \n");
        XmlDateTime d("   2004-01-11T23:50:24 \n");
		XmlDateTime e(XmlAnySimpleType(" 2004-01-11T23:50:24 "));
        CPPUNIT_ASSERT(a.getSimpleContent()=="2004-01-12T23:50:22");
        CPPUNIT_ASSERT(a.datePart().getSimpleContent()=="2004-01-12");
        CPPUNIT_ASSERT(a.timePart().getSimpleContent()=="23:50:22");
        CPPUNIT_ASSERT(a==b);
        CPPUNIT_ASSERT(!(a<b));
        CPPUNIT_ASSERT(a<c);
        CPPUNIT_ASSERT(d<a);
        CPPUNIT_ASSERT(e==d);
    }
    {
        //XmlDate
        XmlDate a("   2001-03-24");
        boost::gregorian::date d=a.getDateValue();
        CPPUNIT_ASSERT(d==boost::gregorian::from_string("2001-3-24"));
        LOG4CXX_DEBUG(logger,a.daysFrom(XmlDate(" 2001-01-24\n")).getSimpleContent());
        CPPUNIT_ASSERT(a.daysFrom(XmlDate(" 2001-01-24\n"))==XmlInteger("59"));
        CPPUNIT_ASSERT(XmlDate("2003-01-01").daysFrom(XmlDate(" 2002-12-30\n"))==XmlInteger("2"));

    }

    {
        //serialization/parsing
        XmlPositiveIntegerPtr x(new XmlPositiveInteger("101")),y;
        LOG4CXX_DEBUG(logger,x->toString());
        y=XmlPositiveInteger::Factory::parse(x->toString());
        CPPUNIT_ASSERT(x->getSimpleContent()==y->getSimpleContent());

        boost::shared_ptr<XmlArray<XmlPositiveInteger> > ar(new XmlArray<XmlPositiveInteger>()),ar2;
        ar->setArray(0,XmlPositiveIntegerPtr(new XmlPositiveInteger("102")));
        ar->setArray(1,XmlPositiveIntegerPtr(new XmlPositiveInteger("103")));
        ar->setArray(2,XmlPositiveIntegerPtr(new XmlPositiveInteger("104")));
        logger->debug(ar->toString());
        LOG4CXX_DEBUG(logger,"!a");
        ar2=java_cast<XmlArray<XmlPositiveInteger> >(XmlObject::Factory::parse(ar->toString()));
        LOG4CXX_DEBUG(logger,"!b");
        CPPUNIT_ASSERT(ar2->getArray(0)->getSimpleContent()=="102");
        CPPUNIT_ASSERT(ar2->getArray(1)->getSimpleContent()=="103");
        CPPUNIT_ASSERT(ar2->getArray(2)->getSimpleContent()=="104");
    }
    {
        //array cast
        boost::shared_ptr<XmlArray<XmlInteger> > a(new XmlArray<XmlInteger>()),b;
        boost::shared_ptr<XmlArray<XmlObject> > c;
        a->append(XmlIntegerPtr(new XmlInteger("101")));
        a->append(XmlPositiveIntegerPtr(new XmlPositiveInteger("102")));
        a->append(XmlIntegerPtr(new XmlInteger("103")));
        LOG4CXX_DEBUG(logger,"!1");
        c=xmlarray_java_cast<XmlObject>(a);
        LOG4CXX_DEBUG(logger,"!2");
        b=xmlarray_java_cast<XmlInteger>(c);
        LOG4CXX_DEBUG(logger,"!3");
        LOG4CXX_DEBUG(logger,a->toString());
        LOG4CXX_DEBUG(logger,std::string(a->getArray(0)->getSchemaType()->className));
        LOG4CXX_DEBUG(logger,std::string(a->getArray(1)->getSchemaType()->className));
        LOG4CXX_DEBUG(logger,std::string(a->getArray(2)->getSchemaType()->className));
        CPPUNIT_ASSERT(std::string(a->getArray(0)->getSchemaType()->className)=="xmlbeansxx::XmlInteger");
        CPPUNIT_ASSERT(std::string(a->getArray(1)->getSchemaType()->className)=="xmlbeansxx::XmlPositiveInteger");
        CPPUNIT_ASSERT(std::string(a->getArray(2)->getSchemaType()->className)=="xmlbeansxx::XmlInteger");
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

        LOG4CXX_DEBUG(logger,a->digest().getSimpleContent());
        LOG4CXX_DEBUG(logger,b->digest().getSimpleContent());
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
        LOG4CXX_DEBUG(logger,"Base64 size:" + TextUtils::intToString(s.size()));
        LOG4CXX_DEBUG(logger,"Base64:" + s);
        CPPUNIT_ASSERT(TextUtils::collapse(s)==std::string("YWJj"));

        XmlBase64BinaryPtr b=XmlBase64Binary::Factory::newInstance();
        b->setByteArrayValue(a);
        LOG4CXX_DEBUG(logger,"Base64 xml:" + b->toString());
        CPPUNIT_ASSERT(b->getByteArrayValue()[0]=='a');
        CPPUNIT_ASSERT(b->getByteArrayValue()[1]=='b');
        CPPUNIT_ASSERT(b->getByteArrayValue()[2]=='c');
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
}

