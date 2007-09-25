#include <string>

#include <xmlbeansxx/logger.h>
#include <xmlbeansxx/xmlbeansxx.h>
#include "c.h"
#include <fstream>
#include <sstream>
#include <xmlbeansxx/LibXMLParser.h>

void namespaceTests();

//log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(std::string("mem01"));
LOGGER_PTR_SET(logger,"mem01");

int main() {
    try {
        namespaceTests();
    } catch (xmlbeansxx::BeansException e) {
        LOG4CXX_ERROR(logger,"Exception: "+std::string(e.getMessage()));
        throw e;
    }
	return 0;
}


using namespace xmlbeansxx;
using namespace com::p4::mind::mytest;
using namespace std;

double currentTime() {
    struct timeval tv;
    if (gettimeofday(&tv,NULL)!=0) throw IllegalStateException();
    return double(tv.tv_sec) + double(tv.tv_usec)/1000000.0;
}

std::string readXml(std::string fname) {
    std::string xml;
    {
        const int S=1025;
        char buf[S];
        FILE *f=fopen(fname.c_str(),"rb");

        if (!f) {
            cerr<<"File "<<fname<<" not found\n";
            return "";
        }
        while (!feof(f)) {
            int l=fread(buf,1,S-1,f);
            buf[l]=0;
            xml+=buf;
        }

        fclose(f);
    }
    return xml;
}

void namespaceTests() {
    XmlOptions opt;
//    opt.setValidation(true);
    LibXMLParser p(opt);
//    p.loadGrammar("c.xsd");
//    p.getXmlOptions()->setValidation(true);

	std::string xml = readXml("c.xml");
	
    do {
        ContentDocument doc=ContentDocument::Factory::newInstance();
        try {
            double t1 = currentTime();
	    istringstream in(xml);
            p.parse(xml,doc);
            double t2 = currentTime();
            cerr << "Duration: " << t2-t1 << " sec." << " " << 1./(t2-t1) << " per second" << endl;
        } catch (BeansException &ex) {
		    //LOG4CXX_DEBUG(logger,"Exception: " << ex.getMessage());
            //log4cxx::logstream s(logger, log4cxx::Level::DEBUG);
            //s << "xx";
            //log4cxx::logstream(logger, log4cxx::Level::DEBUG) << (ex.getMessage());
            
			throw;
        }
    } while(true);
}
