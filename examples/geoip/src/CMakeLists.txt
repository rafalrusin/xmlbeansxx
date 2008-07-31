XMLBEANSXX_GEN(geoip_xsd.xsd geoip_xsd.cpp geoip_xsd.h)
XMLBEANSXX_GEN(soapEnvelope.xsd soapEnvelope.cpp soapEnvelope.h)

add_executable(geoip 
        geoip.cpp
        ${geoip_xsd.xsd_SRC}
        ${soapEnvelope.xsd_SRC}
        )
target_link_libraries(geoip ${BOOST_DATE_TIME_LIBRARY} ${BOOST_REGEX_LIBRARY} ${LOG4CXX_LIBRARY})

