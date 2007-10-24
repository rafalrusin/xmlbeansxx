



XMLBEANSXX_GEN(xml-fragment.xsd xml-fragment.cpp xml-fragment.h)

IF (WIN32)
	SET(PARSERS_H   XercesParser.h)
	SET(PARSERS_CPP XercesParser.cpp)
ELSE (WIN32)
	SET(PARSERS_H   LibXMLParser.h   XercesParser.h)
	SET(PARSERS_CPP LibXMLParser.cpp XercesParser.cpp)
ENDIF (WIN32)


SET(XMLBEANSXX_INSTALL_HEADERS 
        XmlObjectDocument.h
        BoostAssert.h
        TypeSystem.h
        CoreTypes.h
        shared_array.h
        SchemaType.h
        geum_types.h
        xmlbeansxx.h
        BeansException.h
        XmlException.h
        XmlOptions.h
        Map.h
        QName.h
        XmlParser.h
        XmlBeans.h
        macros.h
        TextUtils.h
        Contents.h
        Dict.h
        XmlArray.h
        Tracer.h
        XmlContext.h
        logger.h
        Query.h
        SchemaProperty.h
        StoreString.h
        NullPtr.h
        NSMap.h
        XercesUtils.h
        XercesUtils.h

	${PARSERS_H}

	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h	
	${xml-fragment.xsd_HEADER}
)



SET(XMLBEANSXX_SRC 
	XmlTypesGen.xh
	XmlTypesGen.xcpp
	BoostAssert.h
	BoostAssert.cpp
	StoreString.h
	StoreString.cpp
	CoreTypes.h
	CoreTypes.cpp
	TypeSystem.h
	shared_array.h
	SchemaType.h
	SchemaType.cpp
	Tracer.h
	defs.h
	geum_types.h
	xmlbeansxx.h
	TypeSystem.cpp
	Walker.cpp
	Serialize.cpp
	BeansException.h
	BeansException.cpp
	XmlBeans.cpp
	XmlException.h
	XmlOptions.h
	QName.h
	Query.cpp
	Query.h
	XmlParser.cpp
	XmlParser.h
	macros.h
	TextUtils.h
	TextUtils.cpp
	Contents.h
	Contents.cpp
	Dict.h
	Dict.cpp
	XmlArray.h
	XmlArray.cpp
	XmlContext.cpp
	XmlContext.h
	SchemaProperty.h
	SchemaProperty.cpp
	NullPtr.h
	XmlObjectDocument.h
	XmlObjectDocument.cpp
	XmlObjectXpaths.cpp
	NSMap.h
	XercesUtils.cpp
	
	${PARSERS_CPP}
	
	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h
	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.cpp
	${CMAKE_CURRENT_BINARY_DIR}/config_auto.h
	
	${xml-fragment.xsd_SRC}

	${XMLBEANSXX_INSTALL_HEADERS}
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

#make a config file
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/config_auto.h
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/config.h.default ${CMAKE_CURRENT_BINARY_DIR}/config_auto.h
)


#generate buildin system types 
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h ${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.cpp
	COMMAND ${XMLBEANSXX_GENBUILDIN_PROG} ${CMAKE_CURRENT_SOURCE_DIR}/
	DEPENDS generator
)


ADD_LIBRARY (xmlbeansxx STATIC ${XMLBEANSXX_SRC})
TARGET_LINK_LIBRARIES (xmlbeansxx ${XERCES_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${LOG4CXX_LIBRARY} ${GMP_LIBRARY} ${GMPXX_LIBRARY} ${BOOST_THREAD_LIBRARY})
SET_TARGET_PROPERTIES (xmlbeansxx PROPERTIES VERSION ${XMLBEANSXX_VERSION} SOVERSION ${XMLBEANSXX_SOVERSION} )

SET(INCLUDE_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/include")
SET(XMLBEANSXX_INCLUDE_INSTALL_DIR  "${INCLUDE_INSTALL_DIR}/xmlbeansxx")

INSTALL(FILES
	${XMLBEANSXX_INSTALL_HEADERS}
	DESTINATION ${XMLBEANSXX_INCLUDE_INSTALL_DIR}
)




