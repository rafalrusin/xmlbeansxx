
# generate code from files: geoip_xsd.xsd, soapEnvelope.xsd
XMLBEANSXX_GEN(geoip_xsd.xsd geoip_xsd.cpp geoip_xsd.h)
XMLBEANSXX_GEN(soapEnvelope.xsd soapEnvelope.cpp soapEnvelope.h)


INSTALL(FILES log4j.xml DESTINATION "bin")

add_executable(geoip
        geoip.cpp
        HttpClient.cpp
        WsClient.cpp
        Exception.cpp
#add the generated files to owher program
        ${geoip_xsd.xsd_SRC}
        ${soapEnvelope.xsd_SRC}
        )

#link owher program with some libraries
target_link_libraries(geoip ${XERCES_LIBRARY} ${XMLBEANSXX_LIBRARY} ${CURL_LIBRARY} ${BOOST_THREAD_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${LOG4CXX_LIBRARY})
INSTALL(TARGETS geoip DESTINATION "bin")
