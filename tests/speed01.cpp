#include <log4cxx/logger.h>
#include "c.h"
#include <fstream>
#include <sstream>
#include <xmlbeansxx/XmlParser.h>

using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;

log4cxx::LoggerPtr LOG = log4cxx::Logger::getLogger("speed01");

double currentTime() {
    struct timeval tv;
    if (gettimeofday(&tv,NULL) != 0) throw IllegalStateException();
    return double(tv.tv_sec) + double(tv.tv_usec) / 1000000.0;
}

int main() {
    //Parsing speed test
    try {
        XmlParser parser(XmlParser::create());
        parser->loadGrammar("c.xsd");
        parser->getXmlOptions()->setValidation(true);
    
        while (true) {
            ContentDocument doc = ContentDocument::Factory::newInstance();
            double t1 = currentTime();
            ifstream in("d.xml");
            parser->parse(in, doc);
            int car = cast<Fullpersoninfo>(doc->getContent()->getEmployee())->getCar();
            string xml = doc->toString();
            double t2 = currentTime();
            LOG4CXX_INFO(LOG, "Duration: " << t2-t1 << " sec." << " " << 1. / (t2-t1) << " per second");
            LOG4CXX_DEBUG(LOG, "Car: " << car);
            LOG4CXX_DEBUG(LOG, "Xml: " << xml);
        }
    } catch (xmlbeansxx::BeansException &e) {
        LOG4CXX_ERROR(LOG, "Exception: " + e.getMessage());
        throw e;
    }
    return 0;
}

