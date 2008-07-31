XMLBEANSXX_GEN(geoip_xsd.xsd geoip_xsd.cpp geoip_xsd.h)
XMLBEANSXX_GEN(soapEnvelope.xsd soapEnvelope.cpp soapEnvelope.h)

add_executable(geoip 
        geoip.cpp
        HttpClient.cpp
        WsClient.cpp
        Exception.cpp
        ${geoip_xsd.xsd_SRC}
        ${soapEnvelope.xsd_SRC}
        )
target_link_libraries(geoip ${XERCES_LIBRARY} ${XMLBEANSXX_LIBRARY} ${CURL_LIBRARY} ${BOOST_THREAD_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${LOG4CXX_LIBRARY})
INSTALL(TARGETS geoip DESTINATION "")
