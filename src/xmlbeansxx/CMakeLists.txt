



XMLBEANSXX_GEN(xml-fragment.xsd xml-fragment.cpp xml-fragment.h)


#make a config file
#XMLBEANSXX_COPY(config_auto.h.default config_auto.h)
SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS "")
SET(XMLBEANSXX_OPTIONAL_LIBS "")

IF (LIBXML2_FOUND) 
	XMLBEANSXX_COPY(configLibxml2.h.yes configLibxml2.h)
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS} ${LIBXML2_LIBRARIES})
	SET(PARSERS_H   LibXMLParser.h   XercesParser.h)
	SET(PARSERS_CPP LibXMLParser.cpp XercesParser.cpp)
ELSE (LIBXML2_FOUND) 
	XMLBEANSXX_COPY(configLibxml2.h.no configLibxml2.h)
	SET(PARSERS_H   XercesParser.h)
	SET(PARSERS_CPP XercesParser.cpp)
ENDIF (LIBXML2_FOUND) 


IF (GMPXX_FOUND)
	XMLBEANSXX_COPY(configGmpxx.h.yes  configGmpxx.h)
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS}  ${GMP_LIBRARY} ${GMPXX_LIBRARY})
ELSE (GMPXX_FOUND)
	XMLBEANSXX_COPY(configGmpxx.h.no  configGmpxx.h)
ENDIF (GMPXX_FOUND)


IF (LIBXML2_FOUND)
	XMLBEANSXX_COPY(configLog4cxx.h.yes configLog4cxx.h)
	SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS ${XMLBEANSXX_EXTRA_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS}  ${LOG4CXX_LIBRARY})
ELSE (LIBXML2_FOUND)
	XMLBEANSXX_COPY(configLog4cxx.h.no  configLog4cxx.h)
ENDIF (LIBXML2_FOUND)



SET(XMLBEANSXX_INSTALL_HEADERS 
        XmlObjectDocument.h
        BoostAssert.h
        TypeSystem.h
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
	${CoreTypes.h_FILE}
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
	NSMap.cpp
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
	${XMLBEANSXX_EXTRA_INCLUDE_DIRS}
)


#generate buildin system types 
ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h ${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.cpp
	COMMAND ${XMLBEANSXX_GENBUILDIN_PROG} ${CMAKE_CURRENT_SOURCE_DIR}/
	DEPENDS generator
)


#ADD_LIBRARY (${XMLBEANSXX_LIB} SHARED ${XMLBEANSXX_SRC})
ADD_LIBRARY (${XMLBEANSXX_LIB} STATIC ${XMLBEANSXX_SRC})
TARGET_LINK_LIBRARIES (${XMLBEANSXX_LIB} ${XERCES_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${BOOST_THREAD_LIBRARY} ${XMLBEANSXX_OPTIONAL_LIBS})
SET_TARGET_PROPERTIES (${XMLBEANSXX_LIB} PROPERTIES SOVERSION ${XMLBEANSXX_SOVERSION})

SET(INCLUDE_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/include")
SET(XMLBEANSXX_INCLUDE_INSTALL_DIR  "${INCLUDE_INSTALL_DIR}/xmlbeansxx")

INSTALL(FILES
	${XMLBEANSXX_INSTALL_HEADERS}
	DESTINATION include/xmlbeansxx
)

INSTALL(TARGETS ${XMLBEANSXX_LIB}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)




