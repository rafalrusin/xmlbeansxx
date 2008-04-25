SUBDIRS =
EXTRA_DIST = xml-fragment.xsd CMakeLists.txt

AM_CPPFLAGS = -I${top_srcdir}/src -I${top_builddir}/src

lib_LTLIBRARIES = libxmlbeansxx.la
libxmlbeansxx_la_LDFLAGS = -version-info $(library_version_info)
libxmlbeansxx_la_SOURCES = \
	Map.cpp\
	XmlCursor.cpp\
	XmlCursor.h\
	configGmpxx.h.yes\
	configGmpxx.h.no\
	configLibxml2.h.yes\
	configLibxml2.h.no\
	configLog4cxx.h.yes\
	configLog4cxx.h.no\
	CoreTypes.cpp\
	XmlTypesGen.xh\
	XmlTypesGen.xcpp\
	BoostAssert.h\
	BoostAssert.cpp\
	StoreString.h\
	StoreString.cpp\
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
	QName.cpp\
	Query.cpp\
	Query.h\
	LibXMLParser.cpp\
	LibXMLParser.h\
	XmlParser.cpp\
	XmlParser.h\
	macros.h\
	TextUtils.h\
	TextUtils.cpp\
	ContentsImpl.h\
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
	NSMap.cpp\
	Calendar.h\
	Calendar.cpp

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
	configGmpxx.h\
	configLibxml2.h\
	configLog4cxx.h

libxmlbeansxxincludedir = $(includedir)/xmlbeansxx
libxmlbeansxxinclude_HEADERS = \
	XmlCursor.h\
	CoreTypes.h\
	config.h\
	XmlObjectDocument.h\
	BoostAssert.h\
	TypeSystem.h\
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
	XercesUtils.h\
	Calendar.h

nodist_libxmlbeansxxinclude_HEADERS = \
	XmlTypesGen.h\
	xml-fragment.h\
	configGmpxx.h\
	configLibxml2.h\
	configLog4cxx.h

XmlTypesGen.h XmlTypesGen.cpp: ${top_builddir}/generator/stamp-build  ${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-genbuiltin XmlTypesGen.xh XmlTypesGen.xcpp 
	${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-genbuiltin ${srcdir}/

xml-fragment.h xml-fragment.cpp: ${top_builddir}/generator/stamp-build ${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen xml-fragment.xsd
	${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen ${srcdir}/xml-fragment.xsd


configLog4cxx.h: ${srcdir}/configLog4cxx.h.${enable_log4cxx}
	cp ${srcdir}/configLog4cxx.h.${enable_log4cxx} configLog4cxx.h

configLibxml2.h: ${srcdir}/configLibxml2.h.${enable_libxml2}
	cp ${srcdir}/configLibxml2.h.${enable_libxml2} configLibxml2.h

configGmpxx.h: ${srcdir}/configGmpxx.h.${enable_gmpxx}
	cp ${srcdir}/configGmpxx.h.${enable_gmpxx} configGmpxx.h


MOSTLYCLEANFILES=XmlTypesGen.h XmlTypesGen.cpp xml-fragment.h xml-fragment.cpp configLog4cxx.h configLibxml2.h configGmpxx.h

