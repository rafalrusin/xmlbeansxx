SUBDIRS =
EXTRA_DIST = a.xml b.xml c.xml c.xsd c.xh entities.xml entities.xsd log4j.xml pawel.xsd


LIBADD =
LDADD =

if HAVE_CPPUNIT
AM_CPPFLAGS = $(CPPUNIT_CFLAGS) -g 
LDADD += $(CPPUNIT_LIBS)
endif

INCLUDES = -I ${top_srcdir}/src -I ${top_builddir}/src -I ${top_builddir}/src/xmlbeansxx -I ${top_srcdir}/src/xmlbeansxx 
LDADD += ${top_builddir}/src/xmlbeansxx/libxmlbeansxx.la

SCHEMAS_XSD = ${srcdir}/c.xsd ${srcdir}/entities.xsd ${srcdir}/pawel.xsd
SCHEMAS_CPP = c.cpp entities.cpp pawel.cpp
SCHEMAS_H = c.h entities.h pawel.h

BUILT_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

$(SCHEMAS_CPP) $(SCHEMAS_H): $(SCHEMAS_XSD) c.xh
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen -Ddownload $(SCHEMAS_XSD)

TESTS=BeansTest GenTest EntitiesTest ArrayTest EnumTest
check_PROGRAMS = $(TESTS)
BeansTest_SOURCES=TestApp.cpp BeansTest.cpp BeansTest.h
ArrayTest_SOURCES=TestApp.cpp ArrayTest.cpp ArrayTest.h c.cpp
GenTest_SOURCES=TestApp.cpp GenTest.cpp GenTest.h 
EnumTest_SOURCES=TestApp.cpp EnumTest.cpp EnumTest.h pawel.h pawel.cpp
EntitiesTest_SOURCES=TestApp.cpp EntitiesTest.cpp EntitiesTest.h entities.h entities.cpp
nodist_GenTest_SOURCES = c.h c.cpp

noinst_PROGRAMS = mem01 mem02 mem03
mem01_SOURCES = mem01.cpp
mem02_SOURCES = mem02.cpp
mem03_SOURCES = mem03.cpp
nodist_mem02_SOURCES = c.h c.cpp


MOSTLYCLEANFILES = $(SCHEMAS_CPP) $(SCHEMAS_H)

