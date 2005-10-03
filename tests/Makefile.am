SUBDIRS =
EXTRA_DIST = a.xml b.xml c.xml c.xsd log4j.properties

INCLUDES = -I ${top_srcdir}/src
LDADD = ${top_builddir}/src/xmlbeansxx/libxmlbeansxx.la

if HAVE_CPPUNIT
CXXFLAGS += $(CPPUNIT_CFLAGS) 
LDADD += $(CPPUNIT_LIBS)
endif

c.xsd: ${srcdir}/c.xsd-src
	cp ${srcdir}/c.xsd-src c.xsd

SCHEMAS_XSD = c.xsd
SCHEMAS_CPP = c.cpp
SCHEMAS_H = c.h cImpl.h

BUILT_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

$(SCHEMAS_CPP) $(SCHEMAS_H): $(SCHEMAS_XSD)
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen $(SCHEMAS_XSD)

TESTS=BeansTest GenTest
check_PROGRAMS = $(TESTS)
BeansTest_SOURCES=TestApp.cpp BeansTest.cpp BeansTest.h
GenTest_SOURCES=TestApp.cpp GenTest.cpp GenTest.h 
nodist_GenTest_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

noinst_PROGRAMS = speed01
speed01_SOURCES = speed01.cpp 
nodist_speed01_SOURCES = c.h c.cpp

MOSTLYCLEANFILES = $(SCHEMAS_CPP) $(SCHEMAS_H) $(SCHEMAS_XSD)

