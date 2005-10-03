SUBDIRS = 
EXTRA_DIST = xml-fragment.xsd xmlbeansxx-schema.xsd XmlTypesGen.xh XmlTypesGen.xcpp xmlbeansxx-schema.xh xmlbeansxx-schema.xcpp

CXXFLAGS += -I${srcdir}/../

lib_LTLIBRARIES = libxmlbeansxx.la
libxmlbeansxx_la_LDFLAGS = -version-info $(library_version_info)
<<< 
inside() {
what="$1"
shift
for _i;do
if [ "$_i" = "$what" ]; then return 0; fi
done
return 1
}
f() {
for i in $WHAT; do 
    if ! inside $i $WITHOUT; then echo "	$i\\"; fi
done 
}
dump() {
n=`f | wc -c`
f | head -c $[$n-2]
echo
}

NODIST='XmlTypesGen.h XmlTypesGenImpl.h XmlTypesGen.cpp xml-fragment.h xml-fragmentImpl.h xml-fragment.cpp xmlbeansxx-schema.h xmlbeansxx-schemaImpl.h xmlbeansxx-schema.cpp'

echo 'libxmlbeansxx_la_SOURCES = \'
WITHOUT="$NODIST"
WHAT="*.h *.cpp"
dump

echo 'nodist_libxmlbeansxx_la_SOURCES = \'
WITHOUT=""
WHAT="$NODIST"
dump
>>>


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
<<<
f() {
for i in *.h; do
    echo "	$i\\"
done 
}
dump() {
n=`f | wc -c`
f | head -c $[$n-2]
echo
}
dump
>>>

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
