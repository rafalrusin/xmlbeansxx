




SET(GENDIR ${PROJECT_BINARY_DIR}/generator/target/xmlbeansxx-gen/)

SET(GEN_UNIX_BIN_LINKS scompxx xmlbeansxx-geninclude xmlbeansxx-gen xmlbeansxx-split)
SET(GEN_BINFILES 
	${GENDIR}/bin/acx_pthread.m4
	${GENDIR}/bin/scompxx
	${GENDIR}/bin/xmlbeansxx-geninclude
	${GENDIR}/bin/xmlvalidator
	${GENDIR}/bin/xmlbeansxx-gen
	${GENDIR}/bin/xmlbeansxx-genbuiltin
	${GENDIR}/bin/xmlbeansxx-split
	${GENDIR}/bin/xmlbeansxx-gen.bat
	${GENDIR}/bin/xmlbeansxx-genbuiltin.bat
)
SET(GEN_LIBFILES
	${GENDIR}/lib/commons-logging-1.1.jar
	${GENDIR}/lib/xmlbeans-2.4.0.jar
	${GENDIR}/lib/log4j-1.2.12.jar
)
SET(GEN_JAR
	${GENDIR}/xmlbeansxx-gen.jar
)


ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/pom.xml ${CMAKE_CURRENT_BINARY_DIR}/
)

ADD_CUSTOM_COMMAND(OUTPUT ${GEN_BINFILES} ${GEN_LIBFILES} ${GEN_JAR}
	COMMAND ${MVN_PROG} -Dbase=${CMAKE_CURRENT_SOURCE_DIR} -Prelease package
	MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pom.xml
)

ADD_CUSTOM_TARGET(generator
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pom.xml ${GEN_BINFILES} ${GEN_LIBFILES} ${GEN_JAR} 
)


INSTALL(FILES ${GEN_BINFILES} 
	DESTINATION share/xmlbeansxx/bin
	PERMISSIONS 	OWNER_EXECUTE	OWNER_WRITE	OWNER_READ
              		GROUP_EXECUTE			GROUP_READ
			WORLD_EXECUTE			WORLD_READ
)
INSTALL(FILES ${GEN_LIBFILES}
	DESTINATION share/xmlbeansxx/lib
)

INSTALL(FILES ${GEN_JAR}
	DESTINATION share/xmlbeansxx
)

IF(NOT WIN32)
	FOREACH(LINK ${GEN_UNIX_BIN_LINKS})
		INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \"${CMAKE_INSTALL_PREFIX}/share/xmlbeansxx/bin/${LINK}\"  \"${CMAKE_INSTALL_PREFIX}/bin/${LINK}\" )")
	ENDFOREACH(LINK)
ENDIF(NOT WIN32)

