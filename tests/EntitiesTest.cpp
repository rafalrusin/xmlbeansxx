#include "EntitiesTest.h"
#include "entities.h"
#include "../src/xmlbeansxx/BeansException.h"

#include <fstream>
#include <sstream>
#include <string>
#include <log4cxx/logger.h>

CPPUNIT_TEST_SUITE_REGISTRATION (EntitiesTest);

using namespace std;
using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;

log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger (xmlbeansxx::String("test.EntitiesTest"));

void EntitiesTest::entitiesTest () {

    try {
    	ifstream ifs ("entities.xml");

	stringstream ss1;
	ss1 << ifs.rdbuf ();
	LOG4CXX_INFO (LOG, "oryginal:\n" << ss1.str ());
	
	NodesDocument doc = NodesDocument::Factory::parse (ss1);
	LOG4CXX_INFO (LOG, "after first parsing:\n" + doc->toString ());

	stringstream ss2;
	ss2 << doc->toString ();
	NodesDocument doc2 = NodesDocument::Factory::parse (ss2);
	LOG4CXX_INFO (LOG, "after second parsing:\n" + doc2->toString ());
	
	stringstream ss3;
	ss3 << doc2->toString ();
	NodesDocument doc3 = NodesDocument::Factory::parse (ss3);
	LOG4CXX_INFO (LOG, "after third parsing:\n" + doc3->toString ());

	CPPUNIT_ASSERT_EQUAL (doc2->toString (), doc3->toString ());
   
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
