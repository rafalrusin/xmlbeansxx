
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
	LibXMLParser.cpp
	LibXMLParser.h
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
	
	XmlTypesGen.h
	XmlTypesGen.cpp
	xml-fragment.h
	xml-fragment.cpp

)

add_dependencies (xmlbeansxx generator)

add_library (xmlbeansxx SHARED ${XMLBEANSXX_SRC})
TARGET_LINK_LIBRARIES (xmlbeansxx ${XERCES_LIBRARY} ${BOOST_THREAD_LIBRARY} ${BOOST_DATE_TIME_LIBRARY} ${LOG4CXX_LIBRARY} ${GMP_LIBRARY} ${GMPXX_LIBRARY})




