SUBDIRS = 
EXTRA_DIST = xml-fragment.xsd xmlbeansxx-schema.xsd XmlTypesGen.xh XmlTypesGen.xcpp xmlbeansxx-schema.xh xmlbeansxx-schema.xcpp

CXXFLAGS += -I${srcdir}/../

lib_LTLIBRARIES = libxmlbeansxx.la
libxmlbeansxx_la_LDFLAGS = -version-info $(library_version_info)
libxmlbeansxx_la_SOURCES = \
	Array.h\
	BeansException.h\
	BoostAssert.h\
	cast.h\
	CoreTypes.h\
	CXXBooleanHolder.h\
	CXXDecimalHolder.h\
	CXXDoubleHolder.h\
	CXXFloatHolder.h\
	CXXIntegerHolder.h\
	CXXIntHolder.h\
	CXXLongHolder.h\
	CXXStringHolder.h\
	defs.h\
	Existence.h\
	LibXMLParser.h\
	Map.h\
	Mutex.h\
	MutexPool.h\
	Null.h\
	Pair.h\
	Ptr.h\
	QName.h\
	SchemaProperty.h\
	SchemaPropertyImpl.h\
	SchemaType.h\
	SchemaTypeImpl.h\
	SchemaTypeLoaderBase.h\
	SchemaTypeLoader.h\
	SchemaTypeLoaderImpl.h\
	Set.h\
	SimpleValue.h\
	Stack.h\
	StoreString.h\
	String.h\
	TextUtils.h\
	Tracer.h\
	TypeStore.h\
	TypeStoreImpl.h\
	TypeStoreUser.h\
	TypeStoreVisitor.h\
	ValidationContext.h\
	Vector.h\
	wml_types.h\
	XercesParser.h\
	XercesUtils.h\
	XmlArray.h\
	XmlBeans.h\
	xmlbeansxx.h\
	XmlComplexContentImpl.h\
	XmlContext.h\
	XmlCursor.h\
	XmlErrorCodes.h\
	XmlException.h\
	XmlObjectBase.h\
	XmlOptions.h\
	XmlParser.h\
	XmlTokenSource.h\
	BeansException.cpp\
	BoostAssert.cpp\
	CoreTypes.cpp\
	CXXBooleanHolder.cpp\
	CXXDecimalHolder.cpp\
	CXXDoubleHolder.cpp\
	CXXFloatHolder.cpp\
	CXXIntegerHolder.cpp\
	CXXIntHolder.cpp\
	CXXLongHolder.cpp\
	CXXStringHolder.cpp\
	Existence.cpp\
	LibXMLParser.cpp\
	Map.cpp\
	Mutex.cpp\
	MutexPool.cpp\
	Null.cpp\
	Pair.cpp\
	Ptr.cpp\
	QName.cpp\
	SchemaProperty.cpp\
	SchemaPropertyImpl.cpp\
	SchemaType.cpp\
	SchemaTypeImpl.cpp\
	SchemaTypeLoaderBase.cpp\
	SchemaTypeLoader.cpp\
	SchemaTypeLoaderImpl.cpp\
	Set.cpp\
	SimpleValue.cpp\
	Stack.cpp\
	StoreString.cpp\
	String.cpp\
	TextUtils.cpp\
	TypeStore.cpp\
	TypeStoreImpl.cpp\
	TypeStoreUser.cpp\
	TypeStoreVisitor.cpp\
	ValidationContext.cpp\
	Vector.cpp\
	XercesParser.cpp\
	XercesUtils.cpp\
	XmlArray.cpp\
	XmlBeans.cpp\
	XmlComplexContentImpl.cpp\
	XmlContext.cpp\
	XmlCursor.cpp\
	XmlErrorCodes.cpp\
	XmlObjectBase.cpp\
	XmlParser.cpp\
	XmlTokenSource.cpp
nodist_libxmlbeansxx_la_SOURCES = \
	XmlTypesGen.h\
	XmlTypesGenImpl.h\
	XmlTypesGen.cpp\
	xml-fragment.h\
	xml-fragmentImpl.h\
	xml-fragment.cpp\
	xmlbeansxx-schema.h\
	xmlbeansxx-schemaImpl.h\
	xmlbeansxx-schema.cpp



BUILT_SOURCES = \
	XmlTypesGen.h\
	XmlTypesGenImpl.h\
	XmlTypesGen.cpp\
	xml-fragment.h\
	xml-fragmentImpl.h\
	xml-fragment.cpp\
	xmlbeansxx-schema.h\
	xmlbeansxx-schemaImpl.h\
	xmlbeansxx-schema.cpp

libxmlbeansxxincludedir = $(includedir)/xmlbeansxx
libxmlbeansxxinclude_HEADERS = \
	Array.h\
	BeansException.h\
	BoostAssert.h\
	cast.h\
	CoreTypes.h\
	CXXBooleanHolder.h\
	CXXDecimalHolder.h\
	CXXDoubleHolder.h\
	CXXFloatHolder.h\
	CXXIntegerHolder.h\
	CXXIntHolder.h\
	CXXLongHolder.h\
	CXXStringHolder.h\
	defs.h\
	Existence.h\
	LibXMLParser.h\
	Map.h\
	Mutex.h\
	MutexPool.h\
	Null.h\
	Pair.h\
	Ptr.h\
	QName.h\
	SchemaProperty.h\
	SchemaPropertyImpl.h\
	SchemaType.h\
	SchemaTypeImpl.h\
	SchemaTypeLoaderBase.h\
	SchemaTypeLoader.h\
	SchemaTypeLoaderImpl.h\
	Set.h\
	SimpleValue.h\
	Stack.h\
	StoreString.h\
	String.h\
	TextUtils.h\
	Tracer.h\
	TypeStore.h\
	TypeStoreImpl.h\
	TypeStoreUser.h\
	TypeStoreVisitor.h\
	ValidationContext.h\
	Vector.h\
	wml_types.h\
	XercesParser.h\
	XercesUtils.h\
	XmlArray.h\
	XmlBeans.h\
	xmlbeansxx.h\
	XmlComplexContentImpl.h\
	XmlContext.h\
	XmlCursor.h\
	XmlErrorCodes.h\
	XmlException.h\
	XmlObjectBase.h\
	XmlOptions.h\
	XmlParser.h\
	XmlTokenSource.h


nodist_libxmlbeansxxinclude_HEADERS = \
	XmlTypesGen.h\
	xml-fragment.h\
	xmlbeansxx-schema.h

XmlTypesGen.h XmlTypesGen.cpp: ${top_srcdir}/generator/dist/bin/xmlbeansxx-genbuiltin ${srcdir}/XmlTypesGen.xh ${srcdir}/XmlTypesGen.xcpp
	${top_builddir}/generator/dist/bin/xmlbeansxx-genbuiltin

xml-fragment.h xml-fragment.cpp: ${top_srcdir}/generator/dist/bin/xmlbeansxx-gen ${srcdir}/xml-fragment.xsd
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen ${srcdir}/xml-fragment.xsd

xmlbeansxx-schema.h xmlbeansxx-schema.cpp: ${top_srcdir}/generator/dist/bin/xmlbeansxx-gen ${srcdir}/xmlbeansxx-schema.xsd ${srcdir}/xmlbeansxx-schema.xh ${srcdir}/xmlbeansxx-schema.xcpp
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen ${srcdir}/xmlbeansxx-schema.xsd

MOSTLYCLEANFILES=XmlTypesGen.h XmlTypesGenImpl.h XmlTypesGen.cpp xml-fragment.h xml-fragmentImpl.h xml-fragment.cpp xmlbeansxx-schema.h xmlbeansxx-schemaImpl.h xmlbeansxx-schema.cpp
