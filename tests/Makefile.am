SUBDIRS =
EXTRA_DIST = a.xml b.xml c.xml c.xsd c.xh entities.xml entities.xsd log4j.xml


INCLUDES =
LIBADD =
LDADD =

if HAVE_CPPUNIT
AM_CPPFLAGS = $(CPPUNIT_CFLAGS) -g 
LDADD += $(CPPUNIT_LIBS)
endif

INCLUDES += -I ../src
LDADD += ../src/xmlbeansxx/libxmlbeansxx.la

SCHEMAS_XSD = c.xsd entities.xsd
SCHEMAS_CPP = c.cpp entities.cpp
SCHEMAS_H = c.h entities.h

BUILT_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

$(SCHEMAS_CPP) $(SCHEMAS_H): $(SCHEMAS_XSD) c.xh
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen $(SCHEMAS_XSD)

TESTS=BeansTest GenTest EntitiesTest
check_PROGRAMS = $(TESTS)
BeansTest_SOURCES=TestApp.cpp BeansTest.cpp BeansTest.h
GenTest_SOURCES=TestApp.cpp GenTest.cpp GenTest.h 
EntitiesTest_SOURCES=TestApp.cpp EntitiesTest.cpp EntitiesTest.h entities.h entities.cpp
nodist_GenTest_SOURCES = c.h c.cpp

noinst_PROGRAMS = mem01 mem02 mem03
mem01_SOURCES = mem01.cpp
mem02_SOURCES = mem02.cpp
mem03_SOURCES = mem03.cpp
nodist_mem02_SOURCES = c.h c.cpp


MOSTLYCLEANFILES = $(SCHEMAS_CPP) $(SCHEMAS_H)

