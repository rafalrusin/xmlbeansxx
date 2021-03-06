



XMLBEANSXX_GEN(xml-fragment.xsd xml-fragment.cpp xml-fragment.h)


SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS "")
SET(XMLBEANSXX_OPTIONAL_LIBS "")

IF (LIBXML2_FOUND) 
	XMLBEANSXX_COPY(configLibxml2.h.yes configLibxml2.h)
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS} ${LIBXML2_LIBRARIES})
	SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS ${XMLBEANSXX_EXTRA_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
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


IF (LOG4CXX_FOUND)
	XMLBEANSXX_COPY(configLog4cxx.h.yes configLog4cxx.h)
	SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS ${XMLBEANSXX_EXTRA_INCLUDE_DIRS} ${LOG4CXX_INCLUDE_DIR})
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS}  ${LOG4CXX_LIBRARY})
ELSE (LOG4CXX_FOUND)
	XMLBEANSXX_COPY(configLog4cxx.h.no  configLog4cxx.h)
ENDIF (LOG4CXX_FOUND)



SET(XMLBEANSXX_INSTALL_HEADERS 
	XmlCursor.h
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
	CoreTypes.h
	config.h
	Calendar.h

	${PARSERS_H}

	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h	
	${xml-fragment.xsd_HEADER}
	
	${configLibxml2.h_FILE}
	${configGmpxx.h_FILE}
	${configLog4cxx.h_FILE}
)



SET(XMLBEANSXX_SRC 
	XmlCursor.cpp
	XmlCursor.h
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
	BeansException.h
	BeansException.cpp
	XmlBeans.cpp
	XmlException.h
	XmlOptions.h
	QName.h
	QName.cpp
	Query.cpp
	Query.h
	XmlParser.cpp
	XmlParser.h
	macros.h
	TextUtils.h
	TextUtils.cpp
	ContentsImpl.h
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
	Calendar.cpp
	
	${PARSERS_CPP}
	
	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h
	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.cpp
	
	${xml-fragment.xsd_SRC}

	${XMLBEANSXX_INSTALL_HEADERS}

	Serialize.cpp
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

#rr: removed library deps, deps for static libs should be set up during exe linking
#TARGET_LINK_LIBRARIES (${XMLBEANSXX_LIB} ${XERCES_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${BOOST_THREAD_LIBRARY} ${XMLBEANSXX_OPTIONAL_LIBS})

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




