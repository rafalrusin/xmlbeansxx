#include <string>

#include <xmlbeansxx/logger.h>
//#include <log4cxx/config.h>
#if NNN
#include "log4cxx/stream.h"
#endif

LOGGER_PTR_SET(logger,"mem01");

void tests() {
    //log4cxx::logstream LOG(logger, log4cxx::Level::DEBUG);
    //
    //LOG << "abc" << 15 << std::endl << LOG4CXX_ENDMSG;
    //std::wstring s;
    //LOG << s;
    //LOG << "abc" << 15 << LOG4CXX_ENDMSG;
}

void namespaceTests();

#include <xmlbeansxx/xmlbeansxx.h>

int main() {
    try {
        tests();
        namespaceTests();
    } catch (xmlbeansxx::BeansException e) {
        LOG4CXX_ERROR(logger,"Exception: "+std::string(e.getMessage()));
        throw e;
    }
	return 0;

}


using namespace xmlbeansxx;


void namespaceTests() {
    do {
        //Base64
        shared_array<unsigned char> a(3);
        a[0]='a';
        a[1]='b';
        a[2]='c';

        std::string s=TextUtils::base64Encode(a);
        LOG4CXX_DEBUG(logger,"Base64 size:" + TextUtils::intToString(s.size()));
        LOG4CXX_DEBUG(logger,"Base64:" + s);

        XmlBase64Binary b=XmlBase64Binary::Factory::newInstance();
        b.setByteArrayValue(a);
        LOG4CXX_DEBUG(logger,"Base64 xml:" + b.toString());
		
		shared_array<unsigned char> c = b.getByteArrayValue();
		LOG4CXX_DEBUG(logger, "tu");
#if NNN
        logstream LOG(logger, Level::DEBUG);
        //LOG << "abc" << 15 << LOG4CXX_ENDMSG;
#endif
    } while(true);
    
}
