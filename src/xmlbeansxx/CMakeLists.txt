
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
	XercesParser.cpp
	XercesUtils.cpp
	
	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h
	${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.cpp
	${CMAKE_CURRENT_BINARY_DIR}/xml-fragment.h
	${CMAKE_CURRENT_BINARY_DIR}/xml-fragment.cpp

	${CMAKE_CURRENT_BINARY_DIR}/config_auto.h
)

include_directories(
			${CMAKE_CURRENT_BINARY_DIR} 
			${CMAKE_CURRENT_SOURCE_DIR}
			${CMAKE_CURRENT_BINARY_DIR}/..
			${CMAKE_CURRENT_SOURCE_DIR}/..
		)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/config_auto.h
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/config.h.default ${CMAKE_CURRENT_BINARY_DIR}/config_auto.h
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.h ${CMAKE_CURRENT_BINARY_DIR}/XmlTypesGen.cpp
	COMMAND ${PROJECT_BINARY_DIR}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-genbuiltin${EXEC_SUF} ${CMAKE_CURRENT_SOURCE_DIR}/
	DEPENDS generator
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/xml-fragment.h ${CMAKE_CURRENT_BINARY_DIR}/xml-fragment.cpp
	COMMAND ${PROJECT_BINARY_DIR}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen${EXEC_SUF} ${CMAKE_CURRENT_SOURCE_DIR}/xml-fragment.xsd
	DEPENDS generator
)


add_library (xmlbeansxx SHARED ${XMLBEANSXX_SRC})
TARGET_LINK_LIBRARIES (xmlbeansxx ${XERCES_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${LOG4CXX_LIBRARY} ${GMP_LIBRARY} ${GMPXX_LIBRARY})





