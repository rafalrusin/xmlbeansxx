#include "EntitiesTest.h"
#include "entities.h"
#include "../src/xmlbeansxx/BeansException.h"

#include <fstream>
#include <string>
#include <sstream>
#include <xmlbeansxx/logger.h>

CPPUNIT_TEST_SUITE_REGISTRATION (EntitiesTest);

using namespace std;
using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;

//log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger (string("test.EntitiesTest"));
XMLBEANSXX_LOGGER_PTR_SET(LOG,"test.EntitiesTest");

void EntitiesTest::entitiesTest () {

    try {
        ifstream ifs ("entities.xml");
        
        stringstream ss1;
        ss1 << ifs.rdbuf();
        XMLBEANSXX_INFO(LOG,"original: " + ss1.str());
        
        NodesDocument doc = NodesDocument::Factory::parse (ss1);
        XMLBEANSXX_INFO(LOG,"data: " + doc.getNodes().getNodeArray(0).getData());
        doc.getNodes().getNodeArray(0).setValue("opi\"sik");
            
        stringstream ss2;
        ss2 << doc.toString();
        XMLBEANSXX_INFO(LOG,"after first parsing: \n" + ss2.str());
        
        NodesDocument doc2 = NodesDocument::Factory::parse (ss2);
        XMLBEANSXX_INFO(LOG,"data: " + doc2.getNodes().getNodeArray(0).getData());

        stringstream ss3;
        ss3 << doc.toString();
        XMLBEANSXX_INFO(LOG,"after second parsing: " + ss3.str());
        
        CPPUNIT_ASSERT_EQUAL (ss2.str(), ss3.str());
        CPPUNIT_ASSERT ( !doc2.getNodes().getNodeArray(0).getData().empty());
        
    } catch (XmlException& e) {
        cerr << "XmlException: " << e.what () << endl;
        throw;
    } catch (BeansException& e) {
        cerr << "BeansException: " << e.what () << endl;
        throw;
    } catch (exception& e) {
        cerr << "exception: " << e.what () << endl;
        throw;
    }
}
