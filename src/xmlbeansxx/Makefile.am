SUBDIRS =
EXTRA_DIST = xml-fragment.xsd

AM_CPPFLAGS = -I${top_srcdir}/src -I${top_builddir}/src

lib_LTLIBRARIES = libxmlbeansxx.la
libxmlbeansxx_la_LDFLAGS = -version-info $(library_version_info)
libxmlbeansxx_la_SOURCES = \
	XmlTypesGen.xh\
	XmlTypesGen.xcpp\
	BoostAssert.h\
	BoostAssert.cpp\
	StoreString.h\
	StoreString.cpp\
	CoreTypes.h\
	CoreTypes.cpp\
	TypeSystem.h\
	shared_array.h\
	SchemaType.h\
	SchemaType.cpp\
	Tracer.h\
	defs.h\
	geum_types.h\
	xmlbeansxx.h\
	TypeSystem.cpp\
	Walker.cpp\
	Serialize.cpp\
	BeansException.h\
	BeansException.cpp\
	XmlBeans.cpp\
	XmlException.h\
	XmlOptions.h\
	QName.h\
	Query.cpp\
	Query.h\
	LibXMLParser.cpp\
	LibXMLParser.h\
	XmlParser.cpp\
	XmlParser.h\
	macros.h\
	TextUtils.h\
	TextUtils.cpp\
	Contents.h\
	Contents.cpp\
	Dict.h\
	Dict.cpp\
	XmlArray.h\
	XmlArray.cpp\
	XmlContext.cpp\
	XmlContext.h\
	SchemaProperty.h\
	SchemaProperty.cpp\
	NullPtr.h\
	XmlObjectDocument.h\
	XmlObjectDocument.cpp\
	XmlObjectXpaths.cpp\
	NSMap.h\
	config_auto.h\
	XercesParser.cpp\
	XercesUtils.cpp\
	NSMap.cpp

nodist_libxmlbeansxx_la_SOURCES = \
	XmlTypesGen.h\
	XmlTypesGen.cpp\
	xml-fragment.h\
	xml-fragment.cpp

BUILT_SOURCES = \
	XmlTypesGen.h\
	XmlTypesGen.cpp\
	xml-fragment.h\
	xml-fragment.cpp\
	XercesUtils.cpp

libxmlbeansxxincludedir = $(includedir)/xmlbeansxx
libxmlbeansxxinclude_HEADERS = \
	XmlObjectDocument.h\
	BoostAssert.h\
	TypeSystem.h\
	CoreTypes.h\
	shared_array.h\
	SchemaType.h\
	geum_types.h\
	xmlbeansxx.h\
	BeansException.h\
	XmlException.h\
	XmlOptions.h\
	Map.h\
	QName.h\
	LibXMLParser.h\
	XmlParser.h\
	XmlBeans.h\
	macros.h\
	TextUtils.h\
	Contents.h\
	Dict.h\
	XmlArray.h\
	Tracer.h\
	XmlContext.h\
	logger.h\
	Query.h\
	SchemaProperty.h\
	StoreString.h\
	NullPtr.h\
	NSMap.h\
	XercesUtils.h\
	XercesParser.h\
	XercesUtils.h

nodist_libxmlbeansxxinclude_HEADERS = \
	XmlTypesGen.h\
	xml-fragment.h

XmlTypesGen.h XmlTypesGen.cpp:  ${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-genbuiltin XmlTypesGen.xh XmlTypesGen.xcpp 
	${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-genbuiltin ${srcdir}/

xml-fragment.h xml-fragment.cpp: ${top_builddir}/generator/stamp-build ${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen xml-fragment.xsd
	${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen ${srcdir}/xml-fragment.xsd

MOSTLYCLEANFILES=XmlTypesGen.h XmlTypesGen.cpp xml-fragment.h xml-fragment.cpp

