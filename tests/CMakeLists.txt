

SET(XMLBEANSXX_OPTIONAL_LIBS "")
SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS "")


IF (LIBXML2_FOUND)
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS} ${LIBXML2_LIBRARIES})
	SET(XMLBEANSXX_EXTRA_INCLUDE_DIRS ${XMLBEANSXX_EXTRA_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
ENDIF (LIBXML2_FOUND)

IF (GMPXX_FOUND)
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS}  ${GMP_LIBRARY} ${GMPXX_LIBRARY})
ENDIF (GMPXX_FOUND)


IF (LOG4CXX_FOUND)
	SET(XMLBEANSXX_OPTIONAL_LIBS  ${XMLBEANSXX_OPTIONAL_LIBS}  ${LOG4CXX_LIBRARY})
ENDIF (LOG4CXX_FOUND)





SET(TEST_LIBS
	${XMLBEANSXX_LIB}
	${XERCES_LIBRARY}
	${BOOST_DATE_TIME_LIBRARY}
	${BOOST_THREAD_LIBRARY}
	${CPPUNIT_LIBRARY}

	${XMLBEANSXX_OPTIONAL_LIBS}
)



SET(TEST_PROGS
	BeansTest
	XmlArrayTest
	ArrayTest
#	GenTest
#	ParseTest
#	UnqualTest
	EnumTest
	XPathTest
	EntitiesTest
	XmlCursorTest
)



include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/src/xmlbeansxx
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_BINARY_DIR}/src/xmlbeansxx
	${PROJECT_BINARY_DIR}/src
	${XMLBEANSXX_EXTRA_INCLUDE_DIRS}
)


XMLBEANSXX_GEN(c.xsd 		c.cpp 		c.h)
XMLBEANSXX_GEN(pawel.xsd 	pawel.cpp 	pawel.h)
XMLBEANSXX_GEN(unqual.xsd 	unqual.cpp 	unqual.h)
XMLBEANSXX_GEN(entities.xsd 	entities.cpp 	entities.h)

XMLBEANSXX_COPY(log4j.xml.default	log4j.xml)
XMLBEANSXX_COPY(entities.xml.default	entities.xml)

SET(EXTRA_FILES ${log4j.xml_FILE} ${entities.xml_FILE})


ADD_EXECUTABLE(BeansTest	TestApp.cpp BeansTest.cpp	BeansTest.h 		${EXTRA_FILES}					)
ADD_EXECUTABLE(XmlArrayTest	TestApp.cpp XmlArrayTest.cpp	XmlArrayTest.h		${EXTRA_FILES}					)
ADD_EXECUTABLE(ArrayTest	TestApp.cpp ArrayTest.cpp	ArrayTest.h		${EXTRA_FILES}	${c.xsd_SRC}			)
#ADD_EXECUTABLE(GenTest		TestApp.cpp GenTest.cpp		GenTest.h		${EXTRA_FILES}	${c.xsd_SRC} ${pawel.xsd_SRC}	)
#ADD_EXECUTABLE(ParseTest	TestApp.cpp ParseTest.cpp 	ParseTest.h 		${EXTRA_FILES}	${c.xsd_SRC}			)
#ADD_EXECUTABLE(UnqualTest	TestApp.cpp UnqualTest.cpp 	UnqualTest.h 		${EXTRA_FILES}	${unqual.xsd_SRC}		)
ADD_EXECUTABLE(EnumTest		TestApp.cpp EnumTest.cpp 	EnumTest.h		${EXTRA_FILES}	${pawel.xsd_SRC}		)
ADD_EXECUTABLE(XPathTest	TestApp.cpp XPathTest.cpp 	XPathTest.h		${EXTRA_FILES}	${pawel.xsd_SRC}		)
ADD_EXECUTABLE(EntitiesTest	TestApp.cpp EntitiesTest.cpp 	EntitiesTest.h		${EXTRA_FILES}	${entities.xsd_SRC}		)
ADD_EXECUTABLE(XmlCursorTest	TestApp.cpp XmlCursorTest.cpp 	XmlCursorTest.h		${EXTRA_FILES}					)



FOREACH(P ${TEST_PROGS})
	TARGET_LINK_LIBRARIES(${P} ${TEST_LIBS})
	ADD_TEST("${P}_TEST" ${P})
ENDFOREACH(P)

