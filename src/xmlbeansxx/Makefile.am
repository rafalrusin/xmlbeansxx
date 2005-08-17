SUBDIRS =
EXTRA_DIST = xml-fragment.xsd

AM_CPPFLAGS = -I../

lib_LTLIBRARIES = libxmlbeansxx.la
libxmlbeansxx_la_LDFLAGS = -version-info $(library_version_info)
libxmlbeansxx_la_SOURCES = \
	XmlTypesGen.xh\
	XmlTypesGen.xcpp\
	Query.h\
	Query.cpp\
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
	XercesUtils.cpp\
	XercesUtils.h\
	defs.h\
	xmlbeansxx.h\
	TypeSystem.cpp\
	BeansException.h\
	BeansException.cpp\
	XmlException.h\
	XmlOptions.h\
	XmlParser.cpp\
	XmlParser.h\
	XercesParser.cpp\
	XercesParser.h\
	macros.h\
	TextUtils.h\
	TextUtils.cpp\
	Contents.h\
	Contents.cpp\
	ContentsImpl.h\
	ContentsImpl.cpp\
	SimpleValue.h\
	SimpleValue.cpp\
	Dict.h\
	Dict.cpp\
	XmlArray.h\
	XmlArray.cpp\
	XmlContext.cpp\
	XmlContext.h\
	SchemaProperty.h\
	SchemaProperty.cpp\
	NullPtr.h

nodist_libxmlbeansxx_la_SOURCES = \
	XmlTypesGen.h\
	XmlTypesGen.cpp\
	xml-fragment.h\
	xml-fragment.cpp

BUILT_SOURCES = \
	XmlTypesGen.h\
	XmlTypesGen.cpp\
	xml-fragment.h\
	xml-fragment.cpp

libxmlbeansxxincludedir = $(includedir)/xmlbeansxx
libxmlbeansxxinclude_HEADERS = \
	BoostAssert.h\
	TypeSystem.h\
	CoreTypes.h\
	shared_array.h\
	SchemaType.h\
	XercesUtils.h\
	xmlbeansxx.h\
	BeansException.h\
	XmlException.h\
	XmlOptions.h\
	XmlParser.h\
	macros.h\
	TextUtils.h\
	Contents.h\
	Dict.h\
	XmlArray.h\
	Tracer.h\
	XmlContext.h\
	Query.h\
	SchemaProperty.h\
	StoreString.h\
	NullPtr.h

nodist_libxmlbeansxxinclude_HEADERS = \
	XmlTypesGen.h\
	xml-fragment.h

XmlTypesGen.h XmlTypesGen.cpp: ${top_srcdir}/generator/dist/bin/xmlbeansxx-genbuiltin XmlTypesGen.xh XmlTypesGen.xcpp
	${top_builddir}/generator/dist/bin/xmlbeansxx-genbuiltin

xml-fragment.h xml-fragment.cpp: ${top_srcdir}/generator/dist/bin/xmlbeansxx-gen xml-fragment.xsd
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen ${srcdir}/xml-fragment.xsd

MOSTLYCLEANFILES=XmlTypesGen.h XmlTypesGen.cpp xml-fragment.h xml-fragment.cpp

