SUBDIRS =
EXTRA_DIST = a.xml b.xml c.xml c.xsd c.xh log4j.properties


INCLUDES =
LIBADD =
LDADD =

if HAVE_CPPUNIT
AM_CPPFLAGS = $(CPPUNIT_CFLAGS) 
LDADD += $(CPPUNIT_LIBS)
endif

INCLUDES += -I ../src
LDADD += ../src/xmlbeansxx/libxmlbeansxx.la

SCHEMAS_XSD = c.xsd
SCHEMAS_CPP = c.cpp
SCHEMAS_H = c.h

BUILT_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

$(SCHEMAS_CPP) $(SCHEMAS_H): $(SCHEMAS_XSD) c.xh
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen $(SCHEMAS_XSD)

TESTS=BeansTest GenTest
check_PROGRAMS = $(TESTS)
BeansTest_SOURCES=TestApp.cpp BeansTest.cpp BeansTest.h
GenTest_SOURCES=TestApp.cpp GenTest.cpp GenTest.h 
nodist_GenTest_SOURCES = c.h c.cpp

noinst_PROGRAMS = mem01 mem02
mem01_SOURCES = mem01.cpp
mem02_SOURCES = mem02.cpp
nodist_mem02_SOURCES = c.h c.cpp

MOSTLYCLEANFILES = $(SCHEMAS_CPP) $(SCHEMAS_H)

