


#stamp-build: $(shell find . -regex '\./\(src\|pom\.xml\).*' -not -regex '.*/\..*' | grep -v stamp)
#	cp ${srcdir}/pom.xml .; mvn -Dbase=${srcdir} -Prelease package 
#	touch stamp-build



ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/pom.xml ${CMAKE_CURRENT_BINARY_DIR}/
)

SET(GENDIR ${PROJECT_BINARY_DIR}/generator/target/xmlbeansxx-gen/)
SET(GEN_BINFILES 
	${GENDIR}/bin/scompxx
	${GENDIR}/bin/xmlbeansxx-geninclude
	${GENDIR}/bin/xmlvalidator
	${GENDIR}/bin/xmlbeansxx-gen
	${GENDIR}/bin/acx_pthread.m4
	${GENDIR}/bin/xmlbeansxx-genbuiltin
	${GENDIR}/bin/xmlbeansxx-split
	${GENDIR}/bin/xmlbeansxx-gen.bat
	${GENDIR}/bin/xmlbeansxx-genbuiltin.bat
}
SET(GEN_LIBFILES
	${GENDIR}/lib/commons-logging-1.1.jar
	${GENDIR}/lib/xbean-1.0.4.jar
	${GENDIR}/lib/log4j-1.2.12.jar
}
SET(GEN_JAR
	${GENDIR}/xmlbeansxx-gen.jar
)


ADD_CUSTOM_COMMAND(OUTPUT ${XMLBEANSXX_GENBUILDIN_PROG} ${XMLBEANSXX_GEN_PROG} ${GEN_BINFILES} ${GEN_LIBFILES}
	COMMAND mvn -Dbase=${CMAKE_CURRENT_SOURCE_DIR} -Prelease package
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
)

ADD_CUSTOM_TARGET(generator
	DEPENDS ${XMLBEANSXX_GEN_PROG}
)



#INSTALL(CODE EXECUTE_PROCESS(COMMAND ${CMAKE} -E cp 

#
#INSTALL(DIRECTORY ${PROJECT_BINARY_DIR}/generator/target/xmlbeansxx-gen/ 
#	DESTINATION share/xmlbeansxx
#	PATTERN "bin/*" PERMISSIONS 	OWNER_EXECUTE	OWNER_WRITE	OWNER_READ
#	                      		GROUP_EXECUTE			GROUP_READ
#					WORLD_EXECUTE			WORLD_READ
#)
#