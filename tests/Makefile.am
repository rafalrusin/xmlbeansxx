EXTRA_DIST = a.xml b.xml bad_c.xml c.xml c.xsd c.xh entities.xml entities.xsd log4j.xml pawel.xsd unqual.xml unqual.xsd


LIBADD =
LDADD =

if HAVE_CPPUNIT
AM_CPPFLAGS = $(CPPUNIT_CFLAGS) -g 
LDADD += $(CPPUNIT_LIBS)
endif

INCLUDES = -I ${top_srcdir}/src -I ${top_builddir}/src -I ${top_builddir}/src/xmlbeansxx -I ${top_srcdir}/src/xmlbeansxx 
LDADD += ${top_builddir}/src/xmlbeansxx/libxmlbeansxx.la

SCHEMAS_XSD = ${srcdir}/c.xsd ${srcdir}/entities.xsd ${srcdir}/pawel.xsd ${srcdir}/unqual.xsd
SCHEMAS_CPP = c.cpp entities.cpp pawel.cpp unqual.cpp
SCHEMAS_H = c.h entities.h pawel.h unqual.h
DATA_FILES = $(EXTRA_DIST)

BUILT_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

$(SCHEMAS_CPP) $(SCHEMAS_H): $(SCHEMAS_XSD) c.xh ${top_builddir}/generator/stamp-build
	${top_builddir}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen -Ddownload $(SCHEMAS_XSD)

TESTS=BeansTest GenTest EntitiesTest ArrayTest EnumTest XPathTest ParseTest UnqualTest XmlArrayTest NamespaceTest
check_PROGRAMS = $(TESTS)

BeansTest_SOURCES=TestApp.cpp BeansTest.cpp BeansTest.h

XmlArrayTest_SOURCES=TestApp.cpp XmlArrayTest.cpp XmlArrayTest.h

ArrayTest_SOURCES=TestApp.cpp ArrayTest.cpp ArrayTest.h
nodist_ArrayTest_SOURCES = c.cpp

GenTest_SOURCES=TestApp.cpp GenTest.cpp GenTest.h 
nodist_GenTest_SOURCES = c.h c.cpp pawel.h pawel.cpp

ParseTest_SOURCES=TestApp.cpp ParseTest.cpp ParseTest.h 
nodist_ParseTest_SOURCES = c.h c.cpp pawel.h pawel.cpp

UnqualTest_SOURCES=TestApp.cpp UnqualTest.cpp UnqualTest.h 
nodist_UnqualTest_SOURCES = unqual.h unqual.cpp

EnumTest_SOURCES=TestApp.cpp EnumTest.cpp EnumTest.h
nodist_EnumTest_SOURCES=pawel.h pawel.cpp

NamespaceTest_SOURCES=TestApp.cpp NamespaceTest.cpp NamespaceTest.h

XPathTest_SOURCES=TestApp.cpp XPathTest.cpp XPathTest.h
nodist_XPathTest_SOURCES=pawel.h pawel.cpp

EntitiesTest_SOURCES=TestApp.cpp EntitiesTest.cpp EntitiesTest.h
nodist_EntitiesTest_SOURCES= entities.h entities.cpp

noinst_PROGRAMS = mem01 mem02 mem03
mem01_SOURCES = mem01.cpp
mem02_SOURCES = mem02.cpp
mem03_SOURCES = mem03.cpp
nodist_mem02_SOURCES = c.h c.cpp


MOSTLYCLEANFILES = $(SCHEMAS_CPP) $(SCHEMAS_H)


copy_data: 
	if [ "x`readlink -f ${srcdir}`" != "x`readlink -f .`" ]; then for i in $(DATA_FILES); do   cp ${srcdir}/$$i  . ; done ; true ;  fi 

rm_data: 
	if [ "x`readlink -f ${srcdir}`" != "x`readlink -f .`"  ]; then  rm -f $(DATA_FILES) xmldsig-core-schema.h xmldsig-core-schema.cpp  ; true ;  fi

check: copy_data

clean-local: rm_data
