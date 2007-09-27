


#stamp-build: $(shell find . -regex '\./\(src\|pom\.xml\).*' -not -regex '.*/\..*' | grep -v stamp)
#	cp ${srcdir}/pom.xml .; mvn -Dbase=${srcdir} -Prelease package 
#	touch stamp-build


ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
	COMMAND ${CMAKE_COMMAND} copy ${CMAKE_CURRENT_SOURCE_DIR}/pom.xml .
	)

ADD_CUSTOM_TARGET( generator 
	mvn -Dbase=${CMAKE_CURRENT_SOURCE_DIR} -Prelease package
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
	)
	
add_dependencies (generator xmlbeansxx)


SET(xmlbeansxx-genbuiltin ./bolek)
SET(xmlbeansxx-gen ./bolek2)
