SUBDIRS =
EXTRA_DIST = log4j.properties c.xml c2.xml c3.xml b.xsd c.xsd easypo.xml EasyPO.xsd
INCLUDES = -I${top_srcdir}/src
LDADD = ${top_builddir}/src/xmlbeansxx/libxmlbeansxx.la

SCHEMAS_XSD = ${srcdir}/c.xsd ${srcdir}/EasyPO.xsd
SCHEMAS_CPP = c.cpp EasyPO.cpp
SCHEMAS_H = c.h cImpl.h EasyPO.h EasyPOImpl.h

BUILT_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

$(SCHEMAS_CPP) $(SCHEMAS_H): ${top_srcdir}/generator/dist/bin/xmlbeansxx-gen $(SCHEMAS_XSD)
	${top_builddir}/generator/dist/bin/xmlbeansxx-gen $(SCHEMAS_XSD)

noinst_PROGRAMS = create parse parse2 testEasyPO

create_SOURCES = create.cpp
nodist_create_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

parse2_SOURCES = parse2.cpp
nodist_parse2_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

testEasyPO_SOURCES = testEasyPO.cpp
nodist_testEasyPO_SOURCES = $(SCHEMAS_CPP) $(SCHEMAS_H)

parse_SOURCES = parse.cpp

MOSTLYCLEANFILES = $(SCHEMAS_CPP) $(SCHEMAS_H)
