#include "BeansTest.h"
#include <string>

#include "log4cxx/logger.h"
#include <xmlbeansxx/xmlbeansxx.h>

CPPUNIT_TEST_SUITE_REGISTRATION( BeansTest );

void namespaceTests();

log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger(std::string("test.BeansTest"));

void BeansTest::beansTest() {
    try {
        namespaceTests();
    } catch (xmlbeansxx::BeansException e) {
        LOG4CXX_ERROR(LOG,"Exception: "+std::string(e.getMessage()));
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
            LOG4CXX_DEBUG(LOG, std::string("HashCode for ") + s + "=" + TextUtils::intToString(s.hashCode()));
        }

    }
    {
        /*
        XmlInteger i(101),j(102),k(i+j);
        cout<<k.toString()<<"\n";
        */
        XmlString sx(XmlString::Factory::newInstance());
        sx->setStringValue("xxx");
        CPPUNIT_ASSERT(sx->getStringValue() == "xxx");

        XmlBoolean bx(XmlBoolean::Factory::newInstance()), by(XmlBoolean::Factory::newInstance());
        bx->setStringValue("0");
        by->setBooleanValue(false);
        //CPPUNIT_ASSERT(bx->getStringValue() == by->getStringValue());
        CPPUNIT_ASSERT(bx->getBooleanValue() == by->getBooleanValue());
        
        XmlString s1(XmlString::Factory::newInstance()),s2(XmlString::Factory::newInstance());
        s1->setStringValue("a");
        s2->setStringValue("a");
        CPPUNIT_ASSERT(sx->getStringValue() == "xxx");
        CPPUNIT_ASSERT(s1->getStringValue() == s2->getStringValue());
        //cout<<(s1==s2)<<"\n";

        XmlString s3(XmlString::Factory::newInstance()),s4(XmlString::Factory::newInstance());
        s3->setStringValue("a");
        s4->setStringValue("b");
        XmlString s5(XmlString::Factory::newInstance());
        s5->setStringValue(s3->getStringValue());
        
        CPPUNIT_ASSERT(s3->getStringValue() != s4->getStringValue());
        CPPUNIT_ASSERT(s3->getStringValue() == s3->getStringValue());
        //cout<<"s5:"<<s5.toString()<<"\n";

        XmlDecimal a(XmlDecimal::Factory::newInstance());
        a->setBigDecimalValue(-12);
        XmlDecimal b(XmlDecimal::Factory::newInstance());
        b->setBigDecimalValue(15);
        XmlDecimal c(XmlDecimal::Factory::newInstance());
        c->setBigDecimalValue(a->getBigDecimalValue() + b->getBigDecimalValue());

        XmlObject l=XmlBeans::getContextTypeLoader()->newInstance(XmlBeans::getContextTypeLoader()->findType(QName("http://www.w3.org/2001/XMLSchema", "integer")), XmlOptions::New());
        //cout<<l->xsdTypeName()<<":"<<l->getSimpleContent()<<"\n"<<l->toString()<<"\n";


        //testing fundamental types
        XmlBoolean boolean1(XmlBoolean::Factory::newInstance()),boolean2(XmlBoolean::Factory::newInstance());
        boolean1->setStringValue("false");
        boolean2->setStringValue("true");
        CPPUNIT_ASSERT(boolean1->getBooleanValue()==false);
        CPPUNIT_ASSERT(boolean2->getBooleanValue()==true);

        XmlAnySimpleType obj = XmlPositiveInteger::Factory::newInstance();
        obj->setStringValue("10");
        XmlInteger obj2=cast<XmlInteger>(obj);
        LOG->debug("obj2: " + obj2->getStringValue());
        CPPUNIT_ASSERT(obj2->getStringValue()=="10");
        //XmlDecimal obj3=java_cast<XmlDecimal>(obj);

    }
    {
        //XmlInteger
        XmlInteger i1 = XmlInteger::Factory::newInstance();
        i1->setStringValue("0129");
        CPPUNIT_ASSERT(i1->getBigIntegerValue() == 129);
        
        CPPUNIT_ASSERT((BigInteger(-3) + BigInteger(-5)) == BigInteger(-8));
        BigDecimal v = BigDecimal("-3.5") + BigDecimal("-5.7");
        LOG4CXX_DEBUG(LOG, "big decimal: " << v);
        //CPPUNIT_ASSERT(v == BigDecimal("-9.2"));
        
        XmlInteger a(XmlInteger::Factory::newInstance());
        a->setBigIntegerValue(21);
        CPPUNIT_ASSERT(a->getBigIntegerValue() == 21);

        /*
        TODO
        try {
            XmlInteger d(XmlInteger::Factory::newInstance());
            d->setStringValue("abc");
            CPPUNIT_ASSERT(false);
        } catch (BeansException e) {
        }
        */
    }

    /*
    {
        //XmlString
        XmlString s(XmlString::Factory::newInstance("abcdefghijk"));
        std::string s2=s->substring(5,7),s3=s->substring(-3,-1),s4=s->substring(-4);
        CPPUNIT_ASSERT(s->getSimpleContent()=="abcdefghijk");
        CPPUNIT_ASSERT(s2=="fg");
        CPPUNIT_ASSERT(s3=="ij");
        CPPUNIT_ASSERT(s4=="hijk");
        CPPUNIT_ASSERT(s->endsWith("ijk"));
        CPPUNIT_ASSERT(!s->endsWith("lijk"));
    }

    {
        //XmlDateTime
        XmlDateTime a(XmlDateTime::Factory::newInstance(" 2004-01-12T23:50:22 \n\n\n"));
        XmlDateTime b(XmlDateTime::Factory::newInstance("   2004-01-12T23:50:22 \n"));
        XmlDateTime c(XmlDateTime::Factory::newInstance("   2004-01-12T23:50:24 \n"));
        XmlDateTime d(XmlDateTime::Factory::newInstance("   2004-01-11T23:50:24 \n"));
        XmlDateTime e(XmlDateTime::Factory::newInstance(XmlAnySimpleType::Factory::newInstance(" 2004-01-11T23:50:24 ")));
        CPPUNIT_ASSERT(a->getSimpleContent()=="2004-01-12T23:50:22");
        CPPUNIT_ASSERT(a->datePart()->getSimpleContent()=="2004-01-12");
        CPPUNIT_ASSERT(a->timePart()->getSimpleContent()=="23:50:22");
        
        CPPUNIT_ASSERT((*a) == (*b));
        CPPUNIT_ASSERT(!((*a) < (*b)));
        CPPUNIT_ASSERT((*a) < (*c));
        CPPUNIT_ASSERT((*d) < (*a));
        CPPUNIT_ASSERT((*e) == (*d));
    }
    {
        //XmlDate
        XmlDate a(XmlDate::Factory::newInstance("   2001-03-24"));
        boost::gregorian::date d=a->getDateValue();
        CPPUNIT_ASSERT(d == boost::gregorian::from_string("2001-3-24"));
        LOG4CXX_DEBUG(LOG,a->daysFrom(XmlDate::Factory::newInstance(" 2001-01-24\n")));
        CPPUNIT_ASSERT(a->daysFrom(XmlDate::Factory::newInstance(" 2001-01-24\n")) == 59);
        CPPUNIT_ASSERT(XmlDate::Factory::newInstance("2003-01-01")->daysFrom(XmlDate::Factory::newInstance(" 2002-12-30\n")) == 2);

    }*/
    {
        SchemaType st = XmlBeans::getContextTypeLoader()->findType(QName(XmlBeans::xs_ns(), "positiveInteger"));
        LOG4CXX_DEBUG(LOG, st->getName()->toString());
        CPPUNIT_ASSERT( st != Null());
    }

    {
        //serialization/parsing
        XmlPositiveInteger x(XmlPositiveInteger::Factory::newInstance()),y;
        x->setStringValue("101");
        LOG4CXX_DEBUG(LOG, x->toString());
        y=XmlPositiveInteger::Factory::parse(x->toString());
        CPPUNIT_ASSERT(x->getStringValue() == y->getStringValue());
        CPPUNIT_ASSERT(x->getBigIntegerValue() == y->getBigIntegerValue());


        /*
        XmlArray ar(XmlArray::Factory::newInstance()),ar2;
        ar->setArray(0,XmlPositiveInteger(XmlPositiveInteger::Factory::newInstance("102")));
        ar->setArray(1,XmlPositiveInteger(XmlPositiveInteger::Factory::newInstance("103")));
        ar->setArray(2,XmlPositiveInteger(XmlPositiveInteger::Factory::newInstance("104")));
        LOG->debug(ar->toString());
        LOG4CXX_DEBUG(LOG,"!a");
        ar2=java_cast<XmlArray<XmlPositiveInteger> >(XmlObject::Factory::parse(ar->toString()));
        LOG4CXX_DEBUG(LOG,"!b");
        CPPUNIT_ASSERT(ar2->getArray(0)->getSimpleContent()=="102");
        CPPUNIT_ASSERT(ar2->getArray(1)->getSimpleContent()=="103");
        CPPUNIT_ASSERT(ar2->getArray(2)->getSimpleContent()=="104");
        */
    }
    {
        /*
        //array cast
        XmlArray<XmlInteger> > a(new XmlArray<XmlInteger>()),b;
        XmlArray<XmlObject> > c;
        a->append(XmlInteger(XmlInteger::Factory::newInstance("101")));
        a->append(XmlPositiveInteger(XmlPositiveInteger::Factory::newInstance("102")));
        a->append(XmlInteger(XmlInteger::Factory::newInstance("103")));
        LOG4CXX_DEBUG(LOG,"!1");
        c=xmlarray_java_cast<XmlObject>(a);
        LOG4CXX_DEBUG(LOG,"!2");
        b=xmlarray_java_cast<XmlInteger>(c);
        LOG4CXX_DEBUG(LOG,"!3");
        LOG4CXX_DEBUG(LOG,a->toString());
        LOG4CXX_DEBUG(LOG,std::string(a->getArray(0)->getSchemaType()->className));
        LOG4CXX_DEBUG(LOG,std::string(a->getArray(1)->getSchemaType()->className));
        LOG4CXX_DEBUG(LOG,std::string(a->getArray(2)->getSchemaType()->className));
        CPPUNIT_ASSERT(std::string(a->getArray(0)->getSchemaType()->className)=="xmlbeansxx::XmlInteger");
        CPPUNIT_ASSERT(std::string(a->getArray(1)->getSchemaType()->className)=="xmlbeansxx::XmlPositiveInteger");
        CPPUNIT_ASSERT(std::string(a->getArray(2)->getSchemaType()->className)=="xmlbeansxx::XmlInteger");
        */
    }
    {
        /*
        //digest
        XmlArray<XmlInteger> > a(new XmlArray<XmlInteger>()),b(new XmlArray<XmlInteger>);
        a->append(XmlInteger(XmlInteger::Factory::newInstance("101")));
        a->append(XmlPositiveInteger(XmlPositiveInteger::Factory::newInstance("102")));
        a->append(XmlInteger(XmlInteger::Factory::newInstance("103")));

        b->append(XmlInteger(XmlInteger::Factory::newInstance("101")));
        b->append(XmlPositiveInteger(XmlPositiveInteger::Factory::newInstance("102")));
        b->append(XmlInteger(XmlInteger::Factory::newInstance("103")));

        LOG4CXX_DEBUG(LOG,a->digest().getSimpleContent());
        LOG4CXX_DEBUG(LOG,b->digest().getSimpleContent());
        CPPUNIT_ASSERT(a->digest()==b->digest());
        b->append(XmlInteger(XmlInteger::Factory::newInstance("103")));
        CPPUNIT_ASSERT(a->digest()!=b->digest());
        */
    }
    {
        //Base64
        Array<unsigned char> a(3);
        a[0]='a';
        a[1]='b';
        a[2]='c';

        std::string s=TextUtils::base64Encode(a);
        LOG4CXX_DEBUG(LOG,"Base64 size:" + TextUtils::intToString(s.size()));
        LOG4CXX_DEBUG(LOG,"Base64:" + s);
        CPPUNIT_ASSERT(TextUtils::collapse(s) == std::string("YWJj"));

        XmlBase64Binary b=XmlBase64Binary::Factory::newInstance();
        b->setByteArrayValue(a);
        LOG4CXX_DEBUG(LOG,"Base64 xml:" + b->toString());
        CPPUNIT_ASSERT(b->getByteArrayValue()[0]=='a');
        CPPUNIT_ASSERT(b->getByteArrayValue()[1]=='b');
        CPPUNIT_ASSERT(b->getByteArrayValue()[2]=='c');
    }
    {
        //xmlbeansxx::Array
        Array<int> c = Array<int>();

        Array<int> b = Array<int>(3);
        b[0] = 10;
        b[1] = 11;
        b[2] = 12;

        Array<int> a(b);
        CPPUNIT_ASSERT(a[0] == 10);
        CPPUNIT_ASSERT(a[1] == 11);
        CPPUNIT_ASSERT(a[2] == 12);
    }
}

