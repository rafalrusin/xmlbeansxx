


#stamp-build: $(shell find . -regex '\./\(src\|pom\.xml\).*' -not -regex '.*/\..*' | grep -v stamp)
#	cp ${srcdir}/pom.xml .; mvn -Dbase=${srcdir} -Prelease package 
#	touch stamp-build



ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/pom.xml ${CMAKE_CURRENT_BINARY_DIR}/
)


ADD_CUSTOM_COMMAND(OUTPUT ${XMLBEANSXX_GENBUILDIN_PROG} ${XMLBEANSXX_GEN_PROG} ${PROJECT_BINARY_DIR}/generator/target/xmlbeansxx-gen/bin/xmlbeansxx-gen
	COMMAND mvn -Dbase=${CMAKE_CURRENT_SOURCE_DIR} -Prelease package
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
)

ADD_CUSTOM_TARGET(generator
	DEPENDS ${XMLBEANSXX_GEN_PROG}
)


