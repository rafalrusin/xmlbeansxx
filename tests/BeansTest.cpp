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
        XmlStringPtr sx(XmlString::Factory::newInstance(XmlAnySimpleType::Factory::newInstance("xxx")));
        CPPUNIT_ASSERT(sx->getStringValue() == "xxx");

        XmlBooleanPtr bx(XmlBoolean::Factory::newInstance(std::string("0")));
        XmlStringPtr s1(XmlString::Factory::newInstance("a")),s2(XmlString::Factory::newInstance("a"));
        CPPUNIT_ASSERT(sx->getStringValue() == "xxx");
        CPPUNIT_ASSERT(s1->getStringValue() == s2->getStringValue());
        //cout<<(s1==s2)<<"\n";

        XmlStringPtr s3(XmlString::Factory::newInstance("a")),s4(XmlString::Factory::newInstance("b")),s5(XmlString::Factory::newInstance(s3));
        CPPUNIT_ASSERT(s3->getStringValue() != s4->getStringValue());
        CPPUNIT_ASSERT(s3->getStringValue() == s3->getStringValue());
        //cout<<"s5:"<<s5.toString()<<"\n";

        boost::shared_ptr<XmlDecimal> a(XmlDecimal::Factory::newInstance());
        a->setIntValue(-12);
        boost::shared_ptr<XmlDecimal> b(XmlDecimal::Factory::newInstance());
        b->setIntValue(15);
        boost::shared_ptr<XmlDecimal> c(XmlDecimal::Factory::newInstance());
        c->setIntValue(a->getIntValue() + b->getIntValue());

        boost::shared_ptr<XmlObject> l=globalTypeSystem()->createByName("integer",globalTypeSystem()->getNamespace("http://www.w3.org/2001/XMLSchema"));
        //cout<<l->xsdTypeName()<<":"<<l->getSimpleContent()<<"\n"<<l->toString()<<"\n";


        //testing fundamental types
        XmlBooleanPtr boolean1(XmlBoolean::Factory::newInstance("false")),boolean2(XmlBoolean::Factory::newInstance("true"));
        CPPUNIT_ASSERT(boolean1->getBooleanValue()==false);
        CPPUNIT_ASSERT(boolean2->getBooleanValue()==true);

        XmlObjectPtr obj=XmlObjectPtr(XmlPositiveInteger::Factory::newInstance("10"));
        XmlIntegerPtr obj2=java_cast<XmlInteger>(obj);
        logger->debug("obj2: "+obj2->getSimpleContent());
        CPPUNIT_ASSERT(obj2->getSimpleContent()=="10");
        //XmlDecimalPtr obj3=java_cast<XmlDecimal>(obj);

    }
    {
        //XmlInteger
        CPPUNIT_ASSERT(((int)(*XmlInteger::Factory::newInstance("125")))==125);
        /*
        CPPUNIT_ASSERT((XmlInteger("-3")+XmlInteger("-5"))==XmlInteger("-8"));
        CPPUNIT_ASSERT((XmlDecimal("-3.5")+XmlDecimal("-5.7"))==XmlDecimal("-9.2"));
        */
        
        XmlIntegerPtr a(XmlInteger::Factory::newInstance("10")),b(XmlInteger::Factory::newInstance("11"));
        a->setIntValue(a->getIntValue() + b->getIntValue());
        CPPUNIT_ASSERT(a->getIntValue() == 21);
        int x=21;
        CPPUNIT_ASSERT(x==a->getMpzValue());

        try {
            XmlIntegerPtr c(XmlInteger::Factory::newInstance("10")),d(XmlInteger::Factory::newInstance("abc"));
            CPPUNIT_ASSERT(d->getIntValue() == 10);
            CPPUNIT_ASSERT(false);
        } catch (BeansException e) {
        }
    }

    {
        //XmlString
        XmlStringPtr s(XmlString::Factory::newInstance("abcdefghijk"));
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
        XmlDateTimePtr a(XmlDateTime::Factory::newInstance(" 2004-01-12T23:50:22 \n\n\n"));
        XmlDateTimePtr b(XmlDateTime::Factory::newInstance("   2004-01-12T23:50:22 \n"));
        XmlDateTimePtr c(XmlDateTime::Factory::newInstance("   2004-01-12T23:50:24 \n"));
        XmlDateTimePtr d(XmlDateTime::Factory::newInstance("   2004-01-11T23:50:24 \n"));
        XmlDateTimePtr e(XmlDateTime::Factory::newInstance(XmlAnySimpleType::Factory::newInstance(" 2004-01-11T23:50:24 ")));
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
        XmlDatePtr a(XmlDate::Factory::newInstance("   2001-03-24"));
        boost::gregorian::date d=a->getDateValue();
        CPPUNIT_ASSERT(d == boost::gregorian::from_string("2001-3-24"));
        LOG4CXX_DEBUG(logger,a->daysFrom(XmlDate::Factory::newInstance(" 2001-01-24\n")));
        CPPUNIT_ASSERT(a->daysFrom(XmlDate::Factory::newInstance(" 2001-01-24\n")) == 59);
        CPPUNIT_ASSERT(XmlDate::Factory::newInstance("2003-01-01")->daysFrom(XmlDate::Factory::newInstance(" 2002-12-30\n")) == 2);

    }

    {
        //serialization/parsing
        XmlPositiveIntegerPtr x(XmlPositiveInteger::Factory::newInstance("101")),y;
        LOG4CXX_DEBUG(logger,x->toString());
        y=XmlPositiveInteger::Factory::parse(x->toString());
        CPPUNIT_ASSERT(x->getSimpleContent()==y->getSimpleContent());

        boost::shared_ptr<XmlArray<XmlPositiveInteger> > ar(new XmlArray<XmlPositiveInteger>()),ar2;
        ar->setArray(0,XmlPositiveIntegerPtr(XmlPositiveInteger::Factory::newInstance("102")));
        ar->setArray(1,XmlPositiveIntegerPtr(XmlPositiveInteger::Factory::newInstance("103")));
        ar->setArray(2,XmlPositiveIntegerPtr(XmlPositiveInteger::Factory::newInstance("104")));
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
        a->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("101")));
        a->append(XmlPositiveIntegerPtr(XmlPositiveInteger::Factory::newInstance("102")));
        a->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("103")));
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
        a->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("101")));
        a->append(XmlPositiveIntegerPtr(XmlPositiveInteger::Factory::newInstance("102")));
        a->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("103")));

        b->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("101")));
        b->append(XmlPositiveIntegerPtr(XmlPositiveInteger::Factory::newInstance("102")));
        b->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("103")));

        LOG4CXX_DEBUG(logger,a->digest().getSimpleContent());
        LOG4CXX_DEBUG(logger,b->digest().getSimpleContent());
        CPPUNIT_ASSERT(a->digest()==b->digest());
        b->append(XmlIntegerPtr(XmlInteger::Factory::newInstance("103")));
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

